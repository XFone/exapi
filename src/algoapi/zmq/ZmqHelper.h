/*
 * $Id: $
 *
 * ZeroMQ Helper functions
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#ifndef _ALGOAPI_ZMQ_HELPER_H_
#define _ALGOAPI_ZMQ_HELPER_H_

#include "Base.h"
#include "Trace.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "zmq.h"

#define MAX_SF_PACKET_HEADER    16
#define MAX_SF_CODE             16
#define SF_PACKET_SEP           '\t'

BEGIN_EXTERN_C

    /**
     * Encode Swordfish packet in format: [msgId]\t[pb]
     * @param msg ZMQ msg instance
     * @param msgId message Id as packet header
     * @param plen packet length
     * @return pointer to packet body
     */
    INLINE char *EncodeSfPacket(zmq_msg_t *msg, int msgId, size_t plen) {
        char header[MAX_SF_PACKET_HEADER];
        char *ppkt;
        size_t hlen = snprintf(header, sizeof(header), "%d%c", msgId, SF_PACKET_SEP);        
        zmq_msg_init_size(msg, hlen + plen); // TODO: use my allocator
        ppkt = (char *)zmq_msg_data(msg);
        memcpy(ppkt, header, hlen);
        ppkt += hlen;
        return ppkt;
    }

    /**
     * Decode Swordfish packet in format: [msgId]\t[pb]
     * @param msg ZMQ msg instance
     * @param msgId message Id
     * @return offet to packet body
     */
    INLINE size_t DecodeSfPacket(zmq_msg_t *msg, OUT int *msgId = NULL) {
        char *ibuf, *pt;

        assert(msg != NULL);
        ibuf = (char *)zmq_msg_data(msg);
        if (NULL == (pt = strchr(ibuf, SF_PACKET_SEP))) {
            return 0;
        }
        if (pt > ibuf && NULL != msgId) {
            *msgId = strtol(ibuf, NULL, 10);
        }

        return (pt - ibuf) + 1;
    }

    /**
     * Encode Swordfish Quotation packet in format: [type-char]\t[code]\t[pb]
     * @param msg ZMQ msg instance
     * @param pc prefix char
     * @param code stock, future, or option code
     * @param plen packet length
     * @return pointer to packet body
     */
    INLINE char *EncodeQuotationPacket(zmq_msg_t *msg, const char pc, const char *code, size_t plen) {
        char header[MAX_SF_PACKET_HEADER];
        char *ppkt;
        size_t hlen = snprintf(header, sizeof(header), "%c%c%s%c", 
                               pc, SF_PACKET_SEP, code, SF_PACKET_SEP);
        zmq_msg_init_size(msg, hlen + plen); // TODO: use my allocator
        ppkt = (char *)zmq_msg_data(msg);
        memcpy(ppkt, header, hlen);
        ppkt += hlen;
        return ppkt;
    }

    /**
     * Decode Swordfish Quotation packet in format: [type-char]\t[code]\t[pb]
     * @param msg ZMQ msg instance
     * @param pc prefix char
     * @param code stock, future, or option code
     * @return offet to packet body
     */
    INLINE size_t DecodeQuotationPacket(zmq_msg_t *msg, OUT char *pc, OUT char *code = NULL) {
        char *ibuf, *pt1, *pt2 = NULL;

        assert(msg != NULL);
        ibuf = (char *)zmq_msg_data(msg);

        if (NULL == (pt1 = strchr(ibuf, SF_PACKET_SEP))) {
            return 0;
        }

        if (pt1 > ibuf) {
            if (NULL != pc) {
                *pc = *ibuf;                
            }
            pt2 = strchr(++pt1, SF_PACKET_SEP);
            if (NULL != code && NULL != pt2) {
                int len = pt2 - pt1;
                memcpy(code, pt1, len);
                code[len] = '\0';
            }
        }

        TRACE_THREAD(8, "---- DecodeQuotationPacket: %c\\t%s ----", *pc, code);

        return ((pt2 != NULL) ? ((pt2 - ibuf) + 1) : 
               ((pt1 != NULL) ? ((pt1 - ibuf) + 1) : 0)
               );
    }

END_EXTERN_C

#ifdef __cplusplus

namespace AlgoApi {
    namespace Zmq {

        /**
         * Swordfish message helper via ZMQ:
         */
        struct ZmqHelper {
            /**
             * Encode Swordfish packet in format: [msgId]\t[pb]
             * @return pointer to packet body
             */
            static char *EncodeSfPacket(zmq_msg_t *msg, int msgId, size_t plen) {
                return ::EncodeSfPacket(msg, msgId, plen);     
            }

            /**
             * Decode Swordfish packet in format: [msgId]\t[pb]
             * NOTES: zmq_msg_t is char[32]
             * @return message body offset
             */
            static size_t DecodeSfPacket(zmq_msg_t *msg, int *msgId = NULL) {
                return ::DecodeSfPacket(msg, msgId);
            }
        };

    } // namespace Zmq
} // namespace AlgoApi::Zmq

#endif /* __cplusplus */

#endif /* _ALGOAPI_ZMQ_HELPER_H_ */
