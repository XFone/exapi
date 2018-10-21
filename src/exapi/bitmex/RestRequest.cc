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

#ifdef USE_OPENSSL
#include <openssl/hmac.h>
#endif

#include "detail/RestRequestImpl.h"

#define DEF_HTTPREST_CLIENT
#include "detail/RestClientImpl.ipp"

RestRequest &
RestRequest::Init() {
    m_request->set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    m_request->set(http::field::accept, "*/*");
    return *this;
}

std::string &
RestRequest::ParseReponse(const response_t &rsp, std::string &body)
{
    // TODO
    // StatsLatency(rsp, std::chrono::steady_clock::now(), req->m_sent_time);

    int status_code = rsp->result_int();

    switch (status_code) {
    case 429:   // rate limit is violated
        LOGFILE(LOG_ALERT, "received status code 429 - rate limit is violated: %s, Retry-After=%s", 
                rsp->reason(),
                rsp->at(http::field::retry_after));
        break;
    case 418:   // IP is banned
        LOGFILE(LOG_EMERG, "received status code 418 - IP is banned: %s", 
                rsp->reason());
        break;

    case 503:   // system busy
        LOGFILE(LOG_ALERT, "received status code 503 - System busy: %s", 
                rsp->reason());
        break;

    default:
        break;
    } // (status_code)

    LOGFILE(LOG_DEBUG, "Response: HTTP/%1.1f %d %s", 
            rsp->version() / 10.0, status_code, rsp->reason());

#if 0
    TRACE_IF(8, {
        auto headers = rsp->get_headers();
        for (const auto header : headers) {
            _trace_impl(8, "  Header| %s: %s", header.first.data(), header.second.data());
        }
    });

    rsp->payload_size();

    if (rsp->has_header("Transfer-Encoding")) {
        restbed::Http::fetch( "\r\n", rsp);
        // TRACE(8, "  fetching delimiter");
    } else {
        int length = rsp->get_header("Content-Length", 0);
        restbed::Http::fetch(length, rsp);
        // TRACE(8, "  fetching length %d", length);
    }
#endif

    body = rsp->body();
    TRACE(7, ">>>\n%s\n>>>", body.c_str());

    TRACE_IF(8, {
        _trace_impl(8, "   Body | Length: %d", body.size());
        dump_frame("", " |", body.c_str(), body.size());
    });

    // REST API: 300 requests every 5 minutes
    // "x-ratelimit-limit": 300
    // "x-ratelimit-remaining": 297
    // "x-ratelimit-reset": 1489791662
    auto it = rsp->find("x-ratelimit-remaining");
    if (it != rsp->end()) {
        int ratelimit = std::stoi(it->value().data());
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
        m_request->method_string().data(), m_request->target().data(), expires.c_str(),
        m_request->body().data());

    AddParam("api-expires", expires);        // or 'api-nonce' for old version
    AddParam("api-signature", signature);

    return *this;
}