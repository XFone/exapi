/*
 * $Id: $
 * 
 * RestClientImpl and RestRequest class implemented with boost::asio
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file RestClientImpl.ipp implements RestClient with Boost.Beast
 */

#include "Base.h"
#include "Log.h"
#include "Trace.h"
#include "DumpFunc.h"

#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <system_error>

#include <boost/regex.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>

#include "HttpRestClient.h"

using error_code    = boost::system::error_code;
using tcp           = boost::asio::ip::tcp;         // from <boost/asio/ip/tcp.hpp>
namespace ssl       = boost::asio::ssl;             // from <boost/asio/ssl.hpp>
namespace beast     = boost::beast;
namespace http      = boost::beast::http;           // from <boost/beast/http.hpp>

namespace exapi {

    typedef std::shared_ptr<http::request<http::string_body> >  request_t;
    typedef std::shared_ptr<http::response<http::string_body> > response_t;
    typedef ssl::stream<tcp::socket>                            sslstream_t;
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
    class RestRequest {
    protected:
        // friend WebSocketClient;
        friend  HttpRestClient;
        friend  RestClientImpl;

        request_t                                               m_request;
        response_t                                              m_response;

        std::shared_ptr<sslstream_t>                            m_ssl;

        /** time_point for latency management */
        std::chrono::steady_clock::time_point                   m_sent_time;

        rest_callback_t                                        *m_callback;
        //beast::flat_buffer                                    m_rdbuf
        beast::multi_buffer                                     m_rdbuf;

        std::unordered_map<std::string, std::string>            m_params;

    protected:

        void on_read(error_code ec, size_t bytes_read) {
            boost::ignore_unused(bytes_read);
            if (ec) {
                if (boost::system::errc::operation_canceled != ec) {
                    LOGFILE(LOG_WARN, "on_read - %s", ec.message().c_str());
                }
                return;
            }
            TRACE(8, "on_read");

            if (nullptr != m_callback) {
                (*m_callback)(m_request, m_response);
            }
            // Clear the buffer
            m_rdbuf.consume(m_rdbuf.size());
        }

        void on_write(error_code ec, size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);

            if (ec) {
                if (boost::system::errc::operation_canceled != ec) {
                    LOGFILE(LOG_WARN, "on_write - %s", ec.message().c_str());
                }
                return;
            }
            TRACE(8, "on_write");

            http::async_read(*m_ssl, m_rdbuf, *m_response,
                std::bind(&RestRequest::on_read, this, 
                          std::placeholders::_1, std::placeholders::_2)
            );
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
        
        RestRequest &SetVersion(const double value) {
            m_request->version((int)(value * 10)); // 1.1 is (int)11
            return *this;
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

    };

    /**
     * Implement HttpRestClient
     */
    class RestClientImpl {
    protected:
        friend HttpRestClient;
        boost::asio::io_context                                 m_ioc;
        std::shared_ptr<sslstream_t>                            m_ssl;
        std::shared_ptr<std::thread>                            m_worker;
        bool                                                    is_start;

        std::string                                             m_host;
        std::string                                             m_port;

        std::shared_ptr<RestRequest>                            m_session;

    protected:
        void on_connect(error_code ec) {
            if (ec) {
                LOGFILE(LOG_ERROR, "on_connect - %s", ec.message().c_str());
                return;
            }

            // Perform the ssl handshake
            m_ssl->handshake(ssl::stream_base::client, ec);

            if (ec) {
                LOGFILE(LOG_ERROR, "ssl handshake - %s", ec.message().c_str());
                return;
            }

            TRACE(7, "ssl handshaked");

            // TODO: set socketopt - SO_RCVTIMEO and SO_SNDTIMEO
            // see https://stackoverflow.com/questions/20188718/configuring-tcp-keep-alive-with-boostasio

            // write(m_session->m_request); // send request
        }

        void on_close(error_code ec) {
            if (ec) {
                LOGFILE(LOG_ERROR, "on_close - %s", ec.message().c_str());
                return;
            }

            LOGFILE(LOG_INFO, "server closed connection");
            // TODO re-connect
        }

        void parse_uri(const std::string &url) {
            boost::smatch match;
            static const boost::regex pattern("^(.*:)//([A-Za-z0-9\\-\\.]+)(:[0-9]+)?(.*)$");
            if (regex_search(url, match, pattern)) {
                std::string proto = match[1];    // we should always use https
                std::string port  = match[3];
                if (port.empty()) {
                    m_port = !proto.compare("https") ? "443" : "80";
                } else {
                    m_port = port.substr(1);
                }
                m_host = match[2];
                std::string m_path = match[4];
                TRACE(7, "host: '%s' port: '%s' path: '%s'", 
                      m_host.c_str(), m_port.c_str(), m_path.c_str());
            } else {
                assert(0);
            }
        }

    public:
        RestClientImpl() : m_ioc(), is_start(false) {}
        
        RestClientImpl(const std::string &url) : RestClientImpl() {
            parse_uri(url);
        }

        ~RestClientImpl() {}

