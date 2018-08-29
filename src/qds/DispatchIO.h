/*
 * $Id: $
 *
 * System I/O dispatching
 *
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file DispatchIO.h System I/O dispatching */
#include "algoapi/datasource/IDispatcher.h"
#include "algoapi/datasource/IProducer.h"
#include "fifo/IQueue.h"
#include "BaseMsgDispatcher.h"

#define SUPPORT_ZMQ_ROUTER
#include "algoapi/SwordFishIntf.h"

namespace ATS {
    using namespace AlgoApi;
    using namespace AlgoApi::DataSource;
    using namespace AlgoApi::Zmq;
    
    class DispatchIO : public IDispatcher
    {
    protected:
        /**
         * MQ descriptor (internal)
         */
        class _MqDesc {
        protected:
            BaseZmqClient   *pmq;   /**< message queue client               */
            IDispatcher     *disp;  /**< dispatcher for igress              */
            IProducer       *prod;  /**< producer for egress                */

        public:
            _MqDesc(BaseZmqClient *mq, IDispatcher *dsp, IProducer *prd)
                : pmq(mq), disp(dsp), prod(prd) {
                // NOTHING
            }

            ~_MqDesc() {
                if (NULL != pmq) {
                    pmq->Stop();
                    delete pmq;                    
                }
                if (NULL != disp) {
                    // delete disp;
                }
                if (NULL != prod && disp != (void *)prod) {
                    // delete prod;
                }
            }

            TDispResult doDispatch(Object msg) {
                return (NULL == disp) ? DR_CONTINUE : disp->onDispatch(msg);
            }

            void *GetSocket() {
                return pmq->GetSocket();
            }

            int GetFd() {
                return 0;
            }

            Object doRecv() {
                return pmq->Recv();
            }

            bool hasSend() const {
                return NULL != prod;
            }

            int doSend() {
                BaseSession *bss;
                assert(NULL != prod);
                bss = static_cast<BaseSession *>(prod->getData(1));
                if (NULL != bss) {
                    // TODO, router::client_id
                    SwordFishIntf *psf = static_cast<SwordFishIntf *>(pmq);
                    psf->SendBack(bss->func_id, bss->msg_out); 
                    // Should dispose in SessionMap                       
                    // bss->Dispose();
                    return 1;
                }
                return 0;
            }
        };

    protected:
        const static size_t MAX_MQ_COUNT = 8;
        void     *m_zmqctx; // zmq_ctx
        int       n_mq;
        _MqDesc  *a_mqs[MAX_MQ_COUNT];

    public:        
        DispatchIO(void);

        virtual ~DispatchIO(void);

        // TODO: to be removed
        BaseZmqClient *AssignSfHandler(SwordFishIntf *psf, BaseMsgDispatcher *dsp);

        BaseZmqClient *AssignMqHandler(BaseZmqClient *pmq, IDispatcher *dsp);

    protected:
        /**
         * Called by worker (threads)
         * [thread]
         */
        int  DispatchInfinitely(void *ctx, bool do_egress);
 
        /**
         * Implements IDispatcher::onDispatch
         */
        virtual TDispResult onDispatch(Object desc);

        /**
         * Process _MqDesc::IProducer and send-out via egress 
         */
        int  ProcessEgress(_MqDesc *desc) {
            return desc->hasSend() ? desc->doSend() : 0;
        }

    public:
        /**
         * Process all _MqDesc::IProducer (s) and send-out via egress 
         * Called by TimeSendQueue worker (threads)
         * [thread]
         */
        int ProcessEgress() {
            int res = 0;
            for (int i = 0; i < n_mq && !g_is_exiting; i++) {
                res += ProcessEgress(a_mqs[i]);
            }
            return res;
        }

    };
}
