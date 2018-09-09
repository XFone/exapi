/*
 * $Id: $
 * 
 * RestRequest class declaration
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file RestRequest.h Wrap class of restbed::Request.
 */

#include <restbed>
#include <chrono>

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
        /** time_point for latency management */
        std::chrono::steady_clock::time_point m_sent_time;

    public:
        RestRequest(): restbed::Request() {}

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
            const std::function<void (const std::shared_ptr<restbed::Request>, const std::shared_ptr<restbed::Response>)> &callback);

        static std::string& ParseReponse(const std::shared_ptr<restbed::Response> &rsp, std::string &body);

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

        RestRequest &AddParam(const std::string&name, const std::string&value) {
            set_query_parameter(name, value);
            return *this;
        }
        
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

} // namespace exapi