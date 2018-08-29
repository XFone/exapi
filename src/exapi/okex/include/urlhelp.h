/*
 * $Id: $
 * 
 * RestRequest and UriHelp class declaration
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file urlhelp.h Helper classes for url handling and REST request.
 */

#include <restbed>

namespace exapi {

    enum HTTP_PROTOCOL {
        HTTP_PROTOCOL_HTTP, 
        HTTP_PROTOCOL_HTTPS 
    };

    enum HTTP_METHOD {
        METHOD_GET, 
        METHOD_POST 
    };

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
    public:
        RestRequest(): restbed::Request() {}
        //RestRequest(const restbed::Uri &value): restbed::Request(value) {}

        static std::shared_ptr<RestRequest> CreateBuilder(Uri &uri);
        
        RestRequest &Init();

        /// Sync request and return respose
        static std::string SendSync(std::shared_ptr<RestRequest> &req);

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

        //---------------------------  Sign request --------------------------

        /** Sign request with MD5 signature
         * MUST be called after all setters
         */
        RestRequest &Sign(const std::string &secret_key);

    };

} // namespace exapi