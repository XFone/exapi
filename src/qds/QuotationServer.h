/*
 * $Id: $
 *
 * Dispatching quotation data
 *
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file QuotationServer.h Dispatching quotation data */
#include "Base.h"

#include "algoapi/datasource/IDispatcher.h"
#include "algoapi/QuotationIntf.h"
#include "algoapi/QuotationData.h"
#include "GlobalDefine.h"
#include "fifo/IQueue.h"

#include "PcapHelper.h"

#include <string>
#include <memory>

namespace ATS {

    using namespace std;
    using namespace AlgoApi;
    using namespace AlgoApi::DataSource;

    /** Queue sieze for quotation data */
    const size_t QSIZE_QD  = 128;   /**< default queue size */
    const size_t QDATA_LEN = 512;   /**< default qdata buffer size */

    /**
     * Quotation data item
     */
    struct QDataItem {
        size_t                  length;
        struct timeval          timestamp;
        std::unique_ptr<u_char> bigdata;            /**< save for big packet    */
        u_char                  data[QDATA_LEN];    /**< save for normal packet */
    };

    class QuotationServerIntf: public QuotationIntf
    {
    public:
        /**
         * Default constructor
         */ 
        QuotationServerIntf() : QuotationIntf() {}

        /**
         * override QuotationIntf::Send(Object)
         */
        virtual void Send(Object msg) {
            QDataItem *item = reinterpret_cast<QDataItem *>(msg);
            zmq_msg_t req;
            size_t len = item->length;

            zmq_msg_init_size(&req, len);       // TODO: use my allocator
            char *ppkt = (char *)zmq_msg_data(&req);
            u_char *pdat = item->bigdata.get(); // bigdata/data
            memcpy(ppkt, (NULL != pdat) ? pdat : item->data, len);

            int ret = zmq_msg_send(&req, m_zmqSocket, 0);
            if (-1 == ret) {
                LOGFILE(LOG_ERROR, "ZMQ send fail: %s", zmq_strerror(errno));
            } else {
                TRACE_THREAD(8, "<< PUB << (%d bytes)", ret);
            }
            zmq_msg_close(&req);
        }
    };

    /**
     * Dispatching quotation data
     * NOTE: QuotationServer may dispatch in dedicate mode without DispatchIO
     */
    class QuotationServer : public IDispatcher
    {
    public:
        static void handler_payload(u_char *buf, const struct pcap_pkthdr *header, const u_char *data);

    private:
        static Object m_parser(const QuotationType, const char *, const char *, int);
        

    protected:
        QuotationIntf     &m_intf;
        IQueue            &m_queue;
        long               m_overload;      /**< ififo statistics       */
        long               m_hits;          /**< subscription hit count */
        struct timeval     m_play_ts;       /**< current timestamp of replaying packet */

        static QuotationServer &Attach(void *pnode);

        /**
         * Implements IDispatcher::onDispatch
         * @param obj PbSession
         */
        virtual TDispResult onDispatch(Object obj);

        /**
         * Call by worker (threads)
         */
        int ProcessInfinitely();

    public:
        /**
         * Default constructor
         */
        QuotationServer(QuotationIntf &intf, IQueue &queue);

        /**
         * Create a new quotation data item and dispatch it
         */
        static void NewItem(struct timeval ts, size_t len, const u_char *data);

        /**
         * Printout statitics
         */
        static ostream &ReportStats(ostream &);
    };

} // namespace ATS