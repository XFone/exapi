/*
 * Bitmex Websocket API C++
 *
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 * 
 * $Log: $
 */

#pragma once

/** @file BitmexWsApi.h Bitmex websocket client api.
 */

#include "BitmexApi.h"
#include "WebSockClient.h"

#include <memory>

namespace exapi {

    class BitmexWsApi : public WebSocketClient {
    protected:
        std::string                         m_api_key; 
        std::string                         m_secret_key;

    public:
        BitmexWsApi(const std::string &api_key, const std::string &secret_key);

        void SetUrl(const std::string &url) {
            m_url = url;
        }

        virtual ~BitmexWsApi() {}

        //-------------------------- Commands -------------------------------

    };

} // namespace exapi