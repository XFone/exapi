/*
 * $Id: $
 *
 * MsgQueue Interface
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file IAlgoClient.h AlgoClient Interface */
#include "Base.h"
#include "Common.h"

namespace AlgoApi {
    namespace DataSource {
        /**
         * IMsgQueueClient interface
         */
        _INTERFACE_ IMsgQueueClient {

            virtual void Send(Object msg) = 0;
            
            virtual Object Recv() = 0;

        };
    }
}
