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

        /**
         * Send operator with arguments
         *   {\"op\": string, \"args\": Array<string>}
         * See https://www.bitmex.com/app/wsAPI and https://www.bitmex.com/explorer 
         * for more documentation
         * @param op "authKey", "authKeyExpires", "cancelAllAfter", "subscribe"
         * @param args array of c-string
         */
        void Emit(const char *op, const char *args[]);
    
        /**
         * Subscribe to topics 
         * @param subjects array of subjects (topics)
         *        authenticationRequired: "privateNotifications", "account", 
         *          "wallet", "affiliate", "margin", "position", "transact", 
         *          "order", "execution"
         *        public: "announcement", "connected", "chat", "publicNotifications",
         *          "instrument", "settlement", "funding", "insurance", "liquidation",
         *          "orderBookL2", "orderBookL2_25", "orderBook10", "quote",
         *          "trade", "quoteBin1m", "quoteBin5m", "quoteBin1h", "quoteBin1d",
         *          "tradeBin1m", "tradeBin5m", "tradeBin1h", "tradeBin1d"
         */
        void Subscribe(const char *subjects[]) {
            Emit("subscribe", subjects);
        }

    };

} // namespace exapi