/*
 * $Id: $
 *
 * Base class for messages dispatcher
 *
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file BaseMsgDispatcher.h Base class for messages dispatcher */
#include "Base.h"
#include "Log.h"

#include "algoapi/datasource/IDispatcher.h"
#include "algoapi/datasource/IProducer.h"
#include "algoapi/protos/Enum2Message.h"

#include "fifo/IQueue.h"

namespace ATS {

    using namespace AlgoApi::DataSource;
    using namespace AlgoApi::Protos;

    /**
     * Basement session
     */
    class BaseSession {
    public:
        FuncId   func_id;      /**< Function Id (Swordfish)     */
        PMessage msg_in;       /**< Resquest or Command         */
        PMessage msg_out;      /**< Response or Acknowledgement */

    protected:

        BaseSession()
          : func_id(ID_REQUEST_OK), msg_in(NULL), msg_out(NULL) {}

        BaseSession(FuncId fid, PMessage imsg, PMessage omsg)
          : func_id(fid), msg_in(imsg), msg_out(omsg) {}
          
    public:
        // TODO: review this virtual destroy, to be removed
        virtual void Dispose() {};
    };

    /**
     * Basement dispatcher
     */
    class BaseMsgDispatcher : public IDispatcher, public IProducer
    {
    protected:
        size_t   n_fifo;
        IQueue **a_fifo;        /**< array of queues for dispatching      */
        IQueue  *egress;        /**< queue for message/response egressing */

    protected:
        BaseMsgDispatcher() : n_fifo(0), a_fifo(NULL), egress(NULL) {}

    public:
        /**
         * Get a parser callback function for message decoder
         */
        virtual PUserParseFunc GetMessageParser() = 0;

    public:
 
        /**
         * Implements IProducer::getData
         * <= this.egress
         * Read message from egress queue
         * @return BaseSession *
         */
        virtual Object getData(int max = -1) {
            Object data = NULL;
            int ret;
            if (!egress->empty() && (ret = egress->pop_front(&data)) < 0) {
                if (ret != -EAGAIN) {
                    LOGFILE(LOG_WARN, "getData returns %d", ret);
                }
            }
            return (BaseSession *)data;
        }

        /**
         * Implements IDispatcher::onDispatch
         * => this.a_fifo[]
         * Dispatch and save message into a_fifo[queue_id]
         */
        virtual TDispResult onDispatch(Object obj) {
            return DR_IGNORED;
        }

        /**
         * Add a new FIFO queue as a group of FSMs' ingress
         * NOT thread-safe
         */
        void AddQueue(IQueue *queue) {
            // TODO: a group of queues for load-balance
            IQueue **cur = new IQueue*[n_fifo + 1];
            if (a_fifo != NULL) {
                memcpy(cur, a_fifo, n_fifo * sizeof(IQueue *));
                delete [] a_fifo;
            }
            a_fifo = cur;
            a_fifo[n_fifo++] = queue;
        }

        /**
         * Set FIFO queue as a single egress
         * NOT thread-safe
         */
        void SetEgressQueue(IQueue *queue) {
            egress = queue;
        }

        virtual ~BaseMsgDispatcher() {
            if (NULL != a_fifo) {
                delete [] a_fifo;
            }
        }
    };
}
