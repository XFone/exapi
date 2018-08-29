/*
 * $Id: $
 *
 * Key and uuid utils
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file KeyUtils.h Key and uuid utilities */
#include "Base.h"

// base-58 for key encode/decode
#include "Base58.h"

#include <string.h>
#include <uuid/uuid.h>

namespace AlgoApi {

    using namespace std;

    class KeyUtils {
    public:
        /**
         * remove char '@c' from string s
         */
        static void squeeze(char *s, int c) {
            /* register */ char *p0, *p;
            for (p0 = s, p = s; *p != '\0'; p++) {
                char ch = *p;
                if ((int)ch != c) {
                    *p0++ = ch;
                }
            }
            *p0 = '\0';
        }

        /**
         * Get string from uuid
         * @param uuid uuid in binary format
         * @param uuid_str uuid in string, it is a 32 bytes hex-string without 
         *        '-' plus the trailing '\0' 
         * @return hex-string char buffer
         */
        static const char *uuid2string(const uuid_t &uuid, OUT string &uuid_str) {
            char buf[40];
            // buf "%08x-%04x-%04x-%04x-%012x", 36 bytes plus the trailing '\0'
            uuid_unparse_lower(uuid, buf);
            //cerr << " ----------- " << buf << " -----------" << endl;
            squeeze(buf, '-');
            uuid_str = buf;
            return uuid_str.c_str();
        }

        /**
         * Get uuid from string
         * @param uuid_str output via uuid2string
         * @param uuid uuid in binary format
         * @return 0 for success, otherwise -1 is returned
         */
        static int string2uuid(const string &uuid_str, OUT uuid_t &uuid) {
            char *p, buf[40];
            const char *pp = uuid_str.c_str();

            memset(buf, 0, sizeof(buf));
            buf[8] = buf[13] = buf[18] = buf[23] = '-';
            p = &buf[0];
            while (*pp != '\0') {
                if (*p == '-') p++;
                *p++ = *pp++; 
            }

            // buf "%08x-%04x-%04x-%04x-%012x", 36 bytes plus the trailing '\0'
            //cerr << " ----------- " << buf << " -----------" << endl;
            return uuid_parse(buf, uuid);
        }

        /**
         * Get base-58 encoded string from uuid
         * @param uuid uuid in binary format
         * @param base58_str uuid in string, encoded with base-58 
         * @return base58 char buffer
         */
        static const char *uuid2base58(const uuid_t &uuid, OUT string &base58_str) {
            const unsigned char *pb = reinterpret_cast<const unsigned char *>(&uuid);
            base58_str = EncodeBase58(pb, pb + sizeof(uuid));
            return base58_str.c_str();
        }


        /**
         * Get uuid from base58 string
         * @param base58_str output via uuid2base58
         * @param uuid uuid in binary format
         * @return 0 for success, otherwise -1 is returned
         */
        static int base582uuid(const string &base58_str, OUT uuid_t &uuid) {
            std::vector<unsigned char> sv;
            if (!DecodeBase58(base58_str, sv)) {
                return -1;
            }
            unsigned char *pa = reinterpret_cast<unsigned char *>(&uuid);
            unsigned char *pe = pa + sizeof(uuid);
            for (unsigned char uc : sv) {
                *pa = uc;
                if (++pa >= pe) break;
            }
            return 0;
        }

    };
}

