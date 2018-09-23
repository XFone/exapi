/*
 * $Id: $
 *
 * WebSocketClient class implementation
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

#include <cstdio>
#include <system_error>
#include <zlib.h>

#include "LoggerImpl.h"
#include "WebSockClient.h"
using namespace exapi;

#include <restbed>
#include "corvusoft/restbed/detail/web_socket_manager_impl.hpp"

namespace exapi {
    class WebSocketClientImpl {
    protected:
        friend WebSocketClient;
        std::shared_ptr<restbed::detail::WebSocketManagerImpl> m_ws_mgnr;
        std::shared_ptr<restbed::WebSocket>                    m_socket;
        std::shared_ptr<restbed::Request>                      m_request;
        std::shared_ptr<std::thread>                           m_worker;
        bool                                                   is_start;
        bool                                                   is_deflate;
        std::string                                            m_sec_key;

        static std::string generate_key() {
        #if 0
            unsigned char raw_key[16];

            for (int i = 0; i < 4; i++) {
                conv.i = m_rng();
                std::copy(conv.c, conv.c + 4, &raw_key[i * 4]);
            }
            return base64_encode(raw_key, size_of(raw_key));
        #endif
            return std::string("UxbkqrAEToZeKbwSTU0TCg==");
        }

        void set_websocket_headers(const std::string &url, const std::shared_ptr<restbed::Request> &req) {
            bool is_https = (url.find("wss:") != std::string::npos);

            req->set_protocol(is_https ? "HTTPS" : "HTTP");
            req->set_port(is_https ? 443 : 80);

            std::string host_port(req->get_host()); 
            host_port += ":"; 
            host_port += std::to_string(req->get_port());

            req->add_header("Host", /* host_port */ req->get_host());
            req->add_header("Connection", "Upgrade");
            req->add_header("Pragma", "no-cache");
            req->add_header("Cache-Control", "no-cache");
            req->add_header("Upgrade", "websocket");
            req->add_header("Sec-WebSocket-Version", "13");
            req->add_header("Sec-WebSocket-Key", m_sec_key);
            req->add_header("Sec-WebSocket-Extensions", "permessage-deflate; client_max_window_bits");
        }

    public:
        WebSocketClientImpl() : is_start(false), is_deflate(false) {
            m_sec_key = generate_key(); 
        }
        
        ~WebSocketClientImpl() {}

        /**
         * Start websocket connection and io processing
         * @param url websocket server address, e.g.m "wss://www.bitmex.com/realtime"
         */
        int connect(std::string url) {
            if (nullptr == m_request) {
                m_request = std::make_shared<restbed::Request>(restbed::Uri(url));
                set_websocket_headers(url, m_request);
            }

            TRACE(7, "<<<\n%s\n<<<", 
                  restbed::String::to_string(restbed::Http::to_bytes(m_request)).c_str());

            auto settings = std::make_shared<restbed::Settings>();
            settings->set_connection_timeout(std::chrono::seconds(30));

            auto ssl_settings = std::make_shared<restbed::SSLSettings>();
            //ssl_settings->set_certificate_authority_pool(restbed::Uri( "file://certificates", restbed::Uri::Relative));
            ssl_settings->set_http_disabled(true);
            ssl_settings->set_tlsv12_enabled(true);
            //ssl_settings->set_default_workarounds_enabled(true);
            settings->set_ssl_settings(ssl_settings);

            auto response = restbed::Http::sync(m_request, settings);

            TRACE(7, ">>>\n%s\n>>>", 
                  restbed::String::to_string(restbed::Http::to_bytes(response)).c_str());

            if (response->get_status_code() != (int)restbed::SWITCHING_PROTOCOLS) {
                LOGFILE(LOG_ERROR, "Invalid response status in websocket - %d", 
                        response->get_status_code());
                return -EPROTO;
            }

            auto upgrade = response->get_header("upgrade", restbed::String::lowercase);
            if (upgrade.compare(0, sizeof("websocket") - 1, "websocket")) {
                LOGFILE(LOG_ERROR, "Invalid http header 'upgrade' - '%s'", upgrade.c_str());
                return -EPROTO;
            }

            auto extensions = response->get_header("Sec-WebSocket-Extensions", restbed::String::lowercase);
            if (extensions.find("permessage-deflate") != std::string::npos) {
                is_deflate = true;
                LOGFILE(LOG_INFO, "Enable websocket permessage-deflate");
            }

            if (nullptr == m_ws_mgnr) {
                m_ws_mgnr = std::make_shared<restbed::detail::WebSocketManagerImpl>();
                m_ws_mgnr->set_logger(LoggerImpl::GetInstance());
            }

            if (nullptr == m_socket) {
                m_socket = m_ws_mgnr->create(m_request);
            } else {
                m_ws_mgnr->update(m_socket, m_request);
            }

            // init socket and tls settings
            // m_socket->get_socket()->
            // set_socket_init_handler(bind(&type::on_socket_init,this,::_1));
            // set_tls_init_handler(bind(&type::on_tls_init,this,::_1));
            // set_open_handshake_timeout(3000);
            // set_close_handshake_timeout(3000);

            return 0;
        }

        void start() {
            is_start = true;
            m_worker = std::make_shared<std::thread>([this]() {
                LOGFILE(LOG_INFO, "websocket worker starting...");

                while (this->is_start) {
                    int res = this->m_ws_mgnr->process_io(this->m_request);
                    TRACE(7, "- %d -", res);
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                }

                LOGFILE(LOG_INFO, "websocket worker exiting...");
            });
            // this->m_ws_mgnr->process_io(this->m_request);
        }

        void stop() {
            is_start = false;
            if (nullptr != m_socket && !m_socket->is_closed()) {
                m_socket->close();
            }
            if (nullptr != m_worker && m_worker->joinable()) {
                m_worker->join();
            }
        }

    }; // WebSocketClientImpl
}

