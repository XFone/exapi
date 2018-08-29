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
#include "TimeType.h"

#include "pool/SessionMap.h"
#include "pool/MemoryPool.h"

#include "QuotationServer.h"
#include "PcapHelper.h"

#include <sstream>

using namespace ATS;
using namespace AlgoApi;

MemoryPool<QDataItem> l_data_pool(QSIZE_QD, sizeof(QDataItem), QSIZE_QD);

//static _THREAD_LOCAL_ QuotationServer *_instance = NULL;
static QuotationServer *_instance = NULL;

Object QuotationServer::m_parser(const QuotationType tc, const char *code, 
                                 const char *packed, int plen)
{
    // NOT parse, just alloacate a ring-buffer for containing packet
    // tc, code
    // (packed, len)

    // assert(plen <= (int)MAX_QD_PACKED);

    TRACE_THREAD(7, "---- m_parser('%c' %s, %d bytes)", tc, code, plen);

    return NULL;
}

ostream &QuotationServer::ReportStats(ostream &os)
{
    os << "\nQD ififo overloads: " << _instance->m_overload 
       << "\nQD subscribed hits: " << _instance->m_hits
       << endl;
    return os;
}

QuotationServer &QuotationServer::Attach(void *pnode)
{
    assert(NULL != _instance);
    return *_instance;
}

void QuotationServer::NewItem(struct timeval ts, size_t len, const u_char *data)
{
    QDataItem *item;
    char buf[64];

    TRACE_THREAD(8, "NewItem: {ts:%s, len:%d}", strtimeval_s(buf, sizeof(buf), ts), len);

    while (NULL == (item = l_data_pool.allocate(1))) {
        LOGFILE(LOG_WARN, "l_data_pool is busy");
        usleep(1);
    }

    assert(NULL != item);

    item->length = len;
    item->timestamp = ts;
    if (len >= QDATA_LEN) {
        u_char *buf = new u_char(len);
        std::unique_ptr<u_char> ptr(buf);
        item->bigdata = std::move(ptr);
        memcpy(buf, data, len);
        TRACE_THREAD(7, "NewItem big size=%d", len);
    } else {
        memcpy(&item->data[0], data, len);        
    }

    assert(NULL != _instance);

    _instance->onDispatch(item);
}

TDispResult QuotationServer::onDispatch(Object obj)
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

int QuotationServer::ProcessInfinitely()
{
    do {
        QDataItem *item = NULL;
        int ret;

        if ((ret = m_queue.pop_front<QDataItem *>(&item)) < 0) {
            // TRACE_THREAD(7, "--- %d ---", ret);
        }

        if (NULL != item) {
            if (m_play_ts.tv_sec) { // previous timestamp as offset
                struct timeval ts = item->timestamp;
                dec_time(&ts, &m_play_ts);
                // TODO: use TimerReal::StartOnce(&tv, (int tag, POwner)=> {}, this);
                if (ts.tv_usec >= 0) usleep(ts.tv_usec);
                if (ts.tv_sec >= 0) sleep(ts.tv_sec);
            }
            m_play_ts = item->timestamp;

            // trace for debug
            TRACE_THREAD(8, "-- Pulishing --");
            TRACE_IF(8, {
               char buf[64];
               cout << "🗞  quotation item:{ ts:" 
                    << strtimeval_s(buf, sizeof(buf), item->timestamp)
                    << ", len:" << item->length << " }" 
                    << endl;
            });
 
            // publishing item
            m_intf.Send(item);

            // release buffer resource
            item->bigdata.release();    // if allocate
            l_data_pool.deallocate(item); 
        }

    } while (!g_is_exiting);

    return 0;
}

QuotationServer::QuotationServer(QuotationIntf &intf, IQueue &queue) : 
            m_intf(intf), m_queue(queue), m_overload(0), m_hits(0) 
{
    _instance = this; // TODO (multiple instances)

    //gettimeofday(&m_play_ts, NULL);
    memset(&m_play_ts, 0, sizeof(m_play_ts));

    // use my parser to allocate message buffer
    m_intf.SetParser(m_parser);
}

