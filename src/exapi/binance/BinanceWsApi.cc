/*
 * Binance Websocket API implementation
 *
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 * 
 * $Log: $
 */

#include "BinanceWsApi.h"
#include "JsonUtils.h"

using namespace exapi;

BinanceWsApi::BinanceWsApi(const std::string &api_key, const std::string &secret_key)
  : WebSocketClient(BINANCE_WSS_URL), m_api_key(api_key), m_secret_key(secret_key)
{
    m_url += BINANCE_WSS_RAW_PATH;
}
