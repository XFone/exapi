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

#include "detail/WebSocketClientImpl_restbed.ipp"
using namespace exapi;

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
            TRACE(7, ">>>\n%s\n>>>", restbed::String::to_string(msgdata).c_str());

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

void WebSocketClient::send(const std::string &cmd)
{
    auto socket = m_client->m_socket;
    LOGFILE(LOG_DEBUG, "ws[%s] emit command %s", socket->get_key().c_str(), cmd.c_str());

    socket->send(cmd, [](const std::shared_ptr<restbed::WebSocket> &ws) {
        LOGFILE(LOG_DEBUG, "ws[%s] command emitted", ws->get_key().data());
    });
}

void WebSocketClient::subscribe(const std::string channel, std::string &params)
{
    auto socket = m_client->m_socket;

    std::string cmd("{'event':'addChannel','channel': '");
    
    cmd += channel;
    if (params.size() > 0) {
        cmd += "','parameters':";
        cmd += params;
    }
    cmd +=  "'}";
	
	send(cmd);
}

void WebSocketClient::unsubscribe(std::string channel)
{
    auto socket = m_client->m_socket;

    std::string cmd("{'event':'removeChannel','channel':'");
    cmd += channel;
    cmd += "'}";

	send(cmd);
}
