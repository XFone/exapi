/*
 * $Id: $
 * 
 * RestClientImpl class implemented with boost::asio
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
#include <boost/algorithm/string.hpp>

#include "RestRequestImpl.h"
#include "HttpRestClient.h"
#include "Socks5.h"

using error_code    = boost::system::error_code;
using tcp           = boost::asio::ip::tcp;         // from <boost/asio/ip/tcp.hpp>
namespace ssl       = boost::asio::ssl;             // from <boost/asio/ssl.hpp>
namespace beast     = boost::beast;
namespace http      = boost::beast::http;           // from <boost/beast/http.hpp>

namespace exapi {

    typedef ssl::stream<tcp::socket>                            sslstream_t;

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
        bool                                                    is_open;
        bool                                                    is_pending;

        std::string                                             m_proxy;
        std::string                                             m_host;
        std::string                                             m_port;

        //beast::flat_buffer                                    m_rdbuf
        beast::multi_buffer                                     m_rdbuf;

    protected:
        void on_handshake(error_code ec) {
            if (ec) {
                LOGFILE(LOG_ERROR, "ssl handshake - %s", ec.message().c_str());
                return;
            }

            TRACE(7, "ssl handshaked");

            // TODO: set socketopt - SO_RCVTIMEO and SO_SNDTIMEO
            // see https://stackoverflow.com/questions/20188718/configuring-tcp-keep-alive-with-boostasio

            // call_async(session); // get session from queue and send request
            is_open = true;
        }

        void on_connect(error_code ec) {
            if (ec) {
                LOGFILE(LOG_ERROR, "on_connect - %s", ec.message().c_str());
                return;
            }

            // Preform socks4/socks5 proxy handshake
            if (!m_proxy.empty()) {
                tcp::socket &socket = m_ssl->next_layer();

                Socks5::handshake(socket, m_host, m_port, ec);
                if (ec) {
                    LOGFILE(LOG_ERROR, "socks5 handshake - %s", ec.message().c_str());
                    return;
                }
                TRACE(7, "socks5 handshaked");
            }

            // Perform the ssl handshake
            m_ssl->async_handshake(ssl::stream_base::client, 
                std::bind(&RestClientImpl::on_handshake, this, std::placeholders::_1)
            );
        }

        void on_close(error_code ec) {
            is_open = false;
            if (ec == boost::asio::error::eof) {
                // Rationale:
                // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
                ec.assign(0, ec.category());
            } else if (ec) {
                LOGFILE(LOG_ERROR, "on_close - %s", ec.message().c_str());
                return;
            }

            LOGFILE(LOG_INFO, "'%s' connection closed", m_host.data()); // closed gracefully
            
            // TODO re-connect
        }

        /**
         * Clear the buffer
         */
        void consume() {
            m_rdbuf.consume(m_rdbuf.size());
        }

        static void 
        on_read(std::shared_ptr<RestRequest> &session, error_code ec, size_t bytes_read) {
            HttpRestClient *client = session->m_client;

            boost::ignore_unused(bytes_read);

            if (ec) {
                if (boost::system::errc::operation_canceled != ec) {
                    LOGFILE(LOG_WARN, "on_read - %s", ec.message().c_str());
                }
                return;
            }
            TRACE(7, "on_read");

            if (nullptr != session->m_callback) {
                (*session->m_callback)(session->m_request, session->m_response);
            }

            client->m_impl->consume();
        }

        static void 
        on_write(std::shared_ptr<RestRequest> &session, error_code ec, size_t bytes_transferred) {
            HttpRestClient *client = session->m_client;

            client->m_impl->is_pending = false;

            boost::ignore_unused(bytes_transferred);

            if (ec) {
                if (boost::system::errc::operation_canceled != ec) {
                    LOGFILE(LOG_WARN, "on_write - %s", ec.message().c_str());
                }
                return;
            }
            TRACE(7, "on_write");

            client->m_impl->read_async(session);
        }

        void read_async(std::shared_ptr<RestRequest> &session) {
            http::async_read(*m_ssl, m_rdbuf, *session->m_response,
                std::bind(&RestClientImpl::on_read, session, 
                          std::placeholders::_1, std::placeholders::_2)
            );
        }

        void parse_uri(const std::string &url) {
            boost::smatch match;
            static const boost::regex pattern("^(.*:)//([A-Za-z0-9\\-\\.]+)(:[0-9]+)?(.*)$");
            if (regex_search(url, match, pattern)) {
                std::string proto = match[1];    // we should always use https
                std::string port  = match[3];
                if (port.empty()) {
                    // TRACE(7, "proto: '%s'", proto.c_str());
                    m_port = !proto.compare("https:") ? "443" : "80";
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
        RestClientImpl() : 
            m_ioc(), is_start(false), is_open(false), is_pending(false) {}
        
        ~RestClientImpl() {}

        /**
         * set Socks5 Proxy address
         */
        const std::string & set_proxy(const std::string &proxy) {
            m_proxy = proxy;
            return m_proxy;
        }

        int connect(const std::string &url) {
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

            tcp::resolver resolver(m_ioc);
            tcp::resolver::results_type result;

            if (m_proxy.empty()) {
                // we resolve domain synchronizely since it wont change much
                result = resolver.resolve(m_host, m_port, ec);
            } else {
                std::vector<std::string> fields;
                boost::split(fields, m_proxy, boost::is_any_of(":"));
                LOGFILE(LOG_INFO, "via Socks5 proxy '%s:%s'", fields[0].c_str(), fields[1].c_str());
                result = resolver.resolve(fields[0], fields[1], ec);
            }

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

        void start() {
            is_start = true;
            m_worker = std::make_shared<std::thread>([this]() {
                LOGFILE(LOG_INFO, "restclient worker starting...");

                while (this->is_start) {
                    size_t n = this->m_ioc.run();
                    TRACE(7, "- process_io %d -", n);
                    // if (0 == n) break; // no socket 
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }

                LOGFILE(LOG_INFO, "restclient worker exiting...");
            });
        }

        void stop() {
            is_start = false;

            close();

            if (nullptr != m_worker && m_worker->joinable()) {
                m_worker->join();
            }
        }

        void call_sync(std::shared_ptr<RestRequest> &session) {
            request_t req = session->m_request;

            TRACE(7, "<<< %s %s\n%s\n<<<",
                  to_string(req->method()).data(),  req->target().data(),
                  req->body().data());

            (void)http::write(*m_ssl, *req);
            (void)http::read(*m_ssl, m_rdbuf, *session->m_response);
            // consume();
        }

        void call_async(std::shared_ptr<RestRequest> &session) {
            // wait previous callback
            const int wait_time = 1;
            int wait = 0, max_wait = 3000;
            while (is_pending && wait < max_wait) {
                std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
                wait += wait_time;
            }
            if (is_pending) {
                LOGFILE(LOG_WARN, "call_async timeout");
                return;
            }

            request_t req = session->m_request;

            TRACE(7, "<<< %s %s\n%s\n<<<",
                  to_string(req->method()).data(),  req->target().data(),
                  req->body().data());

#if 0
            // TO FIX
            http::async_write(*m_ssl, *req, 
                std::bind(&RestClientImpl::on_write, session, // session->shared_from_this(), 
                          std::placeholders::_1, std::placeholders::_2)
            );
            is_pending = true;
#else
            (void)http::write(*m_ssl, *req);
            (void)http::read(*m_ssl, m_rdbuf, *session->m_response);

            if (nullptr != session->m_callback) {
                (*session->m_callback)(req, session->m_response);
            }
#endif
        }

    };   // RestClientImpl

} // namespace exapi


/*--------------------- HttpRestClient methods -----------------------------*/

#ifdef DEF_HTTPREST_CLIENT

using namespace exapi;

// C++11 has no std::make_unique which is supported by C++14
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

HttpRestClient::HttpRestClient()
    : m_impl(make_unique<RestClientImpl>())
{
    // NOTHING
}

int HttpRestClient::connect(const std::string &url, const char *proxy)
{
    if (nullptr != proxy) {
        m_impl->set_proxy(proxy);
    }
    return m_impl->connect(url);
}

void HttpRestClient::close()
{
    // m_impl->close();
    m_impl->stop();
}

bool HttpRestClient::is_open() const
{
    return m_impl->is_open;
}

std::shared_ptr<RestRequest> 
HttpRestClient::GetBuilder(HTTP_METHOD method, const char *path)
{
    std::shared_ptr<RestRequest> res = 
        RestRequest::CreateBuilder(nullptr, HTTP_PROTOCOL_HTTPS, method, path);
    
    res->SetClient(this);
    res->SetHost(m_impl->m_host);
    
    return res;
}

client_map_t HttpRestClient::m_cli_map;

std::shared_ptr<HttpRestClient>
HttpRestClient::GetInstance(const std::string &url)
{
    std::shared_ptr<HttpRestClient> client;

    auto it = m_cli_map.find(url);
    if (it == m_cli_map.end()) {
        client = std::make_shared<HttpRestClient>();
        m_cli_map[url] = client;
        client->connect(url, nullptr); // proxy
        // client->connect(url, "localhost:21080");
        client->m_impl->start();
        //client->m_impl->m_ioc.run();
    } else {
        client = it->second;
    }

    return client;
}

void HttpRestClient::DisposeInstance(const std::string &url)
{
    auto it = m_cli_map.find(url);
    if (it != m_cli_map.end()) {
        std::shared_ptr<HttpRestClient> client = it->second;
        client->m_impl->stop();
        // m_cli_map.erase(it);
    }
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
    
    if (nullptr != url) {
        return HttpRestClient::GetBuilder(url, method, path);
    } else {    // assigned in client->GetBuilder()
        auto req = std::make_shared<RestRequest>();
        req->m_request  = std::make_shared< http::request<http::string_body> >();
        req->m_response = std::make_shared< http::response<http::string_body> >();

        req->m_request->keep_alive(true);

        req->SetVersion(11)     // HTTP 1.1
            .SetMethod(HttpRestClient::to_string(method))
            .SetPath(path);

        return req;
    }
}

std::string
RestRequest::SendSync(std::shared_ptr<RestRequest> &req) {
    std::string body;

    // LOGFILE(LOG_DEBUG, "SendSync: %s '%s%s'", );

    try {
        req->ReprarePayload();
        req->GetClient()->m_impl->call_sync(req);

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
    req->m_callback  = &callback;
    
    req->ReprarePayload();

    req->GetClient()->m_impl->call_async(req);

    return 0;
}

#endif // DEF_HTTPREST_CLIENT
