/*
 * $Id: $
 *
 * Lock-Free queue for multi-core platform
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file FifoLockfree.h Lockfree FIFO/Queue */
#include "Base.h"

namespace ATS {
    struct entry {
        const void *data;
    };

    /**
     * Lock-Free queue for multi-core platform
     * NOTE: IS thread-safe (for single reader single writer)
     */
    class FifoLockfree : public IQueue {
    public:
        FifoLockfree(int size) : m_size(size), m_overload(0) {
            // TODO: Handle error return!
            init();
        }

        virtual ~FifoLockfree() {
            if (m_overload) {
                TRACE_THREAD(3, "FifoLockfree overload hits: %d", m_overload);
            }

            fini();
        }

        virtual int init() {
            buf = (struct entry *)malloc(m_size * sizeof(struct entry));
            if (NULL == buf) {
                LOGFILE(LOG_ERROR, "FIFO memory alloc fail!");
                return -1;
            }

            memset(buf, 0, m_size * sizeof(struct entry));
            m_beginning = buf;
            m_end = buf + (m_size -1);
            m_head = m_tail = buf;

            return 0;
        }

        virtual int fini() {
            free(buf);
            return 0;
        }

        /**
         * Check if fifo is empty
         */
        virtual bool empty() const {
            return (NULL == m_head->data);
        }

        /**
         * Pop front from fifo
         * @param pitem
         * return 0 for success, else wise -ERRNO
         */
        virtual int pop_front(OUT void **pitem) {
            if (NULL != m_tail->data) {
                *pitem = (void *)m_tail->data;
                m_tail->data = NULL;
                m_tail++;
                if (m_tail > m_end) {
                    m_tail = m_beginning;
                }
                return 0;
            }
    #ifdef UNIT_TEST
            usleep(1);
    #endif
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
            if (NULL == m_head->data) {
                m_head->data = item;
                m_head++;
                if (m_head > m_end) {
                    m_head = m_beginning;
                }
                return 0;
            }
            m_overload++;
    #ifdef UNIT_TEST
            usleep(1000);
    #endif
            return -ENOMEM;
        }

    private:
        int m_size;
        struct entry *buf;
        volatile struct entry *m_head, *m_tail;
        struct entry *m_beginning, *m_end;
        int m_overload;
    };
}
