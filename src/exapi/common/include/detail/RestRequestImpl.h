/*
 * $Id: $
 * 
 * RestRequest class implemented with boost::asio
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file RestRequesttImpl.ipp implements RestRequest with Boost.Beast
 */

#include "Base.h"
#include "Log.h"
#include "Trace.h"
#include "DumpFunc.h"

#include <memory>
#include <map>
#include <chrono>
#include <thread>
#include <system_error>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include "HttpRestClient.h"

using error_code    = boost::system::error_code;
namespace beast     = boost::beast;
namespace http      = boost::beast::http;           // from <boost/beast/http.hpp>

namespace exapi {

    typedef std::shared_ptr<http::request<http::string_body> >  request_t;
    typedef std::shared_ptr<http::response<http::string_body> > response_t;
    
    //typedef void (* rest_callback_t)(const request_t, const response_t);
    typedef const std::function<void (const request_t, const response_t)> rest_callback_t;

    /**
     * RestRequest
     * Wrap class of request_t to let setters in builder style:
     * <pre>
     * auto reqt = RestRequest::CreateBuilder(uri);
     * reqt->Init().AddHeader("Host", host)
     *             .SetPath("/v1/test")
     *             .AddParam("p1", "1");
     * // call rest method in sync mode
     * auto resp = RestRequest::SendSync(reqt);
     * </pre>
     */
    class RestRequest : public std::enable_shared_from_this<RestRequest> {
    protected:
        // friend WebSocketClient;
        friend  HttpRestClient;
        friend  RestClientImpl;

        request_t                                               m_request;
        response_t                                              m_response;

        HttpRestClient                                         *m_client;

        /** time_point for latency management */
        std::chrono::steady_clock::time_point                   m_sent_time;

        rest_callback_t                                        *m_callback;

        std::unordered_map<std::string, std::string>            m_params;

    protected:

        HttpRestClient *GetClient() const {
            return m_client;
        }

        void SetClient(HttpRestClient *client) {
            m_client = client;
        }

    public:
        RestRequest() : m_callback(nullptr) {}

        static std::shared_ptr<RestRequest>
        CreateBuilder(const char *url_domain, HTTP_PROTOCOL protocol, HTTP_METHOD method, const char *path);

        RestRequest &Init();

        void ReprarePayload();

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

        RestRequest &SetBody(const std::string &value) {
            //m_request->body() = std::move(value);
            m_request->body() = value;
            return *this;
        }
            
        RestRequest &SetPort(const uint16_t value) {
            // m_request->port(value);
            return *this;
        }

        RestRequest &SetVersion(int value) {
            m_request->version(value);              // 1.1 is (int)11
            return *this;
        }

        RestRequest &SetVersion(const double value) {
            return SetVersion((int)(value * 10));   // 1.1 is (int)11
        }
        
        RestRequest &SetPath(const std::string &value) {
            m_request->target(value);
            return *this;
        }
        
        RestRequest &SetHost(const std::string &value) {
            m_request->set(http::field::host, value);
            return *this;
        }

        RestRequest &SetMethod(const http::verb &method) {
            m_request->method(method); // http::verb::get
            return *this;
        }

        RestRequest &SetMethod(const std::string &method) {
            m_request->method_string(method);
            return *this;
        }
        
        RestRequest &SetProtocol(const std::string &value) {
            //set_protocol(value);
            assert(0); // TODO
            return *this;
        }

        RestRequest &AddHeader(const std::string &name, const std::string &value) {
            m_request->set(http::string_to_field(name), value);
            return *this;
        }

        RestRequest &SetHeader(const std::string &name, const std::string &value) {
            m_request->set(http::string_to_field(name), value);
            return *this;
        }
            
        RestRequest &SetHeaders(const std::multimap<std::string, std::string> &values) {
            for (auto v: values) {
                SetHeader(v.first, v.second);
            }
            return *this;
        }

        RestRequest &AddParam(const std::string&name, const std::string &value) {
            m_params[name] = value;
            return *this;
        }

        RestRequest &AddParam(const std::string&name, const char *value) {
            m_params[name] = std::string(value);
            return *this;
        }

        /**
         * Add form optional parameter if only value is NOT null
         */
        RestRequest &AddParamIf(const std::string&name, const char *value) {
            if (nullptr != value)
                AddParam(name, value);
            return *this;
        }

        /**
         * Add form optional parameter if only value is NOT zero value (int32 or int64)
         */
        template <typename T>
        RestRequest &AddParamIf(const std::string&name, T val) {
            if (val != 0)
                AddParam(name, std::to_string(val));
            return *this;
        }

        /**
         * Add form optional parameter if only value is NOT zero value (double)
         */
        RestRequest &AddParamIf(const std::string&name, double val) {
            if (!std::isnan(val) && std::abs(val) > /* SATOSHI/10 */  0.000000001)
                AddParam(name, std::to_string(val));
            return *this;
        }

        /**
         * Add parameters in map
         * @param values parameters in multimap
         */
        RestRequest &AddParams(const std::multimap<std::string, std::string> &values) {
            for (auto v: values) {
                AddParam(v.first, v.second);
            }
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

    }; // RestRequest

} // namespace exapi
