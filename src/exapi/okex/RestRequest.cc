/*
 * $Id: $
 *
 * RestRequest class implementation
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

#include "urlhelp.h"
#include "RestRequest.h"
using namespace exapi;

#ifdef USE_OPENSSL
#include <openssl/md5.h>
#endif

std::shared_ptr<RestRequest> 
RestRequest::CreateBuilder(const char *domain, HTTP_PROTOCOL protocol, HTTP_METHOD method, const char *path) {
    const char *proto_str = ((protocol == HTTP_PROTOCOL_HTTP) ? "HTTP" : "HTTPS");
    const char *method_str = ((method == METHOD_GET) ? "GET" : "POST");

    //std::string _uri = proto_str + domain + path;
    auto req = std::make_shared<RestRequest>(/*restbed::Uri(_uri)*/);

    req->set_host(domain);
    req->set_protocol(proto_str);
    req->set_port((protocol == HTTP_PROTOCOL_HTTPS) ? 443 : 80);
    //req->set_version(1.1);
    req->set_path(path);
    req->set_method(method_str);

    req->add_header("Host", domain);
    //req->add_header("Accept", "text/html,application/json,application/xml,*/*");
    //req->add_header("User-Agent", "Mozilla/5.0 (exapi; rv:1.0.0) exapi");
    req->add_header("Connection", "keep-alive");

    // xiaofeng 2018-08-28 TO CHECK: 'a/5'
    restbed::Bytes body({'\0'});
    req->set_body(body);
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
        TRACE(7, "<<<\n%s\n<<<", restbed::Http::to_bytes(req).data());
#if 0
        auto ssl_settings = std::make_shared<restbed::SSLSettings>();
        ssl_settings->set_certificate_authority_pool(restbed::Uri( "file://certificates", restbed::Uri::Relative));
        ssl_settings->set_http_disabled(true);
        ssl_settings->set_tlsv12_enabled(true);
        auto settings = std::make_shared<restbed::Settings>();
        settings->set_ssl_settings(ssl_settings);
        auto response = restbed::Http::sync(req, settings);
#else
        auto rsp = restbed::Http::sync(req);
#endif

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
        TRACE(7, "<<<\n%s\n<<<", restbed::Http::to_bytes(req).data());
        restbed::Http::async(req, callback);
    } catch (std::system_error ex) {
        LOGFILE(LOG_ERROR, "SendAsync: throws exception '%s'", ex.what()); 
    }

    return 0;
}

std::string &
RestRequest::ParseReponse(const std::shared_ptr<restbed::Response> &rsp, std::string &body)
{
    LOGFILE(LOG_DEBUG, "Response: HTTP/%1.1f %d %s", 
            rsp->get_version(), rsp->get_status_code(), rsp->get_status_message().data());

    auto headers = rsp->get_headers();

    TRACE_IF(8, {
        for (const auto header : headers) {
            _trace_impl(8, "  Header| %s: %s", header.first.data(), header.second.data());
        }
    });

    if (rsp->has_header("Transfer-Encoding")) {
        restbed::Http::fetch( "\r\n", rsp);
        // TRACE(8, "  fetching delimiter");
    } else {
        auto length = rsp->get_header("Content-Length", 0);
        restbed::Http::fetch(length, rsp);
        // TRACE(8, "  fetching length %d", length);
    }

    TRACE(7, ">>>\n%s\n>>>", restbed::Http::to_bytes(rsp).data());

    rsp->get_body(body, nullptr);

    TRACE_IF(8, {
        _trace_impl(8, "   Body | Length: %d", body.size());
        dump_frame("", " |", body.c_str(), body.size());
    });

    return body;
}

RestRequest &
RestRequest::Sign(const std::string &secret_key) {
    unsigned char md[MD5_DIGEST_LENGTH];
    char strMd5[2 * MD5_DIGEST_LENGTH + 1];

    AddParam("secret_key", secret_key);

    std::string params;

    for (const auto qp : this->get_query_parameters()) {
         params += restbed::Uri::encode_parameter(qp.first) + "=" + 
                   restbed::Uri::encode_parameter(qp.second) + "&";
    }

    (void)MD5((const unsigned char *)params.c_str(), params.size() - 1, md);

#if 1
    for (int n = 0; n < MD5_DIGEST_LENGTH; n++) {
        sprintf(&strMd5[n << 1], "%02X", md[n]);
    }
#else
    size_t len = MD5_DIGEST_LENGTH;
    int n = snprintf_hex(strMd5, sizeof(strMd5), (const char *)md, &len);
    strMd5[n] = '\0';
#endif

    AddParam("sign", strMd5);

    return *this;
}