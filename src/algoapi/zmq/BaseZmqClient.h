/*
 * $Id: $
 *
 * 0MQ Base Client API
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file BaseZmqClient.h Base ZMQ Client */

#include "Base.h"
#include "Log.h"

#include "../datasource/IMsgQueueClient.h"
#include "ZmqHelper.h"

#include <string>

namespace AlgoApi {
    namespace Zmq {

        using namespace std;
        using namespace DataSource;

        /** MAX bind / connect for single zmq_socket */
        const size_t MAX_ZMQ_ENDPOINT  = 8;
        const size_t MAX_ZMQ_IDENTITY  = 16;

        /**
         * BaseZmqClient implements IMsgQueueClient
         */
        class BaseZmqClient : public IMsgQueueClient {
        protected:
            bool   b_router;
            void  *m_zmqContext;
            void  *m_zmqSocket;

        public:
            BaseZmqClient(void *ctx = NULL) : 
                b_router(false), m_zmqContext(ctx), m_zmqSocket(NULL) {
                // NOTHING
            }

            virtual ~BaseZmqClient() {
                Stop();
            }

            /**
             * Start with an URL address (e.g., "tcp://10.0.1.2:8601" or "tcp:// *:8601")
             * @param url 0MQ address to bind or connect
             * @param zmqtype 0MQ socket type (DEALER, ROUTER, PUB/SUB, ...)
             */
            void Start(const string &url, int zmqtype)
            {
                int  ret;
                bool do_bind = false;

                if (NULL == m_zmqSocket) {
                    if (NULL == (m_zmqSocket = zmq_socket(m_zmqContext, zmqtype))) {
                        LOGFILE(LOG_ERROR, "ZMQ create socket: %s", zmq_strerror(errno));
                        return;
                    }
                }

#if 0            
                int timeout = 3000; // 3s
                zmq_setsockopt(m_zmqSocket, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
#endif

                if (strchr(url.c_str(), '*') != NULL) {
                    do_bind = true;
                } else if (b_router && strstr(url.c_str(), "ipc:") != NULL) {
                    do_bind = true;
                }

                if (do_bind) {
                    ret = zmq_bind(m_zmqSocket, url.c_str());
                    if (ret) {
                        LOGFILE(LOG_ERROR, "ZMQ bind failed (%d): %s", ret, zmq_strerror(errno));
                    } else {
                        LOGFILE(LOG_INFO, "ZMQ 0x%x bind-ed to %s", m_zmqSocket, url.c_str());
                    }                
                } else {
                    ret = zmq_connect(m_zmqSocket, url.c_str());
                    if (ret) {
                        LOGFILE(LOG_ERROR, "ZMQ connect failed (%d): %s", ret, zmq_strerror(errno));
                    } else {
                        LOGFILE(LOG_INFO, "ZMQ 0x%x connect to %s", m_zmqSocket, url.c_str());
                    }                
                }
            }

            /**
             * Start a series of address
             */
            void Start(const string addrs[], int zmqtype) {
                for (int i = 0; i < (int)MAX_ZMQ_ENDPOINT; i++) {
                    const string &addr = addrs[i];
                    if (addr.empty()) break;
                    this->Start(addr, zmqtype);
                }
            }

            void Stop() {
                if (NULL != m_zmqSocket) {
                    zmq_close(m_zmqSocket);
                    LOGFILE(LOG_DEBUG, "ZMQ socket 0x%x closed", m_zmqSocket);
                    m_zmqSocket = NULL;
                }
            }

            void SetContext(void *ctx) {
                m_zmqContext = ctx;
            }

            void *GetContext() const {
                return m_zmqContext;
            }

            void *GetSocket() const {
                return m_zmqSocket;            
            }

            /**
             * Inherited IMsgQueueClient::Send()
             */
            virtual void Send(Object msg) {
                //
            }

            /**
             * Inherited IMsgQueueClient::Recv()
             */
            virtual Object Recv() {
                return NULL;
            }
        };
    }
}
