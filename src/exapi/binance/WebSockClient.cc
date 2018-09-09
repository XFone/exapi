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

#ifdef USE_OPENSSL
#include <openssl/md5.h>
#endif

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

        static std::string assign_key() {
            // TODO
            return std::string("UxbkqrAEToZeKbwSTU0TCg==");
        }

        static void set_websocket_headers(const std::string &url, const std::shared_ptr<restbed::Request> &req) {
            std::string host_port(req->get_host()); 
            host_port += ":"; 
            host_port += std::to_string(req->get_port());
            
            req->set_protocol((url.find("wss:") == std::string::npos) ? "HTTP" : "HTTPS");
            //req->set_host("localhost");
            //req->set_port(1984);
            //req->set_path("/chat");
            req->set_header("Host", host_port);
            req->set_header("Connection", "Upgrade");
            req->set_header("Pragma", "no-cache");
            req->set_header("Cache-Control", "no-cache");
            req->set_header("Upgrade", "websocket");
            req->set_header("Sec-WebSocket-Version", "13");
            req->set_header("Sec-WebSocket-Key", assign_key());
            req->set_header("Sec-WebSocket-Extensions", "permessage-deflate; client_max_window_bits");
        }

    public:
        WebSocketClientImpl() : is_start(false) {}
        
        ~WebSocketClientImpl() {}

        /**
         * Start websocket connection and io processing
         * @param url websocket server address, e.g.m "ws://localhost:1984/chat"
         */
        void connect(std::string url) {
            if (nullptr == m_request) {
                m_request = std::make_shared<restbed::Request>(restbed::Uri(url));
                set_websocket_headers(url, m_request);
            }

            // init socket and tls settings
            // set_socket_init_handler(bind(&type::on_socket_init,this,::_1));
            // set_tls_init_handler(bind(&type::on_tls_init,this,::_1));
            // set_open_handshake_timeout(3000);
            // set_close_handshake_timeout(3000);

            TRACE(7, "<<<\n%s\n<<<", 
                  restbed::String::to_string(restbed::Http::to_bytes(m_request)).c_str());

            auto response = restbed::Http::sync(m_request);

            TRACE(7, ">>>\n%s\n>>>", 
                  restbed::String::to_string(restbed::Http::to_bytes(response)).c_str());

            if (response->get_status_code() != restbed::SWITCHING_PROTOCOLS ||
                response->get_header("upgrade", restbed::String::lowercase ) != "websocket") {
                LOGFILE(LOG_ERROR, "invalid response status in websocket");
                return;
            }

            if (nullptr == m_ws_mgnr) {
                m_ws_mgnr = std::make_shared<restbed::detail::WebSocketManagerImpl>();
            }

            m_socket = m_ws_mgnr->create(m_request);
            m_socket->set_logger(LoggerImpl::GetInstance());
        }

        void start() {
            m_worker = std::make_shared<std::thread>([this]() {
                LOGFILE(LOG_INFO, "websocket worker starting...");

                while (this->m_socket->is_open()) {
                    this->m_ws_mgnr->process_io(this->m_request);
                    // TODO sleep
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }

                LOGFILE(LOG_INFO, "websocket worker exiting...");
            });
            // this->m_ws_mgnr->process_io(this->m_request);
            is_start = true;
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

/*---------------------------- WebSocketClient -----------------------------*/

WebSocketClient::WebSocketClient(const std::string url)
  : m_url(url), cb_open(nullptr), cb_close(nullptr), cb_message(nullptr),
    m_client(new WebSocketClientImpl)
{
    // m_url = "ws://localhost:1984/chat"; // TESTING
    // to check this:
    //m_pimpl->m_manager = make_shared<restbed::detail::WebSocketManagerImpl>();
    //m_client = new WebSocketClientImpl();
}

WebSocketClient::~WebSocketClient() {
    stop();
    // delete m_client;
};

void WebSocketClient::start()
{
    LOGFILE(LOG_DEBUG, "server '%s'...", m_url.c_str());

    m_client->connect(m_url);

    auto socket = m_client->m_socket;

    LOGFILE(LOG_DEBUG, "websocket is %s", socket->is_open() ? "open" : "closed");

    // open_handler
    socket->set_open_handler([this](const std::shared_ptr<restbed::WebSocket> &ws) {
        LOGFILE(LOG_INFO, "ws[%s] opened connection", ws->get_key().data());

        if (nullptr != this->cb_open) this->cb_open();
    });

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
    socket->set_error_handler([](const std::shared_ptr<restbed::WebSocket> &ws, 
                         const std::error_code &ec) {
        LOGFILE(LOG_WARN, "ws[%s] error - %s", 
                ws->get_key().data(), ec.message().data());
        if (ws->is_closed()) {
            // TODO: re-connecting
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
            //We don't support binary data.
            auto response = std::make_shared<restbed::WebSocketMessage>(
                restbed::WebSocketMessage::CONNECTION_CLOSE_FRAME, 
                restbed::Bytes( { 10, 03 } )
            );
            ws->send(response);
        } else if (opcode == restbed::WebSocketMessage::TEXT_FRAME) {
            auto response = std::make_shared<restbed::WebSocketMessage>(*msg);
            response->set_mask(0);

            auto msgdata = msg->get_data();
            TRACE(7, ">>>[%s]>>>\n%s\n>>>", ws->get_key().data(), 
                  restbed::String::to_string(msgdata).c_str());

            if (nullptr != this->cb_message) {
                this->cb_message((char *)msgdata.data());
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

void WebSocketClient::emit(const std::string channel, std::string &parameter)
{
    auto socket = m_client->m_socket;

    std::string cmd("{'event':'addChannel','channel': '");
    
    cmd += channel;
    if (parameter.size() > 0) {
        cmd += "','parameters':";
        cmd += parameter;
    }
    cmd +=  "'}";

    LOGFILE(LOG_DEBUG, "ws[%s] emit command %s", socket->get_key().c_str(), cmd.c_str());

    socket->send(cmd, [channel](const std::shared_ptr<restbed::WebSocket> &ws) {
        LOGFILE(LOG_DEBUG, "ws[%s] channel '%s' emitted", 
                ws->get_key().data(), channel.c_str());
    });
    //send("{'event':'addChannel','channel':'ok_btcusd_ticker'}");
    //send("{'event':'addChannel','channel':'ok_btcusd_depth'}");
}

void WebSocketClient::remove(std::string channel)
{
    auto socket = m_client->m_socket;

    std::string cmd("{'event':'removeChannel','channel':'");
    cmd += channel;
    cmd += "'}";

    LOGFILE(LOG_DEBUG, "ws[%s] remove command %s", socket->get_key().c_str(), cmd.c_str());

    socket->send(cmd, [channel](const std::shared_ptr<restbed::WebSocket> &ws) {
        LOGFILE(LOG_DEBUG, "ws[%s] channel '%s' removed", 
                ws->get_key().data(), channel.c_str());
    });
}

/*--------------------------- ParameterBuilder -----------------------------*/

std::string ParameterBuilder::build() {
    std::string result("{");
    int n = 0;
    for (auto param : m_params) {
        result += ((n++ != 0) ? ",'" : "'");
        result += param.first;
        result += "':'";
        result += param.second;
        result += "'";
    }
    result += "}";
    return result;
}

std::string ParameterBuilder::buildSign(const std::string &secret) {
    std::string params;    // for generating md5 in HTTP request format
    unsigned char md[MD5_DIGEST_LENGTH];
    char strMd5[2 * MD5_DIGEST_LENGTH + 1];

    std::string result("{");
    int n = 0;
    for (auto param : m_params) {
        result += ((n++ != 0) ? ",'" : "'");
        result += param.first;
        result += "':'";
        result += param.second;
        result += "'";

        // query params for signing
        params += param.first;
        params += "=";
        params += param.second;
        params += "&";
    }
    
    params += "secret_key=";
    params += secret;
    
    (void)MD5((const unsigned char *)params.c_str(), params.size() - 1, md);
    for (int n = 0; n < MD5_DIGEST_LENGTH; n++) {
        sprintf(&strMd5[n << 1], "%02X", md[n]);    // output in upper case
    }

    // append signature
    result += "'sign':'";
    result += strMd5;
    result += "'}";
    return result;
}