/*
 * Bitmex Websocket API implementation
 *
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 * 
 * $Log: $
 */

#include "BitmexWsApi.h"
#include "JsonUtils.h"

#include <string>

#ifdef USE_OPENSSL
#include <openssl/hmac.h>
#endif

using namespace exapi;

BitmexWsApi::BitmexWsApi(const std::string &api_key, const std::string &secret_key)
  : WebSocketClient(BITMEX_WSS_URL BITMEX_WSS_PATH), 
    m_api_key(api_key), m_secret_key(secret_key)
{
    // m_url = BITMEX_WSS_URL BITMEX_WSS_MUX_PATH;
}

void BitmexWsApi::Emit(const char *op, const char *args[])
{
    std::string cmd("{\"op\":\"");
    
    cmd += op;
    cmd += "\",\"args\":";

    std::string params;
    int n = 0;
    while (nullptr != *args) {
        params += ((n++ != 0) ? ",\"" : "\"");
        params += *args++;
        params += "\"";
    }

    if (n > 1) {
        cmd += "[";
        cmd += params;
        cmd += "]}";
    } else {
        cmd += params;
        cmd += "}";
    }

    WebSocketClient::send(cmd);
}

/**
 * Generates an API signature.
 * A signature is HMAC_SHA256(secret, verb + path + nonce + data), hex encoded.
 * Verb must be uppercased, url is relative, nonce must be an increasing 64-bit integer
 * and the data, if present, must be JSON without whitespace between keys.
 */
std::string bitmex_signature(std::string &api_key, std::string &secret_key,
                             const char *verb, const char *endpoint, 
                             const char *nonce, const char *data = nullptr)
{
    unsigned char digest[32];
    unsigned int  digest_size;

    std::string params(verb);
    params += endpoint;
    params += nonce;
    if (data != nullptr) {
        params += data;
    }

    (void)HMAC(EVP_sha256(), 
               secret_key.c_str(), secret_key.size(),
               (const unsigned char *)params.c_str(), params.size() - 1,
               digest, &digest_size);

    char signature[HMAC_MAX_MD_CBLOCK];
    JsonUtils::to_hexstring(signature, (char *)digest, digest_size);
    return std::string(signature);
}

void BitmexWsApi::Authentication()
{
    struct tm tm;
    time_t utc_time = timegm(&tm);
    std::string expires = std::to_string(utc_time + 5); // # 5s grace period in case of clock skew
    std::string cmd("{\"op\":\"authKeyExpires\",\"args\":[\"");
    cmd += m_api_key;
    cmd += "\",";
    cmd += expires;
    cmd += ",\"";
    cmd += bitmex_signature(m_api_key, m_secret_key, 
                            "GET", BITMEX_WSS_PATH, expires.c_str());
    cmd +=  "\"]}";
    WebSocketClient::send(cmd);
}

void BitmexWsApi::CancelAllAfter(int64_t timeout)
{
    std::string cmd("{\"op\":\"cancelAllAfter\",\"args\":");
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