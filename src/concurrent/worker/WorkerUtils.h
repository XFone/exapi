/*
 * $Id: $
 *
 * Utilites for starting/stopping all (thread) tasks and workers
 *  
 * Copyright (c) 2014-2016 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file WorkerUtils.h Workers' Utilities */
#include "Base.h"

namespace ATS {
    
    namespace Worker {
        /**
         * Start task schedule
         */
        void BeginSchedule();

        /**
         * Report schedule and heartbeat
         */
        void ReportHeartbeat();

        /**
         * Stop task schedule
         */
        void EndSchedule();
    };
}