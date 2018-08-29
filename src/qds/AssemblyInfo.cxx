/*
 * $Id: $
 *
 * Assembly Information for deploy workers and tasks
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
/** @file AssemblyInfo.cxx Program task/worker assembler */

#include "Base.h"
#include "Log.h"
#include "Trace.h"
#include "TimeType.h"

#include "algoapi/DataServiceRedis.h"
#include "algoapi/QuotationIntf.h"
#include "worker/WorkerUtils.h"
#include "worker/WorkerThread.h"
#include "fifo/FifoConcurrent.h"
//#include "fifo/FifoLockfree.h"
#include "fifo/FifoBlocking.h"
#include "timer/TimerEvent.h"
#include "timer/TimerReal.h"
#include "pool/MemStats.h"

#include "GlobalDefine.h"
#include "DispatchIO.h"
#include "DispatchQuotation.h"
#include "TimeSendQueue.h"
#include "QuotationServer.h"

#include <thread>

using namespace ATS;

#define MAX_WAIT             1000

/* declared in main.cxx */
extern vector<string> qds_address_cfgs;
extern vector<string> qd_address_cfgs;
extern vector<string> redis_addr_cfgs;

extern string qd_file_name;
extern string pcap_file_name;

namespace QDS_MAIN {

    /** Worker for DispatchIO
     */
    class WorkerDispatchIO : public DispatchIO, public WorkerThread {
    private:
        static void *io_task(void *ctx) {
            WorkerDispatchIO *self = static_cast<WorkerDispatchIO *>(ctx);
            ATS::thread_t tid = WorkerThread::GetId();
            LOGFILE(LOG_DEBUG, "[0x%x] DispatchIO starting...", tid);
            self->DispatchInfinitely(ctx, true);  // egress handled
            LOGFILE(LOG_DEBUG, "[0x%x] DispatchIO exited.", tid);
            return ctx;
        }

    public:
        WorkerDispatchIO() {}

        void StartDispatch() {
            WorkerThread::Run(io_task, this);       
        }

        void StopDispatch() {
            WorkerThread::WaitStop(MAX_WAIT);
        }

        virtual ~WorkerDispatchIO() {
            if (WorkerThread::WaitStop(MAX_WAIT) < 0) {
                WorkerThread::Kill();
            }
        }
    };

    /** Worker for quotation or datafeed - 行情
     */
    class WorkerQuotation : public DispatchQuotation, public WorkerThread {
    private:
        static void *quotation_task(void *self) {
            ATS::thread_t tid = WorkerThread::GetId();
            LOGFILE(LOG_DEBUG, "[0x%x] QuotationClient starting...", tid);
            static_cast<WorkerQuotation *>(self)->ProcessInfinitely(true);
            LOGFILE(LOG_DEBUG, "[0x%x] QuotationClient exited.", tid);
            return self;
        }

    public:
        WorkerQuotation() : 
            DispatchQuotation(*(new QuotationIntf()), *(new FifoBlocking(QSIZE_QD))) { // FifoConcurrent(QSIZE_QD)
            WorkerThread::Run(quotation_task, this);
        }

        QuotationIntf *GetQuotationIntf() const {
            return &m_intf;
        }

        virtual ~WorkerQuotation() {
            UnsubscribeAll(NULL); // stop subscribe
            if (WorkerThread::WaitStop(MAX_WAIT) < 0) {
                WorkerThread::Kill();
            }
        }
    };

    /** Worker for quotation server
     */
    class WorkerQdataServer : public QuotationServer, public WorkerThread {
    private:
        static void *quotation_task(void *self) {
            ATS::thread_t tid = WorkerThread::GetId();
            LOGFILE(LOG_DEBUG, "[0x%x] QuotationServer starting...", tid);
            static_cast<WorkerQdataServer *>(self)->ProcessInfinitely();
            LOGFILE(LOG_DEBUG, "[0x%x] QuotationServer exited.", tid);
            return self;
        }

    public:
        WorkerQdataServer() : 
            QuotationServer(*(new QuotationServerIntf()), *(new FifoBlocking(QSIZE_QD))) { // FifoConcurrent(QSIZE_QD)
            WorkerThread::Run(quotation_task, this);
        }

        QuotationIntf *GetQuotationIntf() const {
            return &m_intf;
        }

        virtual ~WorkerQdataServer() {
            if (WorkerThread::WaitStop(MAX_WAIT) < 0) {
                WorkerThread::Kill();
            }
        }
    };
    
    /** Worker for processing pcap file
     */
    class WorkerPcapFile : public WorkerThread {
    private:
        static struct timeval m_load_ts;       /**< current timestamp of loaded packet */
        static u_long m_count;

        static void handler_payload(u_char *buf, const struct pcap_pkthdr *hdr, const u_char *data) {
            // TRACE_THREAD(8, "---- handler_payload(%d)", header->len);
            m_count++;

            if (!hdr->len) {
                char buf1[64];
                LOGFILE(LOG_INFO, "Loaded %d packets, last timestamp is %s", m_count, 
                        strtimeval_s(buf1, sizeof(buf1), hdr->ts));
                return;
            }

            if (m_load_ts.tv_sec /* not the first loading packet */ &&
                compare_time(&m_load_ts, (TPTimeInterval)&hdr->ts) > 0) { 
                char buf1[64], buf2[64];
                LOGFILE(LOG_WARN, "Packet timestamp(%s) is less than previous timestamp (%s)", 
                        strtimeval_s(buf1, sizeof(buf1), hdr->ts),
                        strtimeval_s(buf2, sizeof(buf2), m_load_ts));
            }
            m_load_ts = hdr->ts;
            QuotationServer::NewItem(hdr->ts, hdr->len, data);
        }

