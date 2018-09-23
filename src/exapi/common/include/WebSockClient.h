/*
 * $Id: $
 * 
 * WebSocketClient class declaration
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file WebSockClient.h Wrap class of WebSocketClientImpl.
 */

#include <string>
#include <chrono>
#include <vector>
#include <memory>

namespace exapi {

    class WebSocketClientImpl;

    typedef void (*ws_callback_open)();
    typedef void (*ws_callback_close)();
    typedef void (*ws_callback_message)(const char *pmsg);

    /**
     * WebSocketClient
     * Wrap class of restbed::WebSocket to handling wss messages:
     */
    class WebSocketClient {
    protected:
        std::string          m_url;
        ws_callback_open     cb_open;
        ws_callback_close    cb_close;
        ws_callback_message  cb_message;

        /** pointer to implementation class */
        std::unique_ptr<WebSocketClientImpl>  m_client;
  
        /** time_point for latency management */
        std::chrono::steady_clock::time_point m_sent_time;
 
        // WebSocketClient() = delete;
 
    public:
        WebSocketClient(const std::string url);

        virtual ~WebSocketClient();

        void start();

        void stop();

        void set_open_callback(ws_callback_open callback) {
            this->cb_open = callback;
        }

        void set_close_callback(ws_callback_close callback) {
            this->cb_close = callback;
        }

        void set_message_callback(ws_callback_message callback) {
            this->cb_message = callback;
        }

        /**
         * Send raw command according to server
         */
        void send(const std::string &command);

        /**
         * subscribe to emit/event channel
         * @param channel name of channel
         */
        void subscribe(const std::string channel) {
            std::string parameter;
            subscribe(channel, parameter);
        }

        /**
         * subscribe to emit/event channel
         * @param channel name of channel
         * @param parameter parameters in json
         */
        void subscribe(const std::string channel, std::string &parameter);

        /**
         * Unsubscribe to emit/event channel
         * @param channel name of channel
         */
        void unsubscribe(const std::string channel);

    };

} // namespace exapi