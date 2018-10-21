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

} // namespace exapi



/*--------------------- HttpRestClient methods -----------------------------*/

#ifdef DEF_HTTPREST_CLIENT

using namespace exapi;

/*------------------------ RestRequest methods -----------------------------*/

std::shared_ptr<restbed::Settings> _restbed_settings;

std::string
RestRequest::SendSync(std::shared_ptr<RestRequest> &req) {
    std::string body;

    LOGFILE(LOG_DEBUG, "SendSync: %s '%s%s'", 
            req->get_method().c_str(), req->get_host().c_str(), req->get_path().c_str());

    try {
        TRACE(7, "<<<\n%s\n<<<", 
              restbed::String::to_string(restbed::Http::to_bytes(req)).c_str());

        req->m_sent_time = std::chrono::steady_clock::now();



        auto rsp = restbed::Http::sync(req, _restbed_settings);

        (void)ParseReponse(rsp, body);

    } catch (std::system_error ex) {
        LOGFILE(LOG_ERROR, "SendSync: throws exception '%s'", ex.what()); 
    }

    return body;
}

int 
RestRequest::SendAsync(std::shared_ptr<RestRequest> &req,
    const std::function<void (const std::shared_ptr<restbed::Request>, const std::shared_ptr<restbed::Response>)> &callback)
{
    LOGFILE(LOG_DEBUG, "SendAsync: %s '%s%s'", 
            req->get_method().c_str(), req->get_host().c_str(), req->get_path().c_str());

    try {
        TRACE(7, "<<<\n%s\n<<<", 
              restbed::String::to_string(restbed::Http::to_bytes(req)).c_str());

        req->m_sent_time = std::chrono::steady_clock::now();

        restbed::Http::async(req, callback, _restbed_settings);

    } catch (std::system_error ex) {
        LOGFILE(LOG_ERROR, "SendAsync: throws exception '%s'", ex.what()); 
    }

    return 0;
}

std::shared_ptr<RestRequest> 
RestRequest::CreateBuilder(const char *url, HTTP_PROTOCOL protocol, HTTP_METHOD method, const char *path) {
    const char *proto_str = ((protocol == HTTP_PROTOCOL_HTTP) ? "HTTP" : "HTTPS");

    auto req = std::make_shared<RestRequest>(url);

    //req->set_host(strstr(url, "://") + 3, false);
    req->set_protocol(proto_str);
    req->set_port((protocol == HTTP_PROTOCOL_HTTPS) ? 443 : 80);
    //req->set_version(1.1);
    req->set_path(path);
    req->set_method(HttpRestClient::to_string(method));

    req->add_header("Host", req->get_host());
    //req->add_header("Accept", "text/html,application/json,application/xml,*/*");
    //req->add_header("User-Agent", "curl/7.54.0");
    //req->add_header("Connection", "keep-alive");

    if (_restbed_settings == nullptr) {
        auto ssl_settings = std::make_shared<restbed::SSLSettings>();
        //ssl_settings->set_certificate_authority_pool(restbed::Uri( "file://certificates", restbed::Uri::Relative));
        ssl_settings->set_http_disabled(true);
        ssl_settings->set_tlsv12_enabled(true);
        //ssl_settings->set_default_workarounds_enabled(true);
        _restbed_settings = std::make_shared<restbed::Settings>();
        _restbed_settings->set_ssl_settings(ssl_settings);
    }

    return req;
}

#endif  // DEF_HTTPREST_CLIENT

/*------------- Sample code ----------------

#define DEF_HTTPREST_CLIENT
#include "detail/RestClientImpl_restbed.ipp"
using namespace exapi;

#ifdef USE_OPENSSL
#include <openssl/hmac.h>
#endif

RestRequest &
RestRequest::Init() {
    AddHeader("Accept", "*" "/" "*");
    //AddHeader("ContentType", "application/x-www-form-urlencoded");
    return *this;
}

std::string &
RestRequest::ParseReponse(const std::shared_ptr<restbed::Response> &rsp, std::string &body)
{
    // TODO
    // StatsLatency(rsp, std::chrono::steady_clock::now(), req->m_sent_time);

    int status_code = rsp->get_status_code();

    switch (status_code) {
    case 429:   // rate limit is violated
        LOGFILE(LOG_ALERT, "received status code 429 - rate limit is violated: %s, Retry-After=%s", 
                rsp->get_status_message().data(),
                rsp->get_header("Retry-After").c_str());
        break;
    case 418:   // IP is banned
        LOGFILE(LOG_EMERG, "received status code 418 - IP is banned: %s", 
                rsp->get_status_message().data());
        break;

    case 503:   // system busy
        LOGFILE(LOG_ALERT, "received status code 503 - System busy: %s", 
                rsp->get_status_message().data());
        break;

    default:
        break;
    } // (status_code)

    LOGFILE(LOG_DEBUG, "Response: HTTP/%1.1f %d %s", 
            rsp->get_version(), status_code, rsp->get_status_message().data());

    TRACE_IF(8, {
        auto headers = rsp->get_headers();
        for (const auto header : headers) {
            _trace_impl(8, "  Header| %s: %s", header.first.data(), header.second.data());
        }
    });

    if (rsp->has_header("Transfer-Encoding")) {
        restbed::Http::fetch( "\r\n", rsp);
        // TRACE(8, "  fetching delimiter");
    } else {
        int length = rsp->get_header("Content-Length", 0);
        restbed::Http::fetch(length, rsp);
        // TRACE(8, "  fetching length %d", length);
    }

    TRACE(7, ">>>\n%s\n>>>", 
          restbed::String::to_string(restbed::Http::to_bytes(rsp)).c_str());

    rsp->get_body(body, nullptr);

    TRACE_IF(8, {
        _trace_impl(8, "   Body | Length: %d", body.size());
        dump_frame("", " |", body.c_str(), body.size());
    });

    // REST API: 300 requests every 5 minutes
    // "x-ratelimit-limit": 300
    // "x-ratelimit-remaining": 297
    // "x-ratelimit-reset": 1489791662
    if (rsp->has_header("x-ratelimit-remaining")) {
        int ratelimit = rsp->get_header("x-ratelimit-remaining", 0);
        if (ratelimit <= 1) {
            LOGFILE(LOG_WARN, "rate limit is exhausted, remaining %d", ratelimit);
        }
    }

    return body;
}

RestRequest &RestRequest::ApiKey(const std::string &api_key)
{
    AddHeader("api-key", api_key);
    return *this;
}

RestRequest &RestRequest::Sign(const std::string &secret_key)
{
    time_t utc_time     = time(NULL); // generate_nonce()
    std::string expires = std::to_string(utc_time + 5); // # 5s grace period in case of clock skew
    
    std::string signature = BitmexSignature(secret_key,
        get_method().c_str(), get_path().c_str(), expires.c_str(),
        (const char *)get_body().data());

    AddParam("api-expires", expires);        // or 'api-nonce' for old version
    AddParam("api-signature", signature);

    return *this;
}

--------------- End of sample -------------*/