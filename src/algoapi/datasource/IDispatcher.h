/*
 * $Id: $
 *
 * Interface for Message and Event dispatcher
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file IDispatcher.h Dispatcher Interface */
#include "Base.h"
#include "Common.h"

namespace AlgoApi {
    namespace DataSource {

        /**
         * IDispatcher interface
         */
        _INTERFACE_ IDispatcher {
            
            /** Receive a new message from mq
             * @param issue message object
             * @return dispatch result
             */
            virtual TDispResult onDispatch(Object issue) = 0;
        };

    }
}
