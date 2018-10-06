/*
 * $Id: $
 * 
 * HttpRestClient and RestRequest implemented with restbed
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file RestClientImpl_restbed.ipp Wrap class of restbed::Request.
 */

#include <cmath>
#include <string>
#include <chrono>

#include <restbed>

#include "HttpRestClient.h"

namespace exapi {

    typedef std::shared_ptr<restbed::Request>  request_t;
    typedef std::shared_ptr<restbed::Response> response_t;

    /**
     * RestRequest
     * Wrap class of restbed::Request to let setters in builder style:
     * <pre>
     * auto reqt = RestRequest::CreateBuilder(uri);
     * reqt->Init().AddHeader("Host", host)
     *             .SetPath("/v1/test")
     *             .AddParam("p1", "1");
     * // call rest method in sync mode
     * auto resp = RestRequest::SendSync(reqt);
     * </pre>
     */
    class RestRequest : public restbed::Request {
    protected:
        // friend WebSocketClient;
        friend HttpRestClient;

        /** time_point for latency management */
        std::chrono::steady_clock::time_point m_sent_time;
        
    public:
        RestRequest() : restbed::Request() {}

        RestRequest(const std::string &srv) : restbed::Request(restbed::Uri(srv)) {}

        static std::shared_ptr<RestRequest>
        CreateBuilder(const char *domain, HTTP_PROTOCOL protocol, HTTP_METHOD method, const char *path);

        RestRequest &Init();

        /**
         *  Send request and return respose in synchronise mode
         * @param req rest request
         * @return response body (json-string)
         */
        static std::string SendSync(std::shared_ptr<RestRequest> &req);

        static int SendAsync(std::shared_ptr<RestRequest> &req, 
            const std::function<void (const request_t, const response_t)> &callback);

        static std::string& ParseReponse(const response_t &rsp, std::string &body);

        //----------------------  Setters in builder style -------------------

        RestRequest &SetBody(const restbed::Bytes &value) {
            set_body(value);
            return *this;
        }
        
        RestRequest &SetBody(const std::string &value) {
            set_body(value);
            return *this;
        }
            
        RestRequest &SetPort(const uint16_t value) {
            set_port(value);
            return *this;
        }
        
        RestRequest &SetVersion(const double value) {
            set_version(value);
            return *this;
        }
        
        RestRequest &SetPath(const std::string &value) {
            set_path(value);
            return *this;
        }
        
        RestRequest &SetHost(const std::string &value) {
            set_host(value);
            return *this;
        }
        
        RestRequest &SetMethod(const std::string &value) {
            set_method(value);
            return *this;
        }
        
        RestRequest &SetProtocol(const std::string &value) {
            set_protocol(value);
            return *this;
        }

        RestRequest &AddHeader(const std::string &name, const std::string &value) {
            add_header(name, value);
            return *this;
        }

        RestRequest &SetHeader(const std::string &name, const std::string &value) {
            set_header(name, value);
            return *this;
        }
            
        RestRequest &SetHeaders(const std::multimap<std::string, std::string> &values) {
            set_headers(values);
            return *this;
        }

        RestRequest &AddParam(const std::string&name, const std::string &value) {
            set_query_parameter(name, value);
            return *this;
        }

        RestRequest &AddParam(const std::string&name, const char *value) {
            set_query_parameter(name, value == nullptr ? "" : value);
            return *this;
        }

        /**
         * Add form optional parameter if only value is NOT null
         */
        RestRequest &AddParamIf(const std::string&name, const char *value) {
            if (nullptr != value)
                set_query_parameter(name, value);
            return *this;
        }

        /**
         * Add form optional parameter if only value is NOT zero value (int32 or int64)
         */
        template <typename T>
        RestRequest &AddParamIf(const std::string&name, T val) {
            if (val != 0)
                set_query_parameter(name, std::to_string(val));
            return *this;
        }

        /**
         * Add form optional parameter if only value is NOT zero value (double)
         */
        RestRequest &AddParamIf(const std::string&name, double val) {
            if (!std::isnan(val) && std::abs(val) > /* SATOSHI/10 */  0.000000001)
                set_query_parameter(name, std::to_string(val));
            return *this;
        }

        /**
         * Add parameters in map
         * @param values parameters in multimap
         */
        RestRequest &AddParams(const std::multimap<std::string, std::string> &values) {
            set_query_parameters(values);
            return *this;
        }

        //---------------------- API-key and Signature  ----------------------

        /**
         * Add a HTTP Header field with given api-key
         * @param api_key the api_key assigned by the exchange site
         */
        RestRequest &ApiKey(const std::string &api_key);

        /** 
         * Sign request with MD5 or SHA256 signature (depends on server)
         * MUST be called after all setters
         * @param secret_key the secret key assigned by the exchange site
         */
        RestRequest &Sign(const std::string &secret_key);

    };


#ifdef DEF_HTTPRESTCLENT
    // HttpRestClient methods

#endif

} // namespace exapi

