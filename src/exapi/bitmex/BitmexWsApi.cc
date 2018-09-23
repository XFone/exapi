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
  : WebSocketClient(BITMEX_WSS_URL BITMEX_WSS_PATH), 
    m_api_key(api_key), m_secret_key(secret_key)
{
    // m_url = BITMEX_WSS_URL BITMEX_WSS_MUX_PATH;
}

void BitmexWsApi::Emit(const char *op, const char *args[])
{
    std::string cmd("{'op':'");
    
    cmd += op;

    std::string params("[");
    int n = 0;
    while (nullptr != *args) {
        params += ((n++ != 0) ? ",'" : "'");
        params += *args++;
        params += "'";
    }
    params += "]";
    cmd += "','args':";
    cmd += params;
    cmd +=  "}";

    WebSocketClient::send(cmd);
}

void BitmexWsApi::Authentication()
{
    // TODO
}

void BitmexWsApi::CancelAllAfter(int64_t timeout)
{
    std::string cmd("{'op':'cancelAllAfter', 'args': ");
    cmd += std::to_string(timeout);
    cmd +=  "}";
    WebSocketClient::send(cmd);
}

//------------------------ Multiplexing -----------------------------
// m_url = BITMEX_WSS_URL BITMEX_WSS_MUX_PATH;

void BitmexWsApi::Emit(const char *channel, const char *topic, const char *payload)
{
    // TODO
}

void BitmexWsApi::Subscribe(const char *channel, const char *subjects[])
{
    // TODO
}

void BitmexWsApi::Unsubscribe(const char *channel, const char *subjects[])
{
    // TODO
}