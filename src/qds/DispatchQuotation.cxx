/*
 * $Id: $
 *
 * Dispatching quotation data (implementation)
 *
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */

#include "Base.h"
#include "Log.h"
#include "Trace.h"

#include "DispatchQuotation.h"
// #include "AlgoManager.h"

#include "algoapi/DataServiceRedis.h"
#include "pool/SessionMap.h"
#include "pool/MemoryPool.h"
#include "worker/WorkerThread.h"

#include <sstream>
#include <unordered_set>
#include <unistd.h>

using namespace ATS;
using namespace AlgoApi;

#define SUBSCRIBE_TC        '+'
#define UNSUBSCRIBE_TC      '-'
#define MAX_QDATA_SIZE      1024
#define MAX_QD_CODE         32
#define MIN_QD_PACKED       4
#define MAX_QD_PACKED       (MAX_QDATA_SIZE - sizeof(QDataItem))

class QDataItem {
public:
    char                type;               /**< Quotation::Type (tc)   */
    char               _reserved_c;         /**< reserved for alignment */
    short               market;             /**< market */
    char                code[MAX_QD_CODE];  /**< Qutotaion::Code (code) */

    union {
        struct {
            size_t      n_buflen;           /**< length of packed in bytes */
            char        packed_buf[1];      /**< packed data buffer        */
        } data;

        PCtxAlgoRule    ctx;
    };

protected:
    /** Contructor of quotation data */
    QDataItem(QuotationType tc, const char *cd, const char *packed, int plen) {
        type = (char)tc;
        market = 0;
        strncpy(code, cd, sizeof(code));
        data.n_buflen = (size_t)plen;
        memcpy(data.packed_buf, packed, plen);
    }

    /** Contructor of subscribe/unscribe request */
    QDataItem(int tc, string &filter, PCtxAlgoRule algo_ctx) {
        type = (char)tc;
        market = 0;
        strncpy(code, filter.c_str(), sizeof(code));
        ctx = algo_ctx;
    }

public:
    static QDataItem *Create(QuotationType tc, const char *code, const char *packed, int plen) {
        char *block = new char[MAX_QDATA_SIZE];
        QDataItem *item = new(block) QDataItem(tc, code, packed, plen);
        // return QuotationIntf::ParseQuotation(tc, code, packed, plen);
        return item;
    }

    static QDataItem *Create(int tid, string &filter, PCtxAlgoRule algo_ctx) {
        char *block = new char[sizeof(QDataItem)];
        return new(block) QDataItem(tid, filter, algo_ctx);
    }

    void Dispose() {
        //this->~QDataItem();
        //delete [] (char *)this;
        delete this;
    }
};

typedef unordered_set<PCtxAlgoRule> TContextSet;

struct QDataMapItem {
    string                  subpath;    /**< subscribe path, 
                                         *   e.g., S_1_600000  (stock.SH.600000)  
                                         */
    string                  token;      /**< published code token = {tc}.{code} 
                                         *   e.g., S_1_600000 (stock.SH.600000)
                                         */
    QuotationData           data;       /**< quotation data */

    TContextSet             contexts;   /**< subscribed context set */
};

class ZmqMonitorThread : public WorkerThread
{
private:
    static const char * ZMQ_MONITOR_ADDR;

    static int get_monitor_event(void *sock, uint32_t *value, char *addr) {
        // First frame in message contains event number and value
        zmq_msg_t msg;
        zmq_msg_init (&msg);
        if (zmq_msg_recv(&msg, sock, 0) == -1) 
            return -1; // Interruped, presumably
        assert (zmq_msg_more (&msg));

        uint8_t *data = (uint8_t *) zmq_msg_data (&msg);
        uint16_t event = *(uint16_t *) (data);
        if (value) *value = *(uint32_t *) (data + 2);

        // Second frame in message contains event address
        zmq_msg_init(&msg);
        if (zmq_msg_recv(&msg, sock, 0) == -1)
            return -1; // Interruped, presumably
        assert (!zmq_msg_more(&msg));

        if (addr) {
            uint8_t *data = (uint8_t *) zmq_msg_data(&msg);
            size_t size = zmq_msg_size(&msg);
            memcpy(addr, data, size);
            addr[size] = '\0';
        }
        return event;
    }

