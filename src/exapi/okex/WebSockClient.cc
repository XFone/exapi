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

#include "WebSockClient.h"
using namespace exapi;

void WebSocketClient::start() {

    set_open_handler([this](const std::shared_ptr<restbed::WebSocket> &ws) {
        const std::string key = ws->get_key();
        LOGFILE(LOG_INFO, "wss[%s]: opened connection", key.data());

        if (nullptr != this->cb_open) this->cb_open();
    });

    set_close_handler([this](const std::shared_ptr<restbed::WebSocket> &ws) {        
        if (ws->is_open()) {
            auto response = std::make_shared<restbed::WebSocketMessage>(
                restbed::WebSocketMessage::CONNECTION_CLOSE_FRAME,
                restbed::Bytes( { 10, 00 })
            );
            ws->send(response);
        }

        LOGFILE(LOG_INFO, "wss[%s]: closed connection", ws->get_key().data());

        if (nullptr != this->cb_close) this->cb_close();
    });

    set_error_handler([](const std::shared_ptr<restbed::WebSocket> &ws, 
                         const std::error_code &ec) {
        LOGFILE(LOG_WARN, "wss[%s]: error - %s", 
                ws->get_key().data(), ec.message().data());
    });

    set_message_handler([this](const std::shared_ptr<restbed::WebSocket> &ws, 
                               const std::shared_ptr<restbed::WebSocketMessage> &msg) {

        const auto opcode = msg->get_opcode();
    
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
            TRACE(7, ">>>[%s]>>>\n%s\n>>>", ws->get_key().data(), msgdata.data());

            if (nullptr != this->cb_message) {
                this->cb_message((char *)msgdata.data());
            }

        } else { /* unknown opcode */
           LOGFILE(LOG_WARN, "wss[%s]: unknown opcode %d", ws->get_key().data(), (int)opcode);
        }
    });

}