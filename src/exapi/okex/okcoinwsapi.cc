/*
 * $Id: $
 *
 * OKCoinWebSocketApi C++ implementation with C++11 lambda and ASIO features
 *  
 * Copyright (c) 2014-2018 Zerone.IO . All rights reserved.
 *
 * $Log: $
 *
 */

#include <cstring>
#include <sstream>
#include <vector>

#include "okcoinwsapi.h"
#include "WebSockClient.h"
using namespace exapi;

#ifdef USE_OPENSSL
#include <openssl/md5.h>
#endif

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

std::string ParameterBuilder::build() {
    std::string result("{");
    int n = 0;
    for (auto param : m_params) {
        result += ((n++ != 0) ? ",'" : "'");
        result += param.first;
        result += "':'";
        result += param.second;
        result += "'";
    }
    result += "}";
    return result;
}

std::string ParameterBuilder::buildSign(const std::string &secret) {
    std::string params;    // for generating md5 in HTTP request format
    unsigned char md[MD5_DIGEST_LENGTH];
    char strMd5[2 * MD5_DIGEST_LENGTH + 1];

    std::string result("{");
    int n = 0;
    for (auto param : m_params) {
        result += ((n++ != 0) ? ",'" : "'");
        result += param.first;
        result += "':'";
        result += param.second;
        result += "'";

        // query params for signing
        params += param.first;
        params += "=";
        params += param.second;
        params += "&";
    }
    
    params += "secret_key=";
    params += secret;
    
    (void)MD5((const unsigned char *)params.c_str(), params.size() - 1, md);
    for (int n = 0; n < MD5_DIGEST_LENGTH; n++) {
        sprintf(&strMd5[n << 1], "%02X", md[n]);    // output in upper case
    }

    // append signature
    result += "'sign':'";
    result += strMd5;
    result += "'}";
    return result;
}


OKCoinWebSocketApi::OKCoinWebSocketApi() : pWebsocket(nullptr)
{
    // NOTHING
}

OKCoinWebSocketApi::~OKCoinWebSocketApi()
{
    Close();
}

void OKCoinWebSocketApi::SetKey(string api_key, string secret_key)
{
    m_api_key    = api_key;
    m_secret_key = secret_key;
}

void OKCoinWebSocketApi::SetUri(string uri)
{
    m_uri = uri;
}

void OKCoinWebSocketApi::Close()
{
    if (pWebsocket != nullptr) {
        pWebsocket->stop();
    }
}

void OKCoinWebSocketApi::Emit(const char *channel, string &parameter)
{
    if (pWebsocket != nullptr) {
        pWebsocket->subscribe(channel, parameter);
    }
}

void OKCoinWebSocketApi::Emit(const char *channel)
{
    string ch(channel);
    Emit(ch);
}

void OKCoinWebSocketApi::Emit(string &channel)
{
    if (pWebsocket != nullptr) {
        pWebsocket->subscribe(channel);
    }
}

void OKCoinWebSocketApi::Remove(string channel)
{
    if (pWebsocket != nullptr) {
        pWebsocket->unsubscribe(channel);
    }
}

void OKCoinWebSocketApi::Run()
{
    if (pWebsocket == nullptr) {
        pWebsocket = std::make_shared<WebSocketClient>(m_uri);
    }
    pWebsocket->start();
}

/*------------------------- OKCoinWebSocketApiCn ----------------------------*/


//-------------- 获取OKCoin现货行情数据 --------------

void OKCoinWebSocketApiCn::ok_spotcny_btc_ticker() //比特币行情数据
{
    Emit("ok_sub_spotcny_btc_ticker");
}

void OKCoinWebSocketApiCn::ok_spotcny_btc_depth_20() //比特币20条市场深度
{
    Emit("ok_sub_spotcny_btc_depth_20");
}

void OKCoinWebSocketApiCn::ok_spotcny_btc_trades() //比特币实时成交记录
{
    Emit("ok_sub_spotcny_btc_trades");
}

void OKCoinWebSocketApiCn::ok_spotcny_btc_kline_1min() //比特币K线数据
{
    Emit("ok_sub_spotcny_btc_kline_1min");
}

//--------------- 用OKCoin进行现货交易 ---------------

void OKCoinWebSocketApiCn::ok_spotcny_trades() //订阅交易数据
{
    ParameterBuilder pbld;

    pbld.AddParam("api_key", m_api_key);

    string param = pbld.buildSign(m_secret_key);
    Emit("ok_sub_spotcny_trades", param);
}

void OKCoinWebSocketApiCn::ok_spotcny_trade(string &symbol,string &type,string &price,string &amount) //下单交易
{
    ParameterBuilder pbld;

    pbld.AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("type", type)
        .AddParam("price", price)
        .AddParam("amount", amount);

    string param = pbld.buildSign(m_secret_key);
    Emit("ok_spotcny_trade", param);
    return;
}

void OKCoinWebSocketApiCn::ok_spotcny_cancel_order(string &symbol,string &order_id) //取消订单
{
    ParameterBuilder pbld;

    pbld.AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("order_id", order_id);

    string param = pbld.buildSign(m_secret_key);
    Emit("ok_spotcny_cancel_order", param);
}

//取消订阅
void OKCoinWebSocketApiCn::remove_ok_spotcny_btc_ticker() //比特币行情数据
{
    Remove("ok_sub_spotcny_btc_ticker");
}

/*------------------------- OKCoinWebSocketApiCom ---------------------------*/

//获取OKCoin现货行情数据
void OKCoinWebSocketApiCom::ok_spotusd_btc_ticker() //比特币行情数据
{
    Emit("ok_sub_spotusd_btc_ticker");
}

//获取OKCoin合约行情数据
void OKCoinWebSocketApiCom::ok_futureusd_btc_ticker_this_week() //比特币当周合约行情
{
    Emit("ok_sub_futureusd_btc_ticker_this_week");
}

void OKCoinWebSocketApiCom::ok_futureusd_btc_index() //比特币合约指数
{
    Emit("ok_sub_futureusd_btc_index");
}

//取消订阅
void OKCoinWebSocketApiCom::remove_ok_spotusd_btc_ticker() //比特币行情数据
{
    Remove("ok_sub_spotusd_btc_ticker");
}