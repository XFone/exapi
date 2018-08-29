/*
 * $Id: $
 *
 * Hash map for locating session or trade-order on its uuid
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file SessionMap.h Hash map for sessions */
#include "Base.h"
#include "Trace.h"

#ifdef USE_TBB
#include <tbb/concurrent_hash_map.h>
#include <tbb/tbb_allocator.h>
#else
#include <unordered_map>
#endif

#include <string>

#define MAP_ID_SF              0            /**< Hashmap ID for SF session */
#define MAP_ID_SF_AC           1            /**< Hashmap ID for SF account */
#define MAP_ID_PB              2            /**< Hashmap ID for PB session */
#define MAP_ID_QD              3            /**< Hashmap ID for Quotation&Datafeed */
#define MAX_MAP_IDS            4            /**< Maximum Hashmaps          */


#if __cplusplus < 201103L
#pragma message("Must compiled with C++-11 (or C++0x) enabled")
#endif /* __cplusplus */

namespace ATS {

    /**
     * Hashmap for session objects
     * {session-id} ==> {session-object}
     */
    class SessionMap {

    private:
#ifdef USE_TBB
        typedef std::basic_string<char, std::char_traits<char>, tbb::tbb_allocator<char>> IdString;
        typedef tbb::concurrent_hash_map<IdString, void *> TSessionMap;
#else
        typedef std::string IdString;
        typedef unordered_map<IdString, void *> TSessionMap;
#endif
        TSessionMap _hashmap;
        static SessionMap *_instance[MAX_MAP_IDS];

    public:
        typedef int (TraverseCallback)(void * it);

    protected:
        /**
         * Hidden default constructor
         */
        SessionMap() {}

    public:

        /**
         * Get session map instance
         * NOTE: optimize as InstanceSF, InstancePB, and InstanceQD ...
         * @param id MAP_ID_xxx
         */
        static SessionMap &Instance(int id) {
            assert(id < MAX_MAP_IDS);
            //if (unlikely(NULL == _instance[id])) {
            if (NULL == _instance[id]) {
                _instance[id] = new SessionMap();
            }
            return *_instance[id];
        }
        
        static SessionMap &InstanceSF() { return Instance(MAP_ID_SF); }
        
        static SessionMap &InstanceAC() { return Instance(MAP_ID_SF_AC); }
        
        static SessionMap &InstancePB() { return Instance(MAP_ID_PB); }

        static SessionMap &InstanceQD() { return Instance(MAP_ID_QD); }

        /**
         * Traverse all items in hashmap and trigger a callback for each of them
         * @param cb callback(void *it)
         */
        void Traverse(TraverseCallback cb) {
            for (auto &it : _hashmap) {
                if (cb(it.second) < 0) break;
            }
        }

        /**
         * Get a value (key is c-string)
         */
        template <typename T>
        T Get(const char *id) {
#ifdef USE_TBB
            TSessionMap::accessor ac;
            //TRACE_THREAD(7, "SessionMap::Get(%s)", id);
            if (_hashmap.find(ac, id)) {
                return (T)ac->second;   
            }
#else /* !USE_TBB */
            // Critical section or concurrent
            TSessionMap::iterator it = _hashmap.find(id);
            if (it != _hashmap.end()) {
                return (T)it->second;
            }
#endif /* USE_TBB */
            return NULL;
        }

        /**
         * Get a value (key is string)
         */
        template <typename T>
        T Get(std::string &id) {
            return Get<T>(id.c_str());
        }

        template <typename T>
        T Get(const unsigned char *id) {
            return Get<T>((const char *)id);
        }

        /**
         * Set a value (key is c-string)
         */
        template <typename T>
        void Set(const char *id, T val) {
#ifdef USE_TBB
            TSessionMap::accessor ac;
            _hashmap.insert(ac, id);
            ac->second = val;
            // TRACE_THREAD(7, "SessionMap::Set(%s)", id);
#else /* !USE_TBB */
            // Critical section or concurrent
            _hashmap[id] = val;            
#endif /* USE_TBB */
        }

        /**
         * Set a value (key is string)
         */
        template <typename T>
        void Set(std::string &id, T val) {
            Set<T>(id.c_str(), val);
        }

        template <typename T>
        void Set(const unsigned char *id, T val) {
            Set<T>((const char *)id, val);
        }

        /**
         * Clear hashmap
         */
        void Clear() {
            // Critical section or concurrent
            _hashmap.clear();
        }

        /**
         * destructor
         */
        ~SessionMap() {
            Clear();
        }
    }; // SessionMap
}