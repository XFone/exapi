/*
 * $Id: $
 *
 * RestRequest class implementation for Binance
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

    /* 
     * Binance specific reponse code, see
     * https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#limits
     */
    int status_code = rsp->result_int();

    switch (status_code) {
    case 429:   // rate limit is violated
        LOGFILE(LOG_ALERT, "received status code 429 - rate limit is violated: %s", 
                rsp->reason());
        break;
    case 418:   // IP is banned
        LOGFILE(LOG_EMERG, "received status code 418 - IP is banned: %s", 
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

    return body;
}

RestRequest &RestRequest::ApiKey(const std::string &api_key)
{
    AddHeader("X-MBX-APIKEY", api_key);
    return *this;
}

RestRequest &RestRequest::Sign(const std::string &secret_key)
{
    // TODO
    return *this;
}