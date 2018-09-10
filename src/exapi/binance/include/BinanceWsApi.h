/*
 * Binance Websocket API C++
 *
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 * 
 * $Log: $
 */

#pragma once

/** @file BinanceWsApi.h Binance websocket client api.
 */

#include "BinanceApi.h"
#include "WebSockClient.h"

#include <memory>

namespace exapi {

    class BinanceWsApi : public WebSocketClient {
    protected:
        std::string                         m_api_key; 
        std::string                         m_secret_key;

    public:
        BinanceWsApi(const std::string &api_key, const std::string &secret_key);

        void SetUrl(const std::string &url) {
            m_url = url;
        }

        virtual ~BinanceWsApi() {}

        //-------------------------- Commands -------------------------------

    };

} // namespace exapi