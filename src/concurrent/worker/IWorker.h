/*
 * $Id: $
 *
 * Local (thread / process) or remote worker to handler tasks
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file IWorker.h Worker Interface */
#include "Base.h"

namespace ATS {

    _INTERFACE_ IWorker
    {
        virtual int Run(void *(*func)(void *), void *arg) = 0;

        virtual int WaitStop(int wait_msecs) = 0;

        virtual int Kill(int signal = 9) = 0;
    };
}
