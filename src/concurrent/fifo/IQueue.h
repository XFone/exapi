/*
 * $Id: $
 *
 * FIFO (First-In-First-Out) queue Interface
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file IQueue.h FIFO/Queue Interface */
#include "Base.h"

namespace ATS {
    _INTERFACE_ IQueue
    {
        /**
         * Check if fifo is empty
         */
        virtual bool empty() const = 0;

        /**
         * Pop front from fifo
         * @param pitem
         * return 0 for success, else wise -ERRNO
         */
        virtual int pop_front(OUT void **pitem) = 0;

        /**
         * Push back a new item into fifo
         * @param item
         * return 0 for success, else wise -ERRNO
         */
        virtual int push_back(IN const void *item) = 0;

        /**
         * helper function
         */
        template <typename T>
        int pop_front(OUT T *pitem) {
            return pop_front((void **)pitem);
        }        
    };
}
