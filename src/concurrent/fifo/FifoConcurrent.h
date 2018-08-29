/*
 * $Id: $
 *
 * Concurrent queue (use Intel TBB):
 * A high-performance thread-safe non-blocking concurrent bounded queue
 *  
 * Copyright (c) 2014-2017 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file FifoConcurrent.h Non-blocking Concurrent FIFO/Queue */
#include "Base.h"
#include "Trace.h"
#include "IQueue.h"

//#undef USE_TBB

#ifdef USE_TBB
#include <tbb/concurrent_queue.h>
#endif

namespace ATS {

    /**
     * Concurrent FIFO queue
     * Thread-safe
     */
    class FifoConcurrent : public IQueue {
    private:
        int m_overload;

#ifdef USE_TBB
        tbb::concurrent_bounded_queue<void *> m_queue;
#endif
 
    public:
        FifoConcurrent(size_t capacity) : m_overload(0) {
            m_queue.set_capacity(capacity);
        }

        virtual ~FifoConcurrent() {
            m_queue.clear(); // clear the queue. not thread-safe.
            if (m_overload) {
                TRACE_THREAD(3, "ConcurrentQueue overload hits: %d", m_overload);
            }
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
            if (m_queue.try_pop(*pitem)) {
                return 0;
            } else {
#ifdef UNIT_TEST
                usleep(1);
#endif
            }
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
            if (m_queue.try_push((void *)item)) {
                return 0;
            } else {
                m_overload++;
#ifdef UNIT_TEST
                usleep(1000);
#endif
            }
#endif /* USE_TBB */

            // res = -ENOMEN;
            return -EAGAIN;
        }
    };
}