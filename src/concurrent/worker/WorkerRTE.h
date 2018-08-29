/*
 * $Id: $
 *
 * Worker implemented as Intel DPDK RTE (Run Time environment)
 *  
 * Copyright (c) 2014-2016 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file WorkerRTE.h Worker Interface for DPDK */
#include "Base.h"
#include "Log.h"

#include "IWorker.h"

namespace ATS {

    class WorkerRTE : public IWorker
    {
    private:

    public:
        virtual int Run(void *(*func)(void *), void *arg) {
            // TODO: not implemented
            assert(0);
            return -EACCES;
        }

        virtual int WaitStop(int wait_msecs) {
            // TODO: not implemented
            assert(0);
            return -EACCES;
        }

        virtual int Kill(int signal = 9) {
            // TODO: not implemented
            assert(0);
            return -EACCES;
        }
    };
}
