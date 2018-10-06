/*
 * $Id: $
 * 
 * HttpRestClient class declaration
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file HttpRestClient.h Declares RestRequest and HttpRestClient
 */

#include <chrono>
#include <string>

namespace exapi {

    enum HTTP_PROTOCOL {
        HTTP_PROTOCOL_HTTP,
        HTTP_PROTOCOL_HTTPS
    };

    enum HTTP_METHOD {
        METHOD_GET,
        METHOD_POST,
        METHOD_PUT,
        METHOD_HEAD,
        METHOD_DELETE,
        METHOD_TRACE,
        METHOD_OPTIONS,
        METHOD_CONNECT
    };

    /**
     * RestRequest depends on implements (boost::best or restbed)
     */
    class RestRequest;

    class RestClientImpl;

    /**
     * HttpRestClient
     * Wrap class of RestClientImpl
     */
    class HttpRestClient {
    protected:
        friend RestClientImpl;
        std::unique_ptr<RestClientImpl>                         m_impl;

        // HttpRestClient() = delete;

    public:

        HttpRestClient(const std::string &url);

        int connect(const std::string &url, const std::string *proxy = nullptr);
        
        void close();

        std::shared_ptr<RestRequest> 
        GetBuilder(HTTP_METHOD method, const char *path);

        /**
         * Get HTTP header and status codes in string
         */
        template<typename T> static std::string to_string(T &t);
    };

} // namespace exapi