/*
 * $Id: $
 *
 * FIFO queue for RDMA (Infiniband) multi-host communication
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file FifiRdma.h RDMA FIFO/Queue */
#include "Base.h"

namespace ATS {
     /**
     * FIFO queue for RDMA (Infiniband) multi-host conmunication
     * NOTE: IS thread-safe
     */
    class FifoRdma : public ATS::IQueue {
        /**
         * Pop front from fifo
         * @param pitem
         * return 0 for success, else wise -ERRNO
         */
        virtual int pop_front(OUT void **pitem);

        /**
         * Push back a new item into fifo
         * -EAGAIN: temporary unavailable, push again!
         * -ENOMEM: fifo full and no enough memory
         * @param item
         * return 0 for success, else wise -ERRNO
         */
        virtual int push_back(IN const void *item);
    };
}
