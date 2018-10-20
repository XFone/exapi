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
#include <unordered_map>

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

    class HttpRestClient;

    typedef std::unordered_map<std::string, std::shared_ptr<HttpRestClient> > client_map_t;
    
    /**
     * HttpRestClient
     * Wrap class of RestClientImpl
     */
    class HttpRestClient {
    protected:
        friend RestClientImpl;
        friend RestRequest;

        std::unique_ptr<RestClientImpl>                 m_impl;
        static client_map_t                             m_cli_map;

        // HttpRestClient() = delete;

    public:

        HttpRestClient();

        int connect(const std::string &url, const char *proxy = nullptr);
        
        void close();

        bool is_open() const;

        std::shared_ptr<RestRequest> 
        GetBuilder(HTTP_METHOD method, const char *path);

        /**
         * Get RestRequestBuilder from exsited HttpRestClient instance
         */
        static std::shared_ptr<RestRequest>
        GetBuilder(const std::string &url, HTTP_METHOD method, const char *path) {
            return GetInstance(url)->GetBuilder(method, path);
        }

        static std::shared_ptr<HttpRestClient> GetInstance(const std::string &url);

        static void DisposeInstance(const std::string &url);

        /**
         * Get HTTP header and status codes in string
         */
        template<typename T> static std::string to_string(T &t);
    };

} // namespace exapi