const char *deflate(const std::vector<unsigned char> &in, std::string &out) {
    unsigned char buf[1024];
    size_t osize = sizeof(buf);
    uncompress(buf, &osize, in.data(), in.size());
    out.assign((char *)buf, osize);
    return out.c_str();
}

/*---------------------------- WebSocketClient -----------------------------*/

WebSocketClient::WebSocketClient(const std::string url)
  : m_url(url), cb_open(nullptr), cb_close(nullptr), cb_message(nullptr),
    m_client(new WebSocketClientImpl())
{
    // m_url = "ws://localhost:1984/chat"; // for testing
}

WebSocketClient::~WebSocketClient() {
    stop();
    // delete m_client;
};

void WebSocketClient::start()
{
    int ret;

    LOGFILE(LOG_DEBUG, "server '%s'...", m_url.c_str());

    if ((ret = m_client->connect(m_url)) != 0) {
        m_client->stop();
        throw std::system_error(-ret, std::generic_category());
    }

    auto socket = m_client->m_socket;

    LOGFILE(LOG_DEBUG, "websocket is %s", socket->is_open() ? "open" : "closed");

    // open_handler
#if 0
    socket->set_open_handler([this](const std::shared_ptr<restbed::WebSocket> &ws) {
        LOGFILE(LOG_INFO, "ws[%s] opened connection", ws->get_key().data());

        if (nullptr != this->cb_open) this->cb_open();
    });
#else
    if (nullptr != this->cb_open) this->cb_open();
#endif

    // close_handler
    socket->set_close_handler([this](const std::shared_ptr<restbed::WebSocket> &ws) {
        if (ws->is_open()) {
            auto response = std::make_shared<restbed::WebSocketMessage>(
                restbed::WebSocketMessage::CONNECTION_CLOSE_FRAME,
                restbed::Bytes( { 10, 00 })
            );
            ws->send(response);
        }

        LOGFILE(LOG_INFO, "ws[%s] closed connection", ws->get_key().data());

        if (nullptr != this->cb_close) this->cb_close();
    });

    // error_handler
    socket->set_error_handler([this](const std::shared_ptr<restbed::WebSocket> &ws, 
                                     const std::error_code &ec) {
        if (this->m_client->is_start) {
            LOGFILE(LOG_WARN, "ws[%s] error - %s", 
                    ws->get_key().data(), ec.message().data());
            if (ws->is_closed()) {
                // re-connecting
                this->m_client->connect(this->m_url);
            }
        }
    });

    LOGFILE(LOG_DEBUG, "ws[%s] starting...", socket->get_key().c_str());
    m_client->start();

    // message_handler
    socket->set_message_handler([this](const std::shared_ptr<restbed::WebSocket> &ws, 
                                       const std::shared_ptr<restbed::WebSocketMessage> &msg) {
        const auto opcode = msg->get_opcode();

        LOGFILE(LOG_DEBUG, "ws[%s] got data with opcode %d", ws->get_key().c_str(), opcode);
    
        if (opcode == restbed::WebSocketMessage::PING_FRAME) {
            auto response = std::make_shared<restbed::WebSocketMessage>(
                restbed::WebSocketMessage::PONG_FRAME, 
                msg->get_data()
            );
            ws->send(response);
        } else if (opcode == restbed::WebSocketMessage::PONG_FRAME) {
            //Ignore PONG_FRAME.
            //
            //Every time the ping_handler is scheduled to run, it fires off a PING_FRAME to each
            //WebSocket. The client, if behaving correctly, will respond with a PONG_FRAME.
            //
            //On each occasion the underlying TCP socket sees any packet data transfer, whether
            //a PING, PONG, TEXT, or BINARY... frame. It will automatically reset the timeout counter
            //leaving the connection active; see also Settings::set_connection_timeout.
            return;
        } else if (opcode == restbed::WebSocketMessage::CONNECTION_CLOSE_FRAME) {
            ws->close();
        } else if (opcode == restbed::WebSocketMessage::BINARY_FRAME) {
            // We don't support binary data.
            auto response = std::make_shared<restbed::WebSocketMessage>(
                restbed::WebSocketMessage::CONNECTION_CLOSE_FRAME, 
                restbed::Bytes( { 10, 03 } )
            );
            ws->send(response);
        } else if (opcode == restbed::WebSocketMessage::TEXT_FRAME) {
            auto response = std::make_shared<restbed::WebSocketMessage>(*msg);
            response->set_mask(0);

            auto msgdata = msg->get_data();
            std::string text;
            if (this->m_client->is_deflate) {
                deflate(msgdata, text);
            } else {
                text = restbed::String::to_string(msgdata);
                TRACE(7, ">>>\n%s\n>>>", text.c_str());
            }

            if (nullptr != this->cb_message) {
                this->cb_message((char *)text.data());
            }

        } else { /* unknown opcode */
           LOGFILE(LOG_WARN, "ws[%s]: unknown opcode %d", ws->get_key().data(), (int)opcode);
        }
    });

    LOGFILE(LOG_INFO, "ws[%s] started", socket->get_key().c_str());
}

void WebSocketClient::stop()
{
    if (m_client->is_start) {
        auto socket = m_client->m_socket;
        LOGFILE(LOG_DEBUG, "ws[%s] stopping...", socket->get_key().c_str());

        m_client->stop();

        LOGFILE(LOG_INFO, "ws[%s] stopped", socket->get_key().c_str());
    }
}

void WebSocketClient::send(const std::string &cmd)
{
    auto socket = m_client->m_socket;
    LOGFILE(LOG_DEBUG, "ws[%s] emit command %s", socket->get_key().c_str(), cmd.c_str());

    socket->send(cmd, [](const std::shared_ptr<restbed::WebSocket> &ws) {
        LOGFILE(LOG_DEBUG, "ws[%s] command emitted", ws->get_key().data());
    });
}

void WebSocketClient::subscribe(const std::string channel, std::string &parameter)
{
    // multiplexing 
    // TODO
}

void WebSocketClient::unsubscribe(std::string channel)
{
    // multiplexing 
    // TODO
}