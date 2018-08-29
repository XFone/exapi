/*
 * $Id: $
 * 
 * Uri class declaration
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file urlhelp.h Helper classes for url handling.
 */

#include <string>

namespace exapi {

    /// TO REMOVE
    enum HTTP_PROTOCOL {
        HTTP_PROTOCOL_HTTP, 
        HTTP_PROTOCOL_HTTPS 
    };

    /// TO REMOVE
    enum HTTP_METHOD {
        METHOD_GET, 
        METHOD_POST 
    };

    /// TO REMOVE
    #ifndef UINT
    # define UINT unsigned int
    #endif

    /**
     * Uri
     * A simple strurct for saving uri data instead of restbed::Uri or 
     * std::network::uri
     */
    struct Uri {
        UINT          type;
        const char   *domain;
        std::string   api;
        std::string   url;
        HTTP_PROTOCOL protocol;
        HTTP_METHOD   method;
    };

} // namespace exapi