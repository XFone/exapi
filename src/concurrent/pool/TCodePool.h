/*
 * $Id: $
 *
 * Hash set for trading code
 *  
 * Copyright (c) 2014-2017 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file SessionMap.h Hash map for sessions */
#include "Base.h"
#include "Trace.h"

#ifdef USE_TBB
#include <tbb/concurrent_unordered_set.h>
#include <tbb/tbb_allocator.h>
#else
#include <unordered_set>
#endif

#include <string>
#include <iostream>

namespace ATS {

    /**
     * Hashset for trading code
     */
    class TCodePool {
    public:
#ifdef USE_TBB
        typedef std::basic_string<char, std::char_traits<char>, tbb::tbb_allocator<char>> CodeString;
        typedef tbb::concurrent_unordered_set<CodeString> TCodeSet;
#else
        typedef std::string CodeString;
        typedef unordered_set<CodeString> TCodeSet;
#endif

    private:
#ifndef USE_TBB
        // std::mutex m_mutex; // TODO
#endif

        TCodeSet m_hashset_curr;
        TCodeSet m_hashset_back;

    public:
        /**
         * Default constructor
         */
        TCodePool() {}

        static std::ostream &ToJson(const TCodeSet &codes, std::ostream &os, size_t max_items) {
            int n = 0;
            os << "codes: [";
            for (CodeString &code : codes) {
                if (0 != n++) {
                    os << ", ";
                }
                os << code;
                if (n >= (int)max_items) {
                    os << " ... (total " << codes.size() << ")";
                    break;
                }
            }
            os << "]";
            return os;
        }

        const TCodeSet &swap() {
            m_hashset_back.clear();
            m_hashset_curr.swap(m_hashset_back);

            return m_hashset_back;
        }

        void emplace(const std::string &tc) {
            CodeString code(tc.c_str());
#ifdef USE_TBB
            m_hashset_curr.emplace(code);
#else /* !USE_TBB */
            m_hashset_curr.emplace(code);
#endif /* USE_TBB */
        }

        /**
         * Clear hashmap
         */
        void Clear() {
            // Critical section or concurrent
            m_hashset_curr.clear();
            m_hashset_back.clear();
        }

        /**
         * destructor
         */
        ~TCodePool() {
            Clear();
        }
    }; // SessionMap
}
