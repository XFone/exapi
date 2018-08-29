/*
 * $Id: $
 *
 * Protocol Buffer encoder/decoder tools
 *
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */

#ifndef _ENUM_TO_MESSAGE_H_
#define _ENUM_TO_MESSAGE_H_

#ifdef __cplusplus

#include <iostream>
#include <google/protobuf/message.h>

#include "FuncId.h"
#include "qerr_protocol.pb.h"

namespace AlgoApi {
    namespace Protos {

        using namespace std;
        using namespace google::protobuf;
        using namespace autotrade;

        typedef google::protobuf::Message *PMessage;
        typedef void *(*PUserParseFunc) (FuncId, const char *packed, size_t plen);

        template<class T>
        ostream &EncodeProto(T *msg, ostream &os) {
            msg->SerializeToOstream(os);
            os.flush();
            return os;
        }

        template<class T>
        PMessage DecodeProto(istream &is, T *msg) {
            msg->ParseFromIstream(&is);
            return msg;
        }

        template<class T>
        char *EncodeProto(T *msg, char *packed, size_t plen) {
            return msg->SerializeToArray(packed, plen) ? packed : NULL;
        }

        template<class T>
        PMessage DecodeProto(const char *packed, size_t plen, T *msg) {
            msg->ParseFromArray(packed, plen);
            // msg->ParsePartialFromArray(packed, plen);
            return msg;
        }

        /**
         * Enum2Message
         */
        class Enum2Message {
        protected:
            /** 
             * Hide constructor visibility, use NewInstance() to create new instance.
             */
            Enum2Message(const char * protos[]) {
                // NOTHING
            }

        public:
            /** 
             * Get a new instance.
             */
            static Enum2Message &NewInstance(const char * protos[])
            {
                Enum2Message *instance = new Enum2Message(protos);
                return *instance;
            }

            // ---------------------------- WORKING -----------------------------
            static PMessage _parseDefault(FuncId funcId, const char *packed, size_t plen) {
                PMessage msg = NULL;
                switch (funcId) {
                case ID_REQUEST_OK:
                    msg = DecodeProto(packed, plen, new ReqtOK());
                    break;

                case ID_REQUEST_ERR:
                    msg = DecodeProto(packed, plen, new ReqtErr());
                    break;

                default:
                    assert(0);
                    break;
                }

                return msg;
            }
        };
   } // namespace Protos
} // namespace AlgoApi::Protos

#else  /* !__cplusplus */
// xiaofeng 2015-03-14: TODO for pure C code
#endif /* __cplusplus */

#endif /* _ENUM_TO_MESSAGE_H_ */