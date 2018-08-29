/*
 * $Id: $
 *
 * SwordFish (0MQ + protobuf) Quotation Interface API
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file QuotationIntf.h Quotation-Interface API */

#include "Base.h"
#include "DateTime.h"
#include "Log.h"
#include "Trace.h"

#include "datasource/IMsgQueueClient.h"
#include "protos/Enum2Message.h"
#include "protos/quotation.pb.h"
#include "zmq/BaseZmqClient.h"
#include "QuotationData.h"

namespace AlgoApi
{
    using namespace std;
    using namespace DataSource;
    using namespace Protos;
    using namespace Zmq;
    using namespace quotation;

    /**
     * Swordfish Quotation message
     * subscribe:   [prefix]\t[code]\t[pb]
     * subscribe:   [prefix]\t[market]\t[code]\t[pb]
     * where [prefix] is (see quotation.proto):
     *    'S'     Stock
     *    'F'     Future
     *    'O'     Option
     *    ...
     */
    class QuotationIntf : public BaseZmqClient
    {
    public:
        typedef Object (*PQuotationParser)(const QuotationType, const char *, const char *, int);
        typedef void (*PRecvCallback)(char tc, const char *code, const char *, size_t);

    private:
        PQuotationParser m_parser;
        PRecvCallback    m_recv_cb;

    public:
        QuotationIntf(void *ctx = NULL) : 
            BaseZmqClient(ctx), m_parser(NULL), m_recv_cb(NULL) {
            // use default parser
            SetParser((PQuotationParser)(&ParseQuotation));
        }

        virtual ~QuotationIntf() {
            // BaseZmqClient::Stop()
        }

        void SetParser(PQuotationParser parser) {
            m_parser = parser;
        }

        void SetRecvCallback(PRecvCallback cb) {
            m_recv_cb = cb;
        }

        /**
         * Receive and decode as an object
         */
        Object RecvObject(zmq_msg_t &msg, size_t data_len) {
            char tc;
            char code[MAX_SF_CODE];
            size_t body_off = DecodeQuotationPacket(&msg, &tc, code);
            char *data = (char *)zmq_msg_data(&msg);
            char *body = data + body_off;

            TRACE_THREAD(8, ">> %c%s >> (%d bytes)", tc, code, data_len);
            if (NULL != m_recv_cb) {
                (*m_recv_cb)(tc, code, data, data_len);
            }
            return (*m_parser)((QuotationType)tc, code, body, data_len - body_off);
        }

        /**
         * Inherited IMsgQueueClient::Send()
         */
        virtual void Send(Object msg) {
            // NOT IMPLEMENTED
            assert(0);
        }

        /**
         * Inherited IMsgQueueClient::Recv()
         */
        virtual Object Recv() {
            Object obj = NULL;
            try {
                zmq_msg_t msg;
                zmq_msg_init(&msg);
                int len = zmq_msg_recv(&msg, m_zmqSocket, 0);

                if (-1 == len) {
                    LOGFILE(LOG_WARN, "ZMQ recv failed: %s ", zmq_strerror(errno));
                } else if (len > 0) {
                    obj = RecvObject(msg, len);
                }
                zmq_msg_close(&msg); 
            } catch (exception ex) {
                LOGFILE(LOG_ERROR, "Recv %s", ex.what());
            }

            return obj;
        }

        void Subscribe(string &filter) {
            LOGFILE(LOG_INFO, "Subscribing with filter '%s'", filter.c_str());
            zmq_setsockopt(m_zmqSocket, ZMQ_SUBSCRIBE, filter.c_str(), filter.length());
        }

        void Unsubscribe(string &filter) {
            LOGFILE(LOG_INFO, "Unsubscribing with filter '%s'", filter.c_str());
            zmq_setsockopt(m_zmqSocket, ZMQ_UNSUBSCRIBE, filter.c_str(), filter.length());
        }
        
    public:
        static PMessage NewQuotation(const QuotationType tc, const char *code = NULL) {
            // Stock
            // Future
            // Option
            // Index
            // VIndex
            // GradedFundIopv
            // OptionVolatility
            // IndexParam
            // SecurityOrderInfo
            // PublicOrderData (level 2 逐笔委托)
            // PublicOrderQueueData (level 2 委托队列)
            // PublicOrderTransaction (level 2 逐笔成交)
            // CurrentBasis (当前盘口位的基差)
            // CircleQuotation (周期行情数据)
            PMessage msg = NULL;
            switch (tc) {
            case QT_STOCK:
                msg = new quotation::Stock();
                if (NULL != code) static_cast<PStockData>(msg)->set_code(code);
                break;

            case QT_FUTURE:
                msg = new quotation::Future();
                break;

            case QT_OPTION:
                msg = new quotation::Option();
                break;

            default:
                TRACE_THREAD(8, "NewQuotation ignored %c", tc);
                break;
            }
            return msg;
        }

        static inline PMessage ParseQuotation(const QuotationType tc, const char *code, 
                                              const char *packed, int plen) {
            PMessage msg = NewQuotation(tc, code);
            return (msg == NULL) ? NULL : DecodeProto(packed, plen, msg);
        }

        static void DisposeQuotation(PMessage msg) {
            // allocated in parseQuoation
            // TODO: use smart-ptr or msg pool
            if (NULL != msg) delete msg;
        }
    };
}
