/*
 * $Id: $
 *
 * Interface for Message and Event producer
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file IProducer.h Producer Interface */
#include "Base.h"
#include "Common.h"

namespace AlgoApi {
    namespace DataSource {

        /** 
         * IProducer interface
         */
        _INTERFACE_ IProducer {

            /**
             * @param max maximum messages to be retrived
             * @return message object (max = 1) or list of objects (max > 1)
             */
            virtual Object getData(int max = -1) = 0;
        };

    }
}
