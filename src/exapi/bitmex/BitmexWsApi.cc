/*
 * Bitmex Websocket API implementation
 *
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 * 
 * $Log: $
 */

#include "BitmexWsApi.h"

#include <string>

using namespace exapi;

BitmexWsApi::BitmexWsApi(const std::string &api_key, const std::string &secret_key)
  : WebSocketClient(BITMEX_WSS_URL), m_api_key(api_key), m_secret_key(secret_key)
{
    m_url += BITMEX_WSS_PATH;
}

void BitmexWsApi::Emit(const char *op, const char *args[])
{
    std::string params("[");
    int n = 0;
    while (nullptr != *args) {
        params += ((n++ != 0) ? ",'" : "'");
        params += *args++;
        params += "'";
    }
    params += "]";

    WebSocketClient::emit(op, params);
}