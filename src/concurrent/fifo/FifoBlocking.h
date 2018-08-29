/*
 * $Id: $
 *
 * Blocking queue (use Intel TBB):
 * A high-performance thread-safe blocking concurrent bounded queue
 *  
 * Copyright (c) 2014-2017 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file FifoBlocking.h Blocking FIFO/Queue */
#include "Base.h"
#include "Trace.h"
#include "IQueue.h"

#ifdef USE_TBB
#include <tbb/concurrent_queue.h>
#else
#include <condition_variable>
#include <list>
#endif

namespace ATS {

    /**
     * Concurrent blocking FIFO queue
     * Thread-safe
     */
    class FifoBlocking : public IQueue {
    private:

#ifdef USE_TBB
        tbb::concurrent_bounded_queue<void *> m_queue;
#else
        mutable std::mutex      m_mutex;  
        std::condition_variable m_condvar;
        std::list<void *>       m_queue;
#endif
 
    public:
        FifoBlocking(size_t capacity) {
            m_queue.set_capacity(capacity);
        }

        virtual ~FifoBlocking() {
            m_queue.clear(); // clear the queue. not thread-safe.
        }

        /**
         * Check if fifo is empty
         */
        virtual bool empty() const {
            return m_queue.empty();
        }

        /**
        * Pop front from fifo
        * @param pitem
        * return 0 for success, else wise -ERRNO
        */
        virtual int pop_front(OUT void **pitem) {
#ifdef USE_TBB
            m_queue.pop(*pitem);
#else  /* !USE_TBB */
            std::unique_lock<std::mutex> lock(m_mutex);
            m_condvar.wait(lock, [this] {   // C++11
                return !m_queue.empty();
            });
            *pitem = m_queue.front();
            m_queue.pop_front();
#endif /* USE_TBB */
            return 0;
        }

        /**
        * Try pop front from fifo (non-blocking)
        * @param pitem
        * return 0 for success, else wise -ERRNO
        */
        int try_pop(OUT void **pitem) {
#ifdef USE_TBB
            if (m_queue.try_pop(*pitem)) {
                return 0;
            }
#else
            assert(0);
#endif /* USE_TBB */
            return -EAGAIN;
        }

        /**
         * Push back a new item into fifo
         * -EAGAIN: temporary unavailable, push again!
         * -ENOMEM: fifo full and no enough memory
         * @param item
         * return 0 for success, else wise -ERRNO
         */
        virtual int push_back(IN const void *item) {

#ifdef USE_TBB
            m_queue.push((void *)item);
#else  /* !USE_TBB */
            std::lock_guard<std::mutex> lock (m_mutex); 
            m_queue.push_back((void *)item);
            m_condvar.notify_all(); 
#endif /* USE_TBB */

            return 0;
        }
    };
}