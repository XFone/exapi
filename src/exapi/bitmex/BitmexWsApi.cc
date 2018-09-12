/*
 * Bitmex Websocket API implementation
 *
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 * 
 * $Log: $
 */

#include "BitmexWsApi.h"

using namespace exapi;

BitmexWsApi::BitmexWsApi(const std::string &api_key, const std::string &secret_key)
  : WebSocketClient(BITMEX_WSS_URL), m_api_key(api_key), m_secret_key(secret_key)
{
    m_url += BITMEX_WSS_PATH;
}
