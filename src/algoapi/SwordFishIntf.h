/*
 * $Id: $
 *
 * SwordFish (0MQ + protobuf) Interface API
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file SwordFishIntf.h SwordFish-Interface API */

#include "Base.h"
#include "Log.h"
 
#include "datasource/IMsgQueueClient.h"
#include "protos/FuncId.h"
#include "zmq/BaseZmqClient.h"

namespace AlgoApi
{
    using namespace std;
    using namespace Protos;
    using namespace Zmq;

    /**
     * Swordfish message formats：
     * request:   [msgstr]\t[pb]
     * subscribe: [msgstr]\t[data_date]\t[fund_id]\t[action]\t[pb]
     */
    class SwordFishIntf : public BaseZmqClient
    {
    private:
        char            m_peerId[MAX_ZMQ_IDENTITY];
        size_t          m_peerIdSize;
        PUserParseFunc  m_ufn;

    public:
        SwordFishIntf(void *ctx, bool is_router = false) : 
            BaseZmqClient(ctx), m_peerIdSize(0)
        {
            //m_enumTable = Enum2Message.NewInstance(protos);
            b_router = is_router;
            m_ufn = (PUserParseFunc)&Enum2Message::_parseDefault;
        }

        virtual ~SwordFishIntf() {
            // NOTHING
            // BaseZmqClient::Stop()
        }

        void GetPeerId(OUT string &id) {
            id = m_peerId;
        }

        /**
         * Inherited IMsgQueueClient::Send()
         */
        virtual void Send(Object msg) {
            assert(0); // DONT call this function
            PMessage pmsg = (PMessage)msg;
            Send(RetrieveFuncId(pmsg), pmsg);
        }

        /**
         * Inherited IMsgQueueClient::Recv()
         */
        virtual Object Recv() {
             return Recv(m_ufn);
        }

        void SetUserParseFunc(PUserParseFunc ufn) {
            m_ufn = ufn;
        }

        static FuncId RetrieveFuncId(PMessage msg) {
            int funcid;
            // use protocol buffer reflection
            const Descriptor *desc = msg->GetDescriptor();
            assert(msg->GetReflection() != NULL);
            assert(NULL != desc);
            funcid = msg->GetReflection()->GetInt32(*msg, desc->field(0));
            // funcid = msg->GetReflection()->GetInt32(*msg, desc->FindFieldByName("funcid"));
            return FuncId(funcid);
        }

        void Send(FuncId funcId, PMessage msg) {
            int plen = msg->ByteSize();
            if (plen > 0) {
                zmq_msg_t req;
                char *pb = EncodeSfPacket(&req, funcId, plen);
                if (NULL != EncodeProto(msg, pb, plen)) {
                    int ret = zmq_msg_send(&req, m_zmqSocket, 0);
                    if (-1 == ret) {
                        LOGFILE(LOG_ERROR, "ZMQ send fail: %s", zmq_strerror(errno));
                    } else {
                        TRACE_THREAD(6, "<< '%d' << (%d bytes)", funcId, ret);
                    }
                }
                zmq_msg_close(&req);
            }
        }

        /**
         * Send to a client with given identifier
         * SERVER_IS_ROUTER
         */
        void SendTo(string &clientId, FuncId funcId, PMessage msg) {
            zmq_send(m_zmqSocket, clientId.c_str(), clientId.size(), ZMQ_SNDMORE);
            this->Send(funcId, msg);
        }

        /**
         * Send to the recent active client
         */
        void SendBack(FuncId funcId, PMessage msg) {
#ifdef SUPPORT_ZMQ_ROUTER
            if (b_router) {
                zmq_send(m_zmqSocket, m_peerId, m_peerIdSize, ZMQ_SNDMORE);
            }
#endif
            this->Send(funcId, msg);            
        }

        /**
         * Recv with user parsing function
         */
        Object Recv(PUserParseFunc pf) {
            Object obj = NULL;
            
            try {
                zmq_msg_t msg;
                int ret, msgId = INVALID_FUNCID;

                zmq_msg_init(&msg);
                ret = zmq_msg_recv(&msg, m_zmqSocket, 0);

#ifdef SUPPORT_ZMQ_ROUTER
                if (b_router) {
                    int64_t more = 0;
                    size_t  more_size = sizeof(more);
                    zmq_getsockopt(m_zmqSocket, ZMQ_RCVMORE, &more, &more_size); 
                    // 1). check identities as head-part
                    size_t  msize = zmq_msg_size(&msg);
                    if (msize <= MAX_ZMQ_IDENTITY || more) {
                        TRACE_THREAD(7, "-- ZMQ header (size=%d, more=%d) --", zmq_msg_size(&msg), more);
                        memcpy(m_peerId, zmq_msg_data(&msg), msize);
                        m_peerIdSize = msize;
                        // 2). Ø as a delimiter
                        ret = zmq_msg_recv(&msg, m_zmqSocket, 0);
                        if (zmq_msg_size(&msg) == 0) {
                            // 3). msg body
                            ret = zmq_msg_recv(&msg, m_zmqSocket, 0);
                        }
                    }
                }
#endif // SUPPORT_ZMQ_ROUTER

                if (-1 == ret) {
                    LOGFILE(LOG_WARN, "ZMQ recv failed: %s ", zmq_strerror(errno));
                } else if (ret > 0) {
                    size_t body_off = DecodeSfPacket(&msg, &msgId);
                    char * pb = (char *)zmq_msg_data(&msg) + body_off;
                    TRACE_THREAD(6, ">> '%d' >> (%d bytes)", msgId, ret);
                    assert(pf != NULL);
                    obj = (*pf)((FuncId)msgId, pb, ret - body_off);
                }

                zmq_msg_close(&msg);

            } catch (exception ex) {
                LOGFILE(LOG_ERROR, "Recv %s", ex.what());
            }

            return obj;
        }
    };
}
