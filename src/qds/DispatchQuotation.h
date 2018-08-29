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

/** @file DispatchQuotation.h Dispatching quotation data */
#include "Base.h"

#include "algoapi/datasource/IDispatcher.h"
#include "algoapi/QuotationIntf.h"
#include "algoapi/QuotationData.h"
#include "GlobalDefine.h"
#include "fifo/IQueue.h"
#include "pool/TCodePool.h"

#include <string>

namespace ATS {

    using namespace std;
    using namespace AlgoApi;
    using namespace AlgoApi::DataSource;

    /**
     * Dispatching quotation data
     * NOTE: DispatchQuotation may dispatch in dedicate mode without DispatchIO
     */
    class DispatchQuotation : public IDispatcher
    {
    private:
        static Object m_parser(const QuotationType, const char *, const char *, int);
        static void   m_recv_callback(char tc, const char *, const char *, size_t);

    protected:
        // following are stats between heartbeats
        TCodePool          m_code_stats;

        // following are stats for summary
        long               m_overload;      /**< ififo total statistics       */
        long               m_hits;          /**< subscription total hit count */

        // members
        bool               m_sync_redis;
        FILE              *m_record_file;
        QuotationIntf     &m_intf;
        IQueue            &m_queue;
        
        static DispatchQuotation &Attach(PCtxAlgoRule algo_ctx);

        /**
         * Implements IDispatcher::onDispatch
         * @param obj PbSession
         */
        virtual TDispResult onDispatch(Object obj);

        /**
         * Call by worker (threads)
         */
        int ProcessInfinitely(bool monitor_event = false);

    public:
        /**
         * Default constructor
         */
        DispatchQuotation(QuotationIntf &intf, IQueue &queue) : 
            m_overload(0), m_hits(0), m_sync_redis(true),
            m_record_file(NULL), m_intf(intf), m_queue(queue) {
            // use my parser to allocate message buffer
            m_intf.SetParser(m_parser);
        }

        void SetRecordFile(FILE *file) {
            m_record_file = file;
            m_intf.SetRecvCallback(m_recv_callback);
        }

        void SetSyncRedis(bool enable) {
            m_sync_redis = enable;
        }

        bool GetSyncRedis() const {
            return m_sync_redis;
        }

        /**
         * Printout statitics
         */
        static ostream &ReportStats(ostream &);

        /**
         * Get quotation data from cache or loading from backend redis
         */
        static QuotationData &GetData(const string &code, int market = -1);

        /**
         * Subscribe a quatation code
         * [thread-safe]
         */
        static void Subscribe(PCtxAlgoRule ctx, const QuotationType type, 
                              const string &code, int market = -1);

        static void SubscribeAll(PCtxAlgoRule ctx, const QuotationType type);

        static void SubscribeStock(PCtxAlgoRule ctx, const string &code, int market = -1) {
            Subscribe(ctx, QT_STOCK, code, market);
        }

        static void SubscribeDefault(PCtxAlgoRule ctx = NULL);

        /**
         * Unsubscribe a quatation code
         * [thread-safe]
         */
        static void Unsubscribe(PCtxAlgoRule ctx, const QuotationType type,
                                const string &code, int market = -1);

        static void UnsubscribeAll(PCtxAlgoRule ctx = NULL);
        
        static void UnsubscribeStock(PCtxAlgoRule ctx, const string &code, int market = -1) {
            Unsubscribe(ctx, QT_STOCK, code, market);
        }
    };

} // namespace ATS