    static void *sub_socket_monitor(void *zmq_ctx) {
        ATS::thread_t tid = WorkerThread::GetId();

        LOGFILE(LOG_DEBUG, "[0x%x] ZmqMonitorThread starting...", tid);

        void *sock = zmq_socket(zmq_ctx, ZMQ_PAIR);
        assert(NULL != sock);
        int rc = zmq_connect(sock, ZMQ_MONITOR_ADDR);
        assert(rc == 0);

        int is_connected = 1;   // TODO check zmq connection status
        if (is_connected) {
            DispatchQuotation::SubscribeDefault();            
        }

        do {
            uint32_t value;
            char addr[256];
            int event = get_monitor_event(sock, &value, addr);

            switch (event) {
            case ZMQ_EVENT_CONNECTED:
                DispatchQuotation::SubscribeDefault();
                LOGFILE(LOG_INFO, "Quotation server '%s' connected", addr);
                break;

            case ZMQ_EVENT_DISCONNECTED:
                LOGFILE(LOG_ERROR, "Quotation server '%s' disconnected", addr);
                break;

            case ZMQ_EVENT_CLOSED:
            case ZMQ_EVENT_CONNECT_RETRIED:
            case ZMQ_EVENT_CONNECT_DELAYED:
                TRACE_THREAD(8, "socket event %d on address '%s'", event, addr); 
                break;

            default:
                LOGFILE(LOG_WARN, "socket event %d on address '%s'", event, addr);
                break;
            }

        } while (!g_is_exiting);

        zmq_close(sock);

        LOGFILE(LOG_DEBUG, "[0x%x] ZmqMonitorThread exited.", tid);
        return NULL;
    }

public:
    ZmqMonitorThread(BaseZmqClient *client) {
        int rc;
        void *zmq_sock = client->GetSocket();
        void *zmq_ctx  = client->GetContext();
        assert(NULL != zmq_sock);
        if (0 == (rc = zmq_socket_monitor(zmq_sock, ZMQ_MONITOR_ADDR, ZMQ_EVENT_ALL))) {
            WorkerThread::Run(sub_socket_monitor, zmq_ctx);
        } else {
            LOGFILE(LOG_ERROR, "zmq_socket_monitor failed: ", zmq_strerror(rc));
        }
    }

    virtual ~ZmqMonitorThread() {
        if (WorkerThread::WaitStop(2000 /*MAX_WAIT*/) < 0) {
            WorkerThread::Kill();
        }
    }
};

const char *ZmqMonitorThread::ZMQ_MONITOR_ADDR = "inproc://monitor.sub";

// TODO: message pool
// MemoryPool<QDataItem> l_data_pool(1024, sizeof(QDataItem), 1024);

//static _THREAD_LOCAL_ DispatchQuotation *_instance = NULL;
static DispatchQuotation *_instance = NULL;

/** Get Market Id
 * See QuotationData::GetMarket()
 * 0-前台未知交易所 1-上海 2-深圳 3-上海B 4-深圳B 5-中金所 6-上期所 7-大商所 8-郑商所 9-特转A 10-特转B, 11-三板 
 */
static void getMarket(const string &market, OUT string &filter)
{
    filter += market;
}

static void getFilter(const QuotationType type, OUT string &filter)
{
    filter += (char)type;
} 

static void getFilter(const QuotationType type, const string &market, 
                      const string &code, OUT string &filter)
{
    getFilter(type, filter);
    filter += "_";

    if (!market.empty()) {
        getMarket(market, filter);
        filter += "_";
    }
    filter += code;
}

static void getFilter(const QuotationType tc, int market, 
                      const string &code, OUT string &filter)
{
    stringstream ss;

    if (market <= 0) {
        market = QuotationData::GetMarket(code);
    }
    ss << market;
    getFilter(tc, ss.str(), code, filter);
}


static inline void getToken(const QuotationType tc, int market,  
                     const string &code, OUT string &token)
{
    // TRACE_THREAD(7, "---- getToken(%c_%d_%s) ----", tc, market, code.c_str()); 
    getFilter(tc, market, code, token);
}

static QuotationType getQuotationType(const string &token)
{
    QuotationType tc;
    switch (token[0]) {
    case (char)QT_STOCK:    tc = QT_STOCK;  break;
    case (char)QT_FUTURE:   tc = QT_FUTURE; break;
    case (char)QT_OPTION:   tc = QT_OPTION; break;
    default:
        LOGFILE(LOG_WARN, "unknown QuotationType %c", token[0]);
        tc = QT_MAX_INVALID;
        break;
    }
    return tc;
}

static void ShrinkMap(QDataMapItem *mapItem)
{
    // TODO
}

QDataMapItem *GetDataItem(const string &token)
{
    const char *tkey = token.c_str();
    QDataMapItem *map_item = SessionMap::InstanceQD().Get<QDataMapItem *>(tkey);

    if (NULL == map_item) {
        QuotationType type = getQuotationType(token); // S_1_600000 => 'S'
        DataServiceRedis &ds = DataServiceRedis::Instance();
        string po;
        PMessage msg = NULL;

        ds.LoadQuotation(token, po);    // load from redis

        if (po.length() > 0) {
            TRACE(7, "Loaded quotation data '%s' from redis", tkey);
            msg = QuotationIntf::ParseQuotation(type, "", po.c_str(), po.length());
            assert(NULL != msg);
        } else {
            LOGFILE(LOG_WARN, "No quotation data '%s' in redis", tkey);
            msg = QuotationIntf::NewQuotation(type, tkey);
        }

        if (NULL != msg) {
            // save into cache
            map_item = new QDataMapItem();
            map_item->data.Push(msg);
            SessionMap::InstanceQD().Set(tkey, map_item); // thread-safe concurrent_set
            QuotationIntf::DisposeQuotation(msg);
        }
    }
    return map_item; 
}

Object DispatchQuotation::m_parser(const QuotationType tc, const char *code, 
                                   const char *packed, int plen)
{
    // NOT parse, just alloacate a ring-buffer for containing packet
    // tc, code
    // (packed, len)

    assert(plen <= (int)MAX_QD_PACKED);
    if (plen > (int)MAX_QD_PACKED) {
        LOGFILE(LOG_WARN, "Got invalid quotation data (%c %s, length=%d), ignored", tc, code, plen);
        return NULL;
    }

    TRACE_THREAD(8, "---- m_parser('%c' %s, %d bytes)", tc, code, plen);

    return QDataItem::Create(tc, code, packed, plen);
}

void DispatchQuotation::m_recv_callback(char tc, const char *code, 
                                        const char *msg, size_t len)
{
    FILE *file = _instance->m_record_file;
    if (NULL != file) {  // record data to given file
        char datebuf[64];
        get_curtime_s(&datebuf[0]);
        fprintf(file, "\n%s %c %s %zu\n", datebuf, tc, code, len);
        fwrite(&len, sizeof(len),  1, file); // length
        fwrite(msg, len, 1, file); // data
    }
}

ostream &DispatchQuotation::ReportStats(ostream &os)
{
    const TCodePool::TCodeSet& codes = _instance->m_code_stats.swap();

    os << "💡  QD ififo overloads: " << _instance->m_overload 
       << "\n💡  QD subscribed hits: " << _instance->m_hits
       << "\n🗞  QD updated ";
 
    TCodePool::ToJson(codes, os, 15);
    // << endl;

#if 0
    QuotationData &data = GetData(1, "600000");
    os << "\n   data { code:" << data.Code() 
       << ", price:" << data.Latest()
       << ", buy2:"  << data.BuyLevel(2)
       << ", sel2:" << data.SellLevel(2)
       << ", ts:" << data.TimeStamp()
       << " }";
#endif

    return os;
}

DispatchQuotation &DispatchQuotation::Attach(PCtxAlgoRule algo_ctx)
{
    assert(NULL != _instance);
    return *_instance;
}

TDispResult DispatchQuotation::onDispatch(Object obj)
{
    int n;
    TDispResult res = DR_SUCCESS;
    
    while ((n = m_queue.push_back(obj)) < 0) { // FIFO is busy
        // report & static overload
        m_overload++;
        if (n == -ENOMEM) {   // FIFO is full or error or debugging
            // TODO: drop it ?
            LOGFILE(LOG_WARN, "quotation queue is busy (overload: %d)", m_overload);
            usleep(1);
            //res = DR_DROPPED;
            //break;
        }
    } // while (fifo)

    return res;
}

