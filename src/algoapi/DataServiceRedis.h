/*
 * $Id: $
 *
 * Data Service (Redis) Interface API
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file DataServiceRedis.h Redis Data Service API */

#include "Base.h"
#include "Log.h"

#include "xredis/Redis.h"
#include "xredis/Exception.h"
#include "keyvalue/KeyUtils.h"
#include "keyvalue/TableKey.h"

namespace AlgoApi
{
    const char * const _POS_O = "O";
    const char * const _POS_D = "D";
    const char * const ID_LAST_POSITION    = "_ats_lp_";            /**< knock posistion (offset)      */
    const char * const ID_DISTRIBUTION     = "_ats_distribution_";  /**< node distribution map         */

    const char * const ID_STRATEGY_LIST    = "_ats_sl_";            /**< list of all strategies        */
    const char * const ID_STRATEGY_STATUS  = "_ats_sl_sta_";        /**< status of a strategy instance */

    const char * const ID_ALG_ORDER_LIST   = "_ats_ol_";            /**< list of all orders            */
    const char * const ID_ALG_ORDER_STATUS = "_ats_ol_sta_";        /**< status of an order instance   */
    const char * const ID_ALG_ORDER_KNOCK  = "_ats_ol_knk_";        /**< knock information of an order */
    const char * const ID_ALG_ORDER_MAP    = "_ats_somap_";         /**< mapping of strategy => orders */

    const char * const ID_QUOTATION_CODE   = "_ats_qd_";            /**< quotation data of a trade code*/

    class DataServiceRedis : public x::Redis {
    private:
        static DataServiceRedis *_instance;
        static string m_host;

        /** Hidden default constructor */
        DataServiceRedis(const string &host, int port) : x::Redis(host, port, 0) {
            // connect();
            LOGFILE(LOG_INFO, "Redis connected to tcp://%s:%d", host.c_str(), port);
        }

    protected:
        size_t safe_smembers(vector<string> &ret, const string &key) {
            size_t res = 0;
            try {
                res = x::Redis::smembers(ret, key);
            } catch (const x::Exception &ex) {
                LOGFILE(LOG_ERROR, "Redis::smembers(,'%s') %s", 
                        key.c_str(), ex.what());
            }
            /*
            catch (const std::exception &ex) {
                LOGFILE(LOG_ERROR, "Redis::smembers() %s", ex.what());                
            }*/
            return res;
        }

        void safe_sadd(const string &key, const string &value) {
            try {
                x::Redis::sadd(key, value);
            } catch (const x::Exception &ex) {
                LOGFILE(LOG_ERROR, "Redis::sadd('%s', '%s') %s", 
                        key.c_str(), value.c_str(), ex.what());
            }
        }

    public:
        /** 
         * Get a new Redis service instance.
         * @param hostname redis service host or ip
         * @return intance
         */
        static DataServiceRedis &Instance(const string *hostname = NULL) {
            if (NULL == _instance) {
                string host = "localhost";
                int port    = 6379;
                if (NULL != hostname) {
                    string::size_type pos = hostname->find("//");
                    string::size_type tos = hostname->rfind(":");
                    if (string::npos != pos) {
                        pos++;
                        pos++;
                        host = hostname->substr(pos, (tos == string::npos) ? tos : (tos - pos));
                    }
                    if (string::npos != tos) {
                        tos++;
                        port = atoi(hostname->substr(tos).c_str());
                    }
                }
                LOGFILE(LOG_INFO, "Redis connecting to tcp://%s:%d", host.c_str(), port);
                _instance = new DataServiceRedis(host, port);
            }
            return *_instance;
        }

        static string &GetKey(const uuid_t &uuid, OUT string &key) {
            KeyUtils::uuid2string(uuid, key);
            return key;
        }

        /**
         * Get lastest offset position (for order deals)
         * @param name order(_POS_O) or deal(_POS_D)
         * @param aid account hash code
         * @param pos [output] position
         */
        void GetLastPosition(const char *name, const string &aid, OUT string &pos) {
            string key(ID_LAST_POSITION);
            key += name;
            key += aid;
            pos = x::Redis::get(key);
        }

        /**
         * Set lastest offset position (for order deals)
         * @param name order(_POS_O) or deal(_POS_D)
         * @param aid account hash code
         * @param pos [input] position
         */
        void SetLastPosition(const char *name, const string &aid, IN const string &pos) {
            string key(ID_LAST_POSITION);
            key += name;
            key += aid;
            x::Redis::set(key, pos);
        }

        /**
         * Save order PO and update order list
         * @param key order ordid (po[n]->id)
         * @param po serialized PO of order request
         */
        int SaveOrderList(const string &key, IN const string &po) {
            string fullkey(ID_ALG_ORDER_LIST);
            safe_sadd(fullkey, key);
            fullkey += key;
            x::Redis::set(fullkey, po);
            return 0;
        }

