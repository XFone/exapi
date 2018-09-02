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

/** @file WebSockClient.h Wrap class of restbed::Request.
 */

#include "wshelp.h"

namespace exapi {

    class WebSocketClientImpl;

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

        std::unique_ptr<WebSocketClientImpl> m_client;
        // WebSocketClient() = delete;

    public:
        WebSocketClient(const std::string url);

        ~WebSocketClient();

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
         * subscribe to emit/event channel
         * @param channel name of channel
         */
        void emit(const std::string channel) {
            std::string parameter;
            emit(channel, parameter);
        }

        /**
         * subscribe to emit/event channel
         * @param channel name of channel
         * @param parameter parameters in json
         */
        void emit(const std::string channel, std::string &parameter);

        /**
         * Unsubscribe to emit/event channel
         * @param channel name of channel
         */
        void remove(const std::string channel);

    };

    /**
     * OKex websocket emit parameter builder
     */
    class ParameterBuilder {
    protected:
        std::vector< std::pair<std::string, std::string> > m_params;

    public:
        ParameterBuilder() : m_params() {}

        ParameterBuilder &AddParam(const char *name, const std::string &val) {
            std::pair<std::string, std::string> param(name, val);
            m_params.push_back(param);
            return *this;
        }

        /**
         * generate parameter in json format
         */
        std::string build();

        /**
         * generate parameter in json format, with signature field
         * @param secret secret key
         */
        std::string buildSign(const std::string &secret);

    };
} // namespace exapi