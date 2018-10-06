/*
 * $Id: $
 *
 * RestRequest class implementation for Bitmex
 * 
 * Copyright (c) 2014-2018 Zerone.IO . All rights reserved.
 *
 * $Log: $
 *
 */

#include "Base.h"
#include "Log.h"
#include "Trace.h"
#include "DumpFunc.h"

#include "JsonUtils.h"
#include "HttpRestClient.h"
#include "BitmexWsApi.h"
#include "detail/RestClientImpl_restbed.ipp"
using namespace exapi;

#ifdef USE_OPENSSL
#include <openssl/hmac.h>
#endif

static std::shared_ptr<restbed::Settings> _bitmex_settings;

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

    if (_bitmex_settings == nullptr) {
        auto ssl_settings = std::make_shared<restbed::SSLSettings>();
        //ssl_settings->set_certificate_authority_pool(restbed::Uri( "file://certificates", restbed::Uri::Relative));
        ssl_settings->set_http_disabled(true);
        ssl_settings->set_tlsv12_enabled(true);
        //ssl_settings->set_default_workarounds_enabled(true);
        _bitmex_settings = std::make_shared<restbed::Settings>();
        _bitmex_settings->set_ssl_settings(ssl_settings);
    }

    return req;
}

RestRequest &
RestRequest::Init() {
    AddHeader("Accept", "*/*");
    //AddHeader("ContentType", "application/x-www-form-urlencoded");
    return *this;
}

std::string
RestRequest::SendSync(std::shared_ptr<RestRequest> &req) {
    std::string body;

    LOGFILE(LOG_DEBUG, "SendSync: %s '%s%s'", 
            req->get_method().c_str(), req->get_host().c_str(), req->get_path().c_str());

    try {
        TRACE(7, "<<<\n%s\n<<<", 
              restbed::String::to_string(restbed::Http::to_bytes(req)).c_str());

        req->m_sent_time = std::chrono::steady_clock::now();



        auto rsp = restbed::Http::sync(req, _bitmex_settings);

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

        restbed::Http::async(req, callback, _bitmex_settings);

    } catch (std::system_error ex) {
        LOGFILE(LOG_ERROR, "SendAsync: throws exception '%s'", ex.what()); 
    }

    return 0;
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