/*
 * $Id: $
 *
 * WebSocketClientImpl class implemented with Boost.Beast
 * 
 * Copyright (c) 2014-2018 Zerone.IO . All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file WebSocketClientImpl.ipp implements WebSocketClient with Boost.Beast
 */

#include "Base.h"
#include "Log.h"
#include "Trace.h"
#include "DumpFunc.h"

#include <cstdio>
#include <thread>
#include <system_error>
#include <uuid/uuid.h>

#include <boost/regex.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/algorithm/string.hpp>

#include "WebSockClient.h"
#include "Socks5.h"

using error_code    = boost::system::error_code;
using tcp           = boost::asio::ip::tcp;         // from <boost/asio/ip/tcp.hpp>
namespace ssl       = boost::asio::ssl;             // from <boost/asio/ssl.hpp>
namespace beast     = boost::beast;
namespace http      = boost::beast::http;           // from <boost/beast/http.hpp>
namespace websocket = boost::beast::websocket;      // from <boost/beast/websocket.hpp>

namespace exapi {

    /**
     * Implements websocket with Boost.Beast
     */
    class WebSocketClientImpl {
    protected:
        friend  WebSocketClient;
        typedef websocket::stream< ssl::stream<tcp::socket> >   websocket_stream;
        WebSocketClient                                        *m_intf;
        boost::asio::io_context                                 m_ioc;
        std::shared_ptr<websocket_stream>                       m_ws;
        std::shared_ptr<std::thread>                            m_worker;
        bool                                                    is_start;
        std::string                                             m_proxy;
        std::string                                             m_host;
        std::string                                             m_port;
        std::string                                             m_path;
        std::string                                             m_wss_key;

        beast::multi_buffer                                     m_rdbuf;

        static std::string generate_key() {
            char buf[40];
            char *p0 = &buf[0], *p;
            uuid_t uuid;
            uuid_generate_random(uuid);
            uuid_unparse_lower(uuid, buf);
            // remove char '-' from string s
            for (p = p0; *p != '\0'; p++) {
                char ch = *p;
                if ((int)ch != '-') {
                    *p0++ = ch;
                }
            }
            *p0 = '\0';
            return std::string(buf);
        }

        void on_connect(error_code ec) {
            if (ec) {
                LOGFILE(LOG_ERROR, "on_connect - %s", ec.message().c_str());
                return;
            }

            LOGFILE(LOG_INFO, "ws[%s] opened connection", get_key().data());

            // Preform socks4/socks5 proxy handshake
            if (!m_proxy.empty()) {
                tcp::socket &socket = m_ws->next_layer().next_layer();

                Socks5::handshake(socket, m_host, m_port, ec);
                if (ec) {
                    LOGFILE(LOG_ERROR, "socks5 handshake - %s", ec.message().c_str());
                    return;
                }
                TRACE(7, "socks5 handshaked");
            }

            // Perform the ssl handshake
            m_ws->next_layer().handshake(ssl::stream_base::client, ec);

            if (ec) {
                LOGFILE(LOG_ERROR, "ssl handshake - %s", ec.message().c_str());
                return;
            }

            TRACE(7, "ssl handshaked");

            // Perform the websocket handshake
            error_code hh_ec;
            m_ws->handshake_ex(m_host, m_path,
                [this](websocket::request_type &m) {
                    // m.insert(http::field::sec_websocket_protocol, "xmpp;ws-chat");
                    // m.insert(http::field::sec_websocket_key, m_wss_key);
                    TRACE(7, "ws[%s]handshake request: %d", 
                          this->m_wss_key.c_str(), 
                          m.count(http::field::sec_websocket_key));
                }, hh_ec
            );

            if (hh_ec) {
                LOGFILE(LOG_ERROR, "websocket handshake - %s", hh_ec.message().c_str());
                return;
            }
            TRACE(7, "websocket handshaked");

            if (nullptr != m_intf->cb_open) m_intf->cb_open();

            // TODO: set socketopt - SO_RCVTIMEO and SO_SNDTIMEO
            // see https://stackoverflow.com/questions/20188718/configuring-tcp-keep-alive-with-boostasio

            read();
        }

        void on_close(error_code ec) {
            if (ec) {
                LOGFILE(LOG_ERROR, "on_close - %s", ec.message().c_str());
                return;
            }

            LOGFILE(LOG_INFO, "ws[%s] closed connection", get_key().data());

            if (nullptr != m_intf->cb_close) m_intf->cb_close();
        }

        void on_control(websocket::frame_type type, beast::string_view str) {
            //
        }

        void on_error(const error_code &ec) {
            // 
        }

        void on_read(error_code ec, size_t bytes_read) {
            boost::ignore_unused(bytes_read);
            if (ec) {
                if (boost::system::errc::operation_canceled != ec) {
                    LOGFILE(LOG_WARN, "on_read - %s", ec.message().c_str());
                }
                return;
            }
            TRACE(8, "on_read");
            
            if (nullptr != m_intf->cb_message) {
                m_intf->cb_message(beast::buffers_to_string(m_rdbuf.data()).c_str());
            }

            // Clear the buffer
            m_rdbuf.consume(m_rdbuf.size());

            read();  // async loop
        }

        void on_write(error_code ec, size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            TRACE(8, "ws[%s] command emitted %d", 
                  get_key().data(), bytes_transferred);
        }

    public:
        WebSocketClientImpl() 
            : m_intf(NULL), m_ioc(), is_start(false) {
            m_wss_key = generate_key();
            // m_proxy = "localhost:21080";
        }
        
        ~WebSocketClientImpl() {}

        const std::string & get_key() {
            return m_wss_key;
        }

        void parse_uri(const std::string &url) {
            boost::smatch match;
            static const boost::regex pattern("^(.*:)//([A-Za-z0-9\\-\\.]+)(:[0-9]+)?(.*)$");
            if (regex_search(url, match, pattern)) {
                m_host = match[2];
                std::string port = match[3];
                if (port.empty()) {
                    m_port = "443";         // always use https
                } else {
                    m_port = port.substr(1);
                }
                m_path = match[4];
                TRACE(7, "host: '%s' port: '%s' path: '%s'", 
                      m_host.c_str(), m_port.c_str(), m_path.c_str());
            } else {
                assert(0);
            }
        }

        const std::string & set_proxy(const std::string &proxy) {
            m_proxy = proxy;
            return m_proxy;
        }

        /**
         * Start websocket connection and io processing
         * @param url websocket server address, e.g.m "ws://localhost:1984/chat"
         */
        int connect(std::string url) {
            error_code ec;

            ssl::context ssl_ctx(
                ssl::context::tlsv12_client
                //ssl::context::sslv23
            );

            parse_uri(url);

            // This holds the root certificate used for verification
            // ssl_ctx.add_certificate_authority(boost::asio::buffer(cert.data(), cert.size()), ec);
            ssl_ctx.set_default_verify_paths();
            ssl_ctx.set_options(ssl::context::default_workarounds |
                                ssl::context::no_sslv2 | 
                                ssl::context::no_sslv3 | 
                                ssl::context::tlsv12_client);
            ssl_ctx.set_verify_mode(ssl::verify_peer |
                                    ssl::verify_fail_if_no_peer_cert);
            // ssl_ctx.set_verify_callback(ssl::rfc2818_verification(address));

            tcp::resolver resolver(m_ioc);
            m_ws = std::make_shared<websocket_stream>(m_ioc, ssl_ctx);

            websocket::permessage_deflate opt;
            opt.client_enable = true;
            m_ws->set_option(opt);

            // m_ws->control_callback(on_control);

            tcp::resolver::results_type result;
            if (m_proxy.empty()) {
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

            // settings->set_connection_timeout(std::chrono::seconds(30));
            boost::asio::async_connect(m_ws->next_layer().next_layer(),
                result.begin(), result.end(),
                std::bind(&WebSocketClientImpl::on_connect, this, std::placeholders::_1)
            );
            return 0;
        }

        void start() {
            is_start = true;
            m_worker = std::make_shared<std::thread>([this]() {
                LOGFILE(LOG_INFO, "websocket worker starting...");

                while (this->is_start) {
                    size_t n = this->m_ioc.run();
                    TRACE(7, "- process_io %d -", n);
                    // if (0 == n) break; // no socket 
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }

                LOGFILE(LOG_INFO, "websocket worker exiting...");
            });
        }

        void stop() {
            is_start = false;

            // Close the WebSocket connection
            m_ws->async_close(websocket::close_code::normal,
                std::bind(&WebSocketClientImpl::on_close, this, std::placeholders::_1)
            );

            if (nullptr != m_worker && m_worker->joinable()) {
                m_worker->join();
            }
        }

        void write(const std::string &cmd) {
            // Send the message
            m_ws->text(true);
            m_ws->async_write(boost::asio::buffer(cmd), 
                std::bind(&WebSocketClientImpl::on_write, this, 
                            std::placeholders::_1, std::placeholders::_2)
            );
        }

        void read() {
            if (!m_ws->is_open() || !is_start) return;

            // Read a message into our buffer
            m_ws->text(m_ws->got_text());
            m_ws->async_read(m_rdbuf, 
                std::bind(&WebSocketClientImpl::on_read, this, 
                            std::placeholders::_1, std::placeholders::_2)
            );
        }
    }; // WebSocketClientImpl

}; // namespace exapi

/*---------------------------- WebSocketClient -----------------------------*/

#ifdef DEF_WEBSOCKET_CLIENT

using namespace exapi;

WebSocketClient::WebSocketClient(const std::string &url)
  : m_url(url), cb_open(nullptr), cb_close(nullptr), cb_message(nullptr),
    m_client(new WebSocketClientImpl())
{
    // m_url = "ws://localhost:1984/chat"; // for testing
    m_client->m_intf = this;
}

WebSocketClient::~WebSocketClient() {
    stop();
    // delete m_client;
};

#endif // DEF_WEBSOCKET_CLIENT

/*------------- Sample code ----------------
#define DEF_WEBSOCKET_CLIENT
#include "detail/WebSocketClientImpl.ipp"

void WebSocketClient::start()
{
    int ret;

    LOGFILE(LOG_DEBUG, "server '%s'...", m_url.c_str());

    if ((ret = m_client->connect(m_url)) != 0) {
        m_client->stop();
        throw std::system_error(-ret, std::generic_category());
    }

    LOGFILE(LOG_DEBUG, "ws[%s] starting...", m_client->get_key().c_str());
    m_client->start();

    for (int i = 0; i < 50; i++) {
        if (!m_client->m_ws->is_open()) {
           std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
 
    if (!m_client->m_ws->is_open()) {
        LOGFILE(LOG_ERROR, "websocket connect timeout");
        throw std::system_error(ECONNABORTED, std::generic_category());
    }

    LOGFILE(LOG_INFO, "ws[%s] started", m_client->get_key().c_str());
}

void WebSocketClient::stop()
{
    if (m_client->is_start) {
        LOGFILE(LOG_DEBUG, "ws[%s] stopping...", m_client->get_key().c_str());

        m_client->stop();

        LOGFILE(LOG_INFO, "ws[%s] stopped", m_client->get_key().c_str());
    }
}

void WebSocketClient::send(const std::string &cmd)
{
    if (!m_client->m_ws->is_open()) {
        throw std::system_error(ECONNABORTED, std::generic_category());
    }

    LOGFILE(LOG_DEBUG, "ws[%s] emit command %s", m_client->get_key().c_str(), cmd.c_str());

    m_client->write(cmd);
}

--------------- End of code --------------*/