/*
 * $Id: $
 *
 * OKCoinApi C++ implementation with C++11 lambda and ASIO features
 *  
 * Copyright (c) 2014-2018 Zerone.IO . All rights reserved.
 *
 * $Log: $
 *
 */

#include <cstring>
#include <sstream>

#include "okcoinapi.h"

void OKCoinApi::SetKey(string api_key, string secret_key)
{
    m_api_key		=	api_key;	
    m_secret_key	=	secret_key;	
}

//现货行情 API
//获取OKCoin最新市场现货行情数据

string OKCoinApi::GetTicker(string &symbol)         //Get /api/v1/ticker	获取OKCoin行情
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_TICKER);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("symbol", symbol);

    return RestRequest::SendSync(request);
}

string OKCoinApi::GetDepth(string &symbol,string &size,string &merge)   //Get /api/v1/depth		获取OKCoin市场深度
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_DEPTH);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("size", size)
        .AddParam("merge", merge);

    return RestRequest::SendSync(request);
}

string OKCoinApi::GetTrades(string &symbol,string &since)			//Get /api/v1/trades	获取OKCoin最近600交易信息
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_TRADES);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("since", since);

    return RestRequest::SendSync(request);
}

string OKCoinApi::GetKline(string &symbol,string &type,string &size,string &since)			//Get /api/v1/kline		获取比特币或莱特币的K线数据
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_KLINE);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("type", type)
        .AddParam("size", size)
        .AddParam("since", since);

    return RestRequest::SendSync(request);
}


//现货交易 API
//用于OKCoin快速进行现货交易

string OKCoinApi::DoUserinfo()				//POST /api/v1/userinfo	获取用户信息
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_USERINFO);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApi::DoTrade(string &symbol,string &type,string &price,string &amount)			//POST /api/v1/trade	下单交易
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_TRADE);

    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("type", type)
        .AddParam("price", price)
        .AddParam("amount", amount)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}


string OKCoinApi::DoTrade_History(string &symbol,string &since)			//POST /api/v1/trade_history				获取OKCoin历史交易信息
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_TRADE_HISTORY);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("since", since)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApi::DoBatch_Trade(string &symbol,string &type,string &orders_data)	//POST /api/v1/batch_trade批量下单
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_BATCH_TRADE);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("type", type)
        .AddParam("orders_data", orders_data)
        .Sign(m_secret_key);


    return RestRequest::SendSync(request);
}

string OKCoinApi::DoCancel_Order(string &symbol,string &order_id)			//POST /api/v1/cancel_order撤销订单
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_CANCEL_ORDER);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("order_id", order_id)
        .Sign(m_secret_key);


    return RestRequest::SendSync(request);
}

string OKCoinApi::DoOrder_Info(string &symbol,string &order_id)			//POST /api/v1/order_info获取用户的订单信息
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_ORDER_INFO);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("order_id", order_id)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApi::DoOrders_Info(string &type,string &symbol,string &order_id)		//POST /api/v1/orders_info批量获取用户订单
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_ORDERS_INFO);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("type", type)
        .AddParam("symbol", symbol)
        .AddParam("order_id", order_id)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApi::DoOrder_History(string &symbol,string &status,string &current_page,string &page_length)				//POST /api/v1/order_history				获取历史订单信息，只返回最近七天的信息
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_ORDER_HISTORY);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("status",status)
        .AddParam("current_page", current_page)
        .AddParam("page_length", page_length)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApi::DoWithdraw(string &symbol,string &chargefee,string &trade_pwd,string &withdraw_address,string &withdraw_amount)			//POST /api/v1/withdraw	提币BTC/LTC
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_WITHDRAW);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("chargefee", chargefee)
        .AddParam("trade_pwd", trade_pwd)
        .AddParam("withdraw_address",withdraw_address)
        .AddParam("withdraw_amount",withdraw_amount)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}


string OKCoinApi::DoCancel_Withdraw(string &symbol,string &withdraw_id)			//POST /api/v1/cancel_withdraw				取消提币BTC/LTC
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_CANCEL_WITHDRAW);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("withdraw_id",withdraw_id)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApi::DoOrder_Fee(string &symbol,string &order_id)			//POST /api/v1/order_fee查询手续费
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_ORDER_FEE);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("order_id", order_id)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApi::DoLend_Depth(string &symbol)				//POST /api/v1/lend_depth获取放款深度前10
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_LEND_DEPTH);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApi::DoBorrows_Info(string &symbol)				//POST /api/v1/borrows_info查询用户借款信息
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_BORROWS_INFO);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApi::DoBorrow_Money(string &symbol,string &days,string &amount,string &rate)			//POST /api/v1/borrow_money申请借款
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_BORROW_MONEY);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("days", days)
        .AddParam("amount", amount)
        .AddParam("rate", rate)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApi::DoCancel_Borrow(string &symbol,string &borrow_id)				//POST /api/v1/cancel_borrow				取消借款申请
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_CANCEL_BORROW);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("borrow_id", borrow_id)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}


string OKCoinApi::DoBorrow_Order_info(string &borrow_id)		//POST /api/v1/borrow_order_info			获取借款订单记录
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_BORROW_ORDER_INFO);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("borrow_id", borrow_id)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApi::DoRepayment(string &borrow_id)				//POST /api/v1/repayment用户还全款
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_REPAYMENT);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("borrow_id", borrow_id)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApi::DoUnrepayments_Info(string &symbol,string &current_page,string &page_length)	//POST /api/v1/unrepayments_info			未还款列表
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_UNREPAYMENTS_INFO);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("current_page", current_page)
        .AddParam("page_length", page_length)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApi::DoAccount_Records(string &symbol,string &type,string &current_page,string &page_length)				//POST /api/v1/account_records				获取用户提现/充值记录
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_ACCOUNT_RECORDS);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("type", type)
        .AddParam("current_page", current_page)
        .AddParam("page_length", page_length)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}



//期货行情 API
//获取OKCoin期货行情数据

string OKCoinApiCom::DoFuture_Ticker(string &symbol,string &contract_type)			//GET /api/v1/future_ticker				获取OKCoin期货行情
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_TICKER);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contract_type);

    return RestRequest::SendSync(request);
}

string OKCoinApiCom::DoFuture_Depth(string &symbol,string &contract_type,string &size,string &merge)	//GET /api/v1/future_depth				获取OKCoin期货深度信息
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_DEPTH);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contract_type)
        .AddParam("size", size)
        .AddParam("merge",merge);

    return RestRequest::SendSync(request);
}

string OKCoinApiCom::DoFuture_Trades(string &symbol,string &contract_type)			//GET /api/v1/future_trades				获取OKCoin期货交易记录信息
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_TRADES);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contract_type);

    return RestRequest::SendSync(request);
}

string OKCoinApiCom::DoFuture_Index(string &symbol)				//GET /api/v1/future_index				获取OKCoin期货指数信息
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_INDEX);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("symbol", symbol);

    return RestRequest::SendSync(request);
}

string OKCoinApiCom::DoExchange_Rate()		//GET /api/v1/exchange_rate				获取美元人民币汇率
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_EXCHANGE_RATE);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init();

    return RestRequest::SendSync(request);
}

string OKCoinApiCom::DoFuture_Estimated_Price(string &symbol)	//GET /api/v1/future_estimated_price	获取交割预估价
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_ESTIMATED_PRICE);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("symbol", symbol);
    return RestRequest::SendSync(request);
}

string OKCoinApiCom::DoFuture_Kline(string &symbol,string &type,string &contract_type,string &size,string &since)	//GET /api/v1/future_kline				获取期货合约的K线数据
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_KLINE);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("type", type)
        .AddParam("contract_type", contract_type)
        .AddParam("size", size)
        .AddParam("since", since);

    return RestRequest::SendSync(request);
}

string OKCoinApiCom::DoFuture_Hold_amount(string &symbol,string &contract_type)		//GET /api/v1/future_hold_amount		获取当前可用合约总持仓量
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_HOLD_AMOUNT);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contract_type);

    return RestRequest::SendSync(request);
}


//期货交易 API
//用于OKCoin快速进行期货交易
string OKCoinApiCom::DoFuture_Userinfo()			//POST /api/v1/future_userinfo			获取OKCoin期货账户信息 （全仓）
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_USERINFO);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApiCom::DoFuture_Position(string &symbol,string &contract_type)			//POST /api/v1/future_position			获取用户持仓获取OKCoin期货账户信息 （全仓）
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_POSITION);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contract_type)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApiCom::DoFuture_Trade(string &symbol,string &contract_type,string &price,string &amount,string &type,string &match_price,string &lever_rate)			//POST /api/v1/future_trade				期货下单
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_TRADE);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contract_type)
        .AddParam("price", price)
        .AddParam("amount", amount)
        .AddParam("type", type)
        .AddParam("match_price", match_price)
        .AddParam("lever_rate", lever_rate)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApiCom::DoFuture_Trades_history(string &symbol,string &date,string &since)			//POST /api/v1/future_trades_history	获取OKCoin期货交易历史
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_TRADES_HISTORY);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("date", date)
        .AddParam("since", since)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}


string OKCoinApiCom::DoFuture_Batch_trade(string &symbol,string &contract_type,string &orders_data,string &lever_rate)	//POST /api/v1/future_batch_trade		批量下单
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_BATCH_TRADE);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contract_type)
        .AddParam("orders_data", orders_data)
        .AddParam("lever_rate", lever_rate)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApiCom::DoFuture_Cancel(string &symbol,string &order_id,string &contract_type)				//POST /api/v1/future_cancel			取消期货订单
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_CANCEL);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("order_id", order_id)
        .AddParam("contract_type", contract_type)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}
string OKCoinApiCom::DoFuture_Order_info(string &symbol,string &contract_type,string &status,string &order_id,string &current_page,string &page_length)	//POST /api/v1/future_order_info		获取期货订单信息
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_ORDER_INFO);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contract_type)
        .AddParam("status", status)
        .AddParam("order_id", order_id)
        .AddParam("current_page", current_page)
        .AddParam("page_length", page_length)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApiCom::DoFuture_Orders_info(string &symbol,string &contract_type,string &order_id)			//POST /api/v1/future_orders_info		批量获取期货订单信息
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_ORDERS_INFO);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contract_type)
        .AddParam("order_id", order_id)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);	
}

string OKCoinApiCom::DoFuture_Userinfo_4fix()	//POST /api/v1/future_userinfo_4fix		获取逐仓期货账户信息	
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_USERINFO_4FIX);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}

string OKCoinApiCom::DoFuture_Position_4fix(string &symbol,string &contract_type,string &type)			//POST /api/v1/future_position_4fix		逐仓用户持仓查询
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_POSITION_4FIX);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contract_type)
        .AddParam("type", type)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}
    
string OKCoinApiCom::DoFuture_Explosive(string &symbol,string &contract_type,string &status,string &current_page,string &page_length)		//POST /api/v1/future_explosive			获取期货爆仓单
{
    Uri uri = urlprotocol.GetUrl(HTTP_API_TYPE_FUTURE_EXPLOSIVE);
    auto request = RestRequest::CreateBuilder(uri);

    request->Init()
        .AddParam("api_key", m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contract_type)
        .AddParam("status", status)
        .AddParam("current_page", current_page)
        .AddParam("page_length", page_length)
        .Sign(m_secret_key);

    return RestRequest::SendSync(request);
}