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
         * see https://www.bitmex.com/app/wsAPI#Subscriptions
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

        /**
         * Unsubscribe to topics
         * see https://www.bitmex.com/app/wsAPI#Subscriptions
         */
        void Unsubscribe(const char *subjects[]) {
            Emit("unsubscribe", subjects);
        }

        /**
         * For user-locked streams, you must authenticate first.
         */
        void Authentication();

        /**
         * Automatically cancel all your orders after a specified timeout
         * see [Dead Man’s Switch (Auto Cancel)](https://www.bitmex.com/app/wsAPI#Dead-Mans-Switch-Auto-Cancel)
         * see also (REST at /order/cancelAllAfter) 
         * \ref DATraderBitmexApi::CancelOrdersAfter
         * 
         * Advanced users of BitMEX should use this operation. A common use 
         * pattern is to set a timeout of 60000, and call it every 15 seconds. 
         * This gives you sufficient wiggle room to keep your orders open in 
         * case of a network hiccup, while still offering significant protection 
         * in case of a larger outage. Of course, the parameters are up to you.
         * 
         * @param timeout millisecond timeout, To cancel this operation and keep 
         *        your orders open, pass a timeout of 0
         */
        void CancelAllAfter(int64_t timeout = 60000L);

        //------------------------ Multiplexing -----------------------------

        void Emit(const char *channel, const char *topic, const char *payload);

        void Subscribe(const char *channel, const char *subjects[]);

        void Unsubscribe(const char *channel, const char *subjects[]);
    };

    /**
     * Generates an API signature.
     * A signature is HMAC_SHA256(secret, verb + path + nonce + data), hex encoded.
     * Verb must be uppercased, url is relative, nonce must be an increasing 64-bit integer
     * and the data, if present, must be JSON without whitespace between keys.
     */
    std::string BitmexSignature(const std::string &secret_key,
                                const char *verb, const char *path, 
                                const char *nonce, const char *data = nullptr);

} // namespace exapi