int DispatchQuotation::ProcessInfinitely(bool monitor_event)
{
    ZmqMonitorThread *monitor_thread = NULL;
    DataServiceRedis &ds = DataServiceRedis::Instance();

    _instance = this; // TODO (multiple instances)

    // SubscribeDefault();
    while (NULL == m_intf.GetSocket()) {
        usleep(10); // wait for DispatchIO thread starting...
    }

    if (monitor_event) {
        monitor_thread = new ZmqMonitorThread(&m_intf);
    }

    do {
        QDataItem *item = NULL;
        int ret;
        if ((ret = m_queue.pop_front<QDataItem *>(&item)) < 0) {
            // TRACE_THREAD(7, "--- %d ---", ret);
        }

        if (NULL != item) {
            QDataMapItem *map_item;
            PMessage msg = NULL;
            QuotationType type;
            string token;

            switch (item->type) {
            case SUBSCRIBE_TC:
                token = item->code;
                if (!token.empty()) {
                    map_item = GetDataItem(token);
                    assert(NULL != map_item);
                    map_item->contexts.insert(item->ctx);       // NOT thread-safe
                }
                m_intf.Subscribe(token);  // TODO: format not valid, current is S_6000000
                TRACE_THREAD(8, "---- '%s' subscribed ----", token.c_str()); 
                break;

            case UNSUBSCRIBE_TC:
                token = item->code;
                if (!token.empty()) {
                    map_item = SessionMap::InstanceQD().Get<QDataMapItem *>(token);
                    if (NULL != map_item) {
                        assert(NULL != item->ctx);
                        map_item->contexts.erase(item->ctx);    // NOT thread-safe
                        ShrinkMap(map_item);
                    }
                }
                m_intf.Unsubscribe(token);
                TRACE_THREAD(8, "---- '%s' unsubscribed ----", token.c_str()); 
                break;

            default: // published from quotation server
                type = (QuotationType)item->type;
                getToken(type, item->market, item->code, token);
                TRACE_THREAD(8, "---- %s recved published quotation ----", token.c_str());

                msg = QuotationIntf::ParseQuotation(type, item->code, 
                                                    item->data.packed_buf, 
                                                    item->data.n_buflen);

                if (NULL == msg) {
                    // TODO: xiaofeng 2017-04-18, parse 'I_990978' ...
                    // LOGFILE(LOG_WARN, "unknown message %c_%s", type, item->code);
                    break;
                }
                
                // save for statistics
                m_code_stats.emplace(item->code); // token

                // check cache
                map_item = SessionMap::InstanceQD().Get<QDataMapItem *>(token);
                if (NULL == map_item) { // not subscribed internally
                    TRACE_THREAD(8, ">> QD >> code: '%s' (%d bytes) - no hit, updated", 
                                 token.c_str(), item->data.n_buflen);

                    // save to redis with timestamp
                    if (m_sync_redis) {
                        stringstream ss;
                        msg->SerializeToOstream(&ss);
                        ds.SaveQuotation(token, ss.str());
                    }

                } else {                // had subscribed by some algo instances
                    TRACE_THREAD(7, ">> QD >> code: '%s'- hits %d subscriber(s)", 
                                 token.c_str(), map_item->contexts.size()); 
                    
                    map_item->data.Push(msg);

                    for (TContextSet::iterator it = map_item->contexts.begin();
                         it != map_item->contexts.end(); it++) {
                        if (NULL != *it) { // update price instantly
                            m_hits++;
                            // AlgoManager::OnDataFeed((*it), map_item->data);                       
                        }
                    }
                }

                break;

            } // switch (item->type)

            // release buffer resource
            item->Dispose();
 
            // dispatch msg and release memory
            if (NULL != msg) {
                TRACE_THREAD(8, "=== %s ==>\n%s=====>", 
                             msg->GetTypeName().c_str(), 
                             msg->Utf8DebugString().c_str());
                QuotationIntf::DisposeQuotation(msg);
            }
        }

    } while (!g_is_exiting);

    if (NULL != m_record_file) {
        m_intf.SetRecvCallback(NULL);
        fclose(m_record_file);
        m_record_file = NULL;
    }

    m_intf.Stop();
    
    if (NULL != monitor_thread) {
        delete monitor_thread;
    }

    return 0;
}

QuotationData &DispatchQuotation::GetData(const string &code, int market)
{
    string token;
    QuotationType type = QT_STOCK; // TODO: market to type
    
    getToken(type, market, code, token);
    QDataMapItem *map_item = GetDataItem(token);

    // already cached, check timestamp
    assert(NULL != map_item);

    QuotationData &data = map_item->data;

    // TODO: check timestamp
    //TRACE_THREAD(7, "%s timestamp %d", data.Timestamp());

    return data;
}


void DispatchQuotation::Subscribe(PCtxAlgoRule ctx, const QuotationType type, 
                                  const string &code, int market)
{
    string filter;
    getFilter(type, market, code, filter);
    Attach(ctx).onDispatch(QDataItem::Create(SUBSCRIBE_TC, filter, ctx));
}

void DispatchQuotation::SubscribeAll(PCtxAlgoRule ctx, const QuotationType type)
{
    string filter;

    getFilter(type, filter);
    Attach(ctx).onDispatch(QDataItem::Create(SUBSCRIBE_TC, filter, ctx));
}

void DispatchQuotation::SubscribeDefault(PCtxAlgoRule ctx)
{
#if true
    // subscribe for all stocks (from remote publish-sever)
    //SubscribeAll(NULL, QT_STOCK);
    string filter("");
    Attach(ctx).onDispatch(QDataItem::Create(SUBSCRIBE_TC, filter, ctx));
#endif

#if false
    // for test
    SubscribeStock(NULL, "600000");
#endif
}

void DispatchQuotation::Unsubscribe(PCtxAlgoRule ctx, const QuotationType type, 
                                    const string &code, int market)
{
    string filter;
    getFilter(type, market, code, filter);
    Attach(ctx).onDispatch(QDataItem::Create(UNSUBSCRIBE_TC, filter, ctx));
}

void DispatchQuotation::UnsubscribeAll(PCtxAlgoRule ctx)
{
    string filter("");
    Attach(ctx).onDispatch(QDataItem::Create(UNSUBSCRIBE_TC, filter, ctx));
}