        int connect(const std::string &url, const std::string *proxy) {
            error_code ec;

            parse_uri(url);

            // setup ssl
            ssl::context ssl_ctx(
                ssl::context::tlsv12_client
              //ssl::context::sslv23_client
            );
            ssl_ctx.set_default_verify_paths();
            ssl_ctx.set_options(ssl::context::default_workarounds);
            ssl_ctx.set_verify_mode(ssl::verify_peer |
                                    ssl::verify_fail_if_no_peer_cert);
            m_ssl = std::make_shared<sslstream_t>(m_ioc, ssl_ctx);

            // Set SNI Hostname (many hosts need this to handshake successfully)
            if (!SSL_set_tlsext_host_name(m_ssl->native_handle(), m_host.c_str())) {
                error_code ec {
                    static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()
                };
                LOGFILE(LOG_ERROR, "SSL_set_tlsext_host_name: %s", ec.message().c_str());
                return -ec.value();
            }

            // we resolve domain synchronizely since it wont change much
            tcp::resolver resolver(m_ioc);
            auto result = resolver.resolve(m_host, m_port, ec);

            if (ec) {
                LOGFILE(LOG_ERROR, "tcp::resolver::resolve: %s", ec.message().c_str());
                return -ec.value();
            }

            boost::asio::async_connect(m_ssl->next_layer(),
                result.begin(), result.end(),
                std::bind(&RestClientImpl::on_connect, this, std::placeholders::_1)
            );

            return 0; // success
        }

        void close() {
            // Gracefully close the stream
            m_ssl->async_shutdown(
                std::bind(&RestClientImpl::on_close, this, std::placeholders::_1)
            );
        }
  
    };   // RestClientImpl

} // namespace exapi


/*--------------------- HttpRestClient methods -----------------------------*/

#ifdef DEF_HTTPREST_CLIENT

using namespace exapi;

HttpRestClient::HttpRestClient(const std::string &url)
    : m_impl(new RestClientImpl(url))
{
    // NOTHING
}

int HttpRestClient::connect(const std::string &url, const std::string *proxy)
{
    return m_impl->connect(url, proxy);
}

void HttpRestClient::close()
{
    m_impl->close();
}

std::shared_ptr<RestRequest> 
HttpRestClient::GetBuilder(HTTP_METHOD method, const char *path)
{
    const char *host = m_impl->m_host.c_str(); // domain name
    std::shared_ptr<RestRequest> res = 
        RestRequest::CreateBuilder(host, HTTP_PROTOCOL_HTTPS, method, path);
    
    if (res->m_ssl == nullptr) {
        res->m_ssl = m_impl->m_ssl; // resuse stream
    }

    return res;
}

/*------------------------ RestRequest methods -----------------------------*/

void RestRequest::ReprarePayload()
{
    std::string params;
    bool is_path_parameter = false;
    if (m_request->method() == http::verb::get) {
        is_path_parameter = true;
    } else {
        //
    }

    if (is_path_parameter) {
        for (auto pa : m_params) {
            params += "&" + pa.first + "=" + pa.second;
        }
        if (params.size() > 0) {
            params[0] = '?';
            // url-encoder
            std::string path(m_request->target());
            m_request->target(path + params);
        }
    } else { // use form (or json)
        for (auto pa : m_params) {
            params += "\r" + pa.first + "=" + pa.second;
        }
        if (params.size() > 0) {
            // form-encoder
            m_request->body() = params.substr(1);
        }
    }

    // m_params.clear();
    this->m_request->prepare_payload();
    this->m_sent_time = std::chrono::steady_clock::now();   // must send 
}

std::shared_ptr<RestRequest> 
RestRequest::CreateBuilder(const char *url, HTTP_PROTOCOL protocol, HTTP_METHOD method, const char *path) {
    
    auto req = std::make_shared<RestRequest>();

    // TODO
    // auto impl = RestClientImpl::instance(url, protocol);
    // req->m_ssl = impl->m_ssl;

    req->m_request  = std::make_shared< http::request<http::string_body> >();
    req->m_response = std::make_shared< http::response<http::string_body> >();

    req->m_request->keep_alive(true);

    req->SetHost(url)
        .SetVersion(1.1)    // HTTP 1.1
        .SetMethod(HttpRestClient::to_string(method))
        .SetPath(path);

    return req;
}

std::string
RestRequest::SendSync(std::shared_ptr<RestRequest> &req) {
    std::string body;

    // LOGFILE(LOG_DEBUG, "SendSync: %s '%s%s'", );

    try {
        TRACE(7, "<<<\n%s\n<<<", "");

        req->ReprarePayload();
        http::write(*req->m_ssl, *req->m_request);

        http::read(*req->m_ssl, req->m_rdbuf, *req->m_response);
        (void)ParseReponse(req->m_response, body);

    } catch (std::system_error ex) {
        LOGFILE(LOG_ERROR, "SendSync: throws exception '%s'", ex.what()); 
    }

    return body;
}

int 
RestRequest::SendAsync(std::shared_ptr<RestRequest> &req,
    const std::function<void (const request_t, const response_t)> &callback)
{
    // LOGFILE(LOG_DEBUG, "SendAsync: %s '%s%s'", );

    try {
        // TRACE(7, "<<<\n%s\n<<<", "");

        req->m_callback  = &callback;
        
        req->ReprarePayload();
        http::async_write(*req->m_ssl, *req->m_request, 
            std::bind(&RestRequest::on_write, *req, 
                      std::placeholders::_1, std::placeholders::_2)
        );

    } catch (std::system_error ex) {
        LOGFILE(LOG_ERROR, "SendAsync: throws exception '%s'", ex.what()); 
    }

    return 0;
}

#endif // DEF_HTTPREST_CLIENT
