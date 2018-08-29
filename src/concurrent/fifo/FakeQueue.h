/*
 * $Id: $
 *
 * Faked FIFO queue (contains only one item) for testing /debuging
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file FakeQueue.h Faked FIFO/Queue */
#include "Base.h"
#include "Trace.h"
#include "IQueue.h"

#ifdef USE_TBB
#include <tbb/atomic.h>
#else
#include <atomic>
#endif

namespace ATS {

#ifndef USE_TBB
    /**
     * FIFO queue (contains only one item) for testing /debuging 
     */
    class FakeQueue : public IQueue {
    private:
        int m_overload;
        std::atomic<void *> m_item;

    public:
        FakeQueue(void *item = NULL) : m_overload(0) {
            TRACE_THREAD(3, "FakeQueue(std) created");
            m_item = item;
        }

        virtual ~FakeQueue() {
            if (m_overload) {
                TRACE_THREAD(3, "FakeQueue(std) overload hits: %d", m_overload);
            }
        }

        /**
         * Check if fifo is empty
         */
        virtual bool empty() const {
            return (NULL == m_item);
        }

        /**
        * Pop front from fifo
        * @param pitem
        * return 0 for success, else wise -ERRNO
        */
        virtual int pop_front(OUT void **pitem) {
            void *saved = NULL;
            if (NULL != m_item) {
                saved = atomic_exchange(&m_item, (void *)NULL);
            }

            if (NULL != saved) {
                *pitem = saved;
                return 0;
            } else {

        #ifdef UNIT_TEST
                usleep(1);
        #endif
            }
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
            void *expected = NULL;
            if (m_item.compare_exchange_weak(expected, (void *)item)) {
                return 0;
            } else {
                m_overload++;
        #ifdef UNIT_TEST
                usleep(1000);
        #endif
            }

            // res = -ENOMEN;
            return -EAGAIN;
        }
    };

#else /* !USE_TBB */

    /**
     * FIFO queue (contains only one item) for testing /debuging 
     * (use intel TBB atomic)
     */
    class FakeQueue : public IQueue {
    private:
        int m_overload;
        tbb::atomic<void *> m_item;

    public:
        FakeQueue(void *item = NULL) : m_overload(0) {
            TRACE_THREAD(3, "FakeQueue(tbb) created");
            m_item = item;
        }

        virtual ~FakeQueue() {
            if (m_overload) {
                TRACE_THREAD(3, "FakeQueue(tbb) overload hits: %d", m_overload);
            }
        }

        /**
        * Pop front from fifo
        * @param pitem
        * return 0 for success, else wise -ERRNO
        */
        virtual int pop_front(OUT void **pitem) {
            void *saved = NULL;
            if (NULL != m_item) {
                saved = m_item.fetch_and_store((void *)NULL);
            }

            if (NULL != saved) {
                *pitem = saved;
                return 0;
            } else {
        #ifdef UNIT_TEST
                usleep(1);
        #endif
            }
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
            if (NULL == m_item.compare_and_swap((void *)item, NULL)) {
                return 0;
            } else {
                m_overload++;
        #ifdef UNIT_TEST
                usleep(1000);
        #endif
            }

            // res = -ENOMEN;
            return -EAGAIN;
        }
    };

#endif /* USE_TBB */

}