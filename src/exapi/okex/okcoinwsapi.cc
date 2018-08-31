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
        pWebsocket->emit(channel, parameter);
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
        pWebsocket->emit(channel);
    }
}

void OKCoinWebSocketApi::Remove(string channel)
{
    if (pWebsocket != nullptr) {
        pWebsocket->remove(channel);
    }
}

void OKCoinWebSocketApi::Run()
{
    assert(pWebsocket != nullptr);
    pWebsocket->start();
}

/*
unsigned __stdcall OKCoinWebSocketApi::RunThread( LPVOID arg )
{
    if(arg != 0)
    {
        OKCoinWebSocketApi *api = (OKCoinWebSocketApi *)arg;
        
        for(int i = 0;i < MAX_RETRY_COUNT;i++)
        {
            if(api->pWebsocket != 0)
            {
                api->pWebsocket->doclose();
            }
            
            if(api->pWebsocket == 0)
            {
                api->pWebsocket = new WebSocket();
            }

            if(api->pWebsocket != 0)
            {
                api->pWebsocket->callbak_open = api->m_callbak_open;
                api->pWebsocket->callbak_close = api->m_callbak_close;
                api->pWebsocket->callbak_message = api->m_callbak_message;
                api->pWebsocket->run(api->m_uri);
                bool bManualClose = api->pWebsocket->m_manual_close;
                delete api->pWebsocket;
                api->pWebsocket = 0;
                if(bManualClose == false)//是否为主动关闭连接，如果不是用户主动关闭，当接到断开联接回调时则自动执行重新连接机制。
                {
                    Sleep(2000);
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
    }
    //::SetEvent(Global::g_hExit);

    return 0;
}
*/

//////////////////////////////////////////////////////////////////////////////////////////////

//获取OKCoin现货行情数据
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

// 用OKCoin进行现货交易
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

//////////////////////////////////////////////////////////////////////////////////

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