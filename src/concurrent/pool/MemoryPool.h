/*
 * $Id: $
 *
 * Memory pool
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file MemoryPool.h Memory Allocation */

#include "Base.h"
#include "Log.h"
#include "Trace.h"

#ifdef USE_TBB
# include <tbb/scalable_allocator.h>
# include <tbb/cache_aligned_allocator.h>
# define TBB_PREVIEW_MEMORY_POOL 1
# include <tbb/memory_pool.h>
//#define USE_TBB__FIXED_POOL
# define USE_TBB__SCALABLE
//#define USE_TBB__CACHE_ALIGNED
#else /*! USE_TBB */
# include <memory>
#endif /* USE_TBB */

#include <ostream>

namespace ATS {
    using namespace std;

#ifdef USE_TBB
    typedef tbb::scalable_allocator<char>       TDefaultAllocator;
    typedef tbb::cache_aligned_allocator<char>  TCachedAllocator;
#else
    typedef std::allocator<char>                TDefaultAllocator;
    typedef std::allocator<char>                TCachedAllocator;
#endif

    extern TDefaultAllocator g_defaultPool;
    extern TCachedAllocator  g_cachedPool;

    /**
     * MemoryPool
     */
    template<typename T>
    class MemoryPool {

    protected:
        size_t m_initialSize;
        size_t m_chunkSize;
        long   m_used;
        long   m_total;

#ifdef USE_TBB
        typedef tbb::memory_pool_allocator<T> TPoolAllocator;
#else
        typedef std::allocator<T>             TPoolAllocator;        
#endif

#ifdef USE_TBB__FIXED_POOL
        typedef tbb::fixed_pool TMemoryPool;
        void               *m_buf;
        TMemoryPool         m_pool;
        /** Hidden constructor for fixed pool */
        MemoryPool(size_t bufsize) : m_buf(Malloc(bufsize)), m_pool(m_buf, bufsize), m_allocator(m_pool) {}
#endif /* USE_TBB__FIXED_POOL */

#ifdef USE_TBB__SCALABLE
        typedef tbb::memory_pool<tbb::scalable_allocator<T>> TMemoryPool;
        TMemoryPool         m_pool;
#endif /* USE_TBB__SCALABLE */

#ifdef USE_TBB__CACHE_ALIGNED
        typedef tbb::memory_pool<tbb::cache_aligned_allocator<T>> TMemoryPool;
        TMemoryPool         m_pool;
#endif /* USE_TBB__CACHE_ALIGNED */

        TPoolAllocator      m_allocator;

    public:
        MemoryPool(size_t initial_size, size_t chunk_size, size_t max_size) 
          : m_pool(), m_allocator(m_pool) {
            m_initialSize = initial_size;
            m_chunkSize   = chunk_size;
            m_used  = 0;
            m_total = max_size;
        }

        ~MemoryPool() {
#ifdef USE_TBB__FIXED_POOL
            Free(m_buf);
#endif /* USE_TBB__FIXED_POOL */
        }

        /**
         * Printout statitics
         */
        ostream &ReportStats(ostream &os) {
            os  << m_used << " / " << m_total
                << " (used/total)";
            return os;
        }

        /**
         * Allocate a object T
         */
        T * allocate(size_t n) {
            T * obj = m_allocator.allocate(1);
            if (NULL == obj) {
                LOGFILE(LOG_CRIT, "memory pool is full");
            } else {
                m_used++;
            }
            TRACE_THREAD(8, "(+) allocate %x", obj);
            return obj;
        }

        /**
         * Deallocate a object T
         */
        void deallocate(T *obj) {
            TRACE_THREAD(8, "(-) deallocate %x", obj);
            m_allocator.deallocate(obj, sizeof(T));
            m_used--;
        }
        
        /**
         * The "malloc" analogue to allocate block of memory of size bytes
         */
        static void *Malloc(size_t size) {
            return g_defaultPool.allocate(size);
        }

        /** 
         * The "free" analogue to discard a previously allocated piece of memory.
         */
        static void Free(void *ptr) { 
            g_defaultPool.deallocate((char *)ptr, sizeof(T));
        }
    };

    /**
     * New a message with default constructor (google::protobuf::message)
     * TODO: use google::protobuf::_alloc
     */
    template<typename T>
    T *NewMessage() {
        //T *p = (T *)g_defaultPool.allocate(sizeof(T));
        //g_defaultPool.construct(p);
        //return p;
        return new (g_defaultPool.allocate(sizeof(T))) T();
    }

    /**
     * Delete a message (google::protobuf::message)
     * TODO: use google::protobuf::_alloc
     */
    template<typename T>
    void DeleteMessage(T *p, size_t size = sizeof(T)) {
        //g_defaultPool.destroy(p);
        p->~T();
        g_defaultPool.deallocate((char *)p, size);
    }

    /**
     * New a cached alligned object
     */
    template<typename T>
    T *NewCached(size_t size = sizeof(T)) {
        return new (g_cachedPool.allocate(size)) T();
    }

    /**
     * Delete a cached alligned object
     */
    template<typename T>
    void DeleteCached(T *p, size_t size = sizeof(T)) {
        p->~T();
        g_cachedPool.deallocate((char *)p, size);
    }
}