        int SaveOrderStatus(const string &ordid, IN const string &status) {
            string fullkey(ID_ALG_ORDER_STATUS);
            fullkey += ordid;
            x::Redis::set(fullkey, status);    
            return 0;
        }

        int SaveOrderKnock(const string &ordid, IN const string &knock) {
            string fullkey(ID_ALG_ORDER_KNOCK);
            fullkey += ordid;
            x::Redis::lpush(fullkey, knock);
            return 0;
        }

        /**
         * Load order PO with ordid
         * @param key order ordid (po[n]->id)
         * @param po serialized PO (persistant object) of order request
         */
        void LoadOrder(const string &key, OUT string &po) {
            string fullkey(ID_ALG_ORDER_LIST);
            fullkey += key;
            po = x::Redis::get(fullkey);
            return;
        }
        
        void LoadOrderStatus(const string &ordid, OUT string &po) {
            string fullkey(ID_ALG_ORDER_STATUS);
            fullkey += ordid;
            po = x::Redis::get(fullkey);
            return;
        }

        void LoadOrderKnocks(const string &ordid, OUT vector<string> &vl) {
            string fullkey(ID_ALG_ORDER_LIST);
            fullkey += ordid;
            x::Redis::lrange(vl, fullkey, 0, 999);  // TODO, warning on too many knocks!
            return;
        }

        int LoadOrderList(OUT vector<string> &vl) {
            string fullkey(ID_ALG_ORDER_LIST);
            return x::Redis::smembers(vl, fullkey);
        }

        /**
         * Save strategy PO and update strategy list
         * @param key algo instance uuid_str
         * @param po serialized PO (persistant object) of strategy request
         */
        int SaveStrategy(const string &key, IN const string &po) {
            string fullkey(ID_STRATEGY_LIST);
            safe_sadd(fullkey, key);
            fullkey += key;
            x::Redis::set(fullkey, po);
            return 0;
        }

        int SaveStrategyStatus(const string &key, IN const string &po) {
            string fullkey(ID_STRATEGY_STATUS);
            fullkey += key;
            x::Redis::set(fullkey, po);
            return 0;
        }

        /**
         * Load strategy PO with uuid_str
         * @param key algo instance uuid_str
         * @param po serialized PO (persistant object) of strategy request
         */
        void LoadStrategy(const string &key, OUT string &po) {
            string fullkey(ID_STRATEGY_LIST);
            fullkey += key;
            po = x::Redis::get(fullkey);
            return;
        }

        void LoadStrategyStatus(const string &key, OUT string &po) {
            string fullkey(ID_STRATEGY_STATUS);
            fullkey += key;
            po = x::Redis::get(fullkey);
            return;
        }

        /**
         * Load strategy list
         * @param domain cluster domain (e.g., ATS Node Id)
         * @param key algo instance uuid_str
         * @param po serialized PO (persistant object) of strategy request
         */
        int LoadStrategyList(const string &domain, OUT vector<string> &vl) {
            string fullkey(ID_STRATEGY_LIST);
            //fullkey += domain;
            return safe_smembers(vl, fullkey);
        }

        void ClearStrategyList(const string &domain) {
            string fullkey(ID_STRATEGY_LIST);
            //fullkey += domain;
            x::Redis::del(fullkey);
        }

        /**
         * Add a single order into strategy map (strategy ==> orders)
         * @param key strategy instance uuid
         * @param key order order id (ordid)
         */
        int SaveStrategyMap(const string &key, IN const string &ordid) {
            string fullkey(ID_ALG_ORDER_MAP);
            fullkey += key;
            safe_sadd(fullkey, ordid);
            return 0;
        }

        int LoadStrategyMap(const string &key, OUT vector<string> &vordid) {
            string fullkey(ID_ALG_ORDER_MAP);
            fullkey += key;
            return safe_smembers(vordid, fullkey);
        }

        int LoadDistributionMap(const string &key, OUT vector<string> &vordid) {
            string fullkey(ID_DISTRIBUTION);
            fullkey += key;
            return safe_smembers(vordid, fullkey);
        }

        /**
         * Save quotation PO and update its timestamp
         * @param key trade code with market prefix
         * @param po serialized PO (persistant object) of quotation data
         */
        int SaveQuotation(const string &key, IN const string &po) {
            string fullkey(ID_QUOTATION_CODE);
            fullkey += key;
            x::Redis::set(fullkey, po);
            return 0;
        }

        /**
         * Load quotation PO with code
         * @param key trade code with market prefix
         * @param po serialized PO (persistant object) of quotation data
         */
        void LoadQuotation(const string &key, OUT string &po) {
            string fullkey(ID_QUOTATION_CODE);
            fullkey += key;
            po = x::Redis::get(fullkey);
            return;
        }

    };
}