        static void *pcap_task(void *self) {
            const char *fname = pcap_file_name.c_str();
            assert(NULL != fname);
            LOGFILE(LOG_INFO, "Loading quotation payload from file %s...", fname);
            load_pcap_file(fname, handler_payload);
            return self;
        }

    public:
        WorkerPcapFile() {
            WorkerThread::Run(pcap_task, this);
        }

        virtual ~WorkerPcapFile() {
            if (WorkerThread::WaitStop(MAX_WAIT) < 0) {
                WorkerThread::Kill();
            }
        }
    };

    /** Worker for TimerReal
     */
    class WorkerRealTimer : public TimerReal, public WorkerThread {
    private:
        static void *timer_task(void *ctx) {
            ATS::thread_t tid = WorkerThread::GetId();
            LOGFILE(LOG_DEBUG, "[0x%x] TimerReal starting...", tid);
            static_cast<WorkerRealTimer *>(ctx)->ProcessTimers();
            LOGFILE(LOG_DEBUG, "[0x%x] TimerReal exited.", tid);
            return ctx;
        }

    public:
        WorkerRealTimer() {
            WorkerThread::Run(timer_task, this);
        }

        virtual ~WorkerRealTimer() {
            if (WorkerThread::WaitStop(MAX_WAIT) < 0) {
                WorkerThread::Kill();
            }
        }
    };

} // namespace


using namespace QDS_MAIN;

struct timeval WorkerPcapFile::m_load_ts = { 0L, 0L };
u_long WorkerPcapFile::m_count = 0L;

WorkerDispatchIO  *_worker_io;
WorkerQuotation   *_worker_quotation;
WorkerQdataServer *_worker_qdserver;
WorkerPcapFile    *_worker_pcapfile = NULL;
WorkerRealTimer   *_worker_timer;

void ATS::Worker::BeginSchedule()
{
    // db-redis
    if (!redis_addr_cfgs.size()) {
        LOGFILE(LOG_WARN, "Redis server was not set, using local");
        redis_addr_cfgs.push_back("localhost:6379");
    }
    (void)DataServiceRedis::Instance(&redis_addr_cfgs[0]);

    // Total 6 threads: 4(worker) + 2 * 1(zmq_ctx + sockt)
    _worker_io        = new WorkerDispatchIO();
    _worker_timer     = new WorkerRealTimer();
    _worker_quotation = new WorkerQuotation();
    _worker_qdserver  = new WorkerQdataServer();

    // Quotation (remote)
    if (!qd_address_cfgs.size()) {
        LOGFILE(LOG_WARN, "QD address was not set, using default");
        qd_address_cfgs.push_back("tcp://localhost:8608");
        //qd_address_cfgs.push_back("tcp://121.199.20.201:8100");
    }
    qd_address_cfgs.push_back(""); // MUST end with empty string
    if (!qd_file_name.empty()) {
        const char *fname = qd_file_name.c_str();
        FILE *qd_file = fopen(fname, "a+");
        if (NULL == qd_file) {
            LOGFILE(LOG_WARN, "failed to open file '%s': %s", fname, strerror(errno));
        } else {
            LOGFILE(LOG_INFO, "save to quotation file '%s'", fname);
            _worker_quotation->SetRecordFile(qd_file);
        }
    }
    _worker_io->AssignMqHandler(_worker_quotation->GetQuotationIntf(), _worker_quotation)
              ->Start(&qd_address_cfgs[0], ZMQ_SUB);   // start a list of addr

    // Quotation (server)
    if (!qds_address_cfgs.size()) {
        LOGFILE(LOG_WARN, "QDS address was not set, using default");
        qds_address_cfgs.push_back("tcp://*:8100");
    }
    qds_address_cfgs.push_back(""); // MUST end with empty string
    _worker_io->AssignMqHandler(_worker_qdserver->GetQuotationIntf(), _worker_qdserver)
              ->Start(&qds_address_cfgs[0], ZMQ_PUB);   // start a list of addr

    // dispatching i/o
    _worker_io->StartDispatch();

    // loading packets from pcap file
    if (!pcap_file_name.empty()) {
        _worker_pcapfile = new WorkerPcapFile();
    }

    // print all memory usage
    print_all_usage();
}

void ATS::Worker::ReportHeartbeat()
{
    // TODO: do heartbeat (every second) to keep-alive or zookeeper server
    // Zookeep::Heartbeat();
    sleep(1);

    TRACE_THREAD(7, "-- Heartbeat --");
    TRACE_IF(7, {
        stringstream ss;
        DispatchQuotation::ReportStats(ss);
        cout << ss.str() << endl;
    });
}

void ATS::Worker::EndSchedule()
{
    _worker_io->StopDispatch();

    if (NULL != _worker_pcapfile) delete _worker_pcapfile;

    delete _worker_quotation;
    delete _worker_qdserver;
    delete _worker_timer;

    // print all memory usage
    print_all_usage();
}
