/*
 * $Id: $
 *
 * DATraderOkexApi implementation
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */

#include "Base.h"
#include "Log.h"

#include "curlprotocol.h"
#include "urlhelp.h"
#include "RestRequest.h"
#include "trade/DATraderOkexApi.h"
using namespace exapi;

#define _OKEX_(api_type)         (int)(TraderApiType::EX_TYPE_OKEX + api_type)

/**
 * Internal class implemented DATraderOkexApi
 */
class _DATraderOkexApiImpl : public DATraderOkexApi {
private:
    friend class DATraderOkexApi;
    const char      *m_domain;
    DATraderOkexSpi *m_spi;
    std::string      m_api_key;
    std::string      m_secret_key;

    _DATraderOkexApiImpl(const char *api_key, const char *secret_key)
      : m_domain(URL_DOMAIN_COM), m_spi(nullptr), 
        m_api_key(api_key), m_secret_key(secret_key) {
        // TODO
    }

public:
    int VerifyConnection();

    //------------------- overrides DATraderOkexApi ------------------------

    virtual void Dispose() override {
        delete this;
    }

    virtual void Init() override {
    }

    virtual int Join() override {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return 0;
    }

    //------------------- Overrides IQuoteApi -----------------------------

    virtual int ConnServer(const char *slist[], ITraderSpi *pEventHandler) override {
  //virtual int ConnServer(const char *slist[], DATraderOkexSpi *pEventHandler) override {
        assert(nullptr != pEventHandler);
        m_spi = dynamic_cast<DATraderOkexSpi *>(pEventHandler);
        if (m_spi == nullptr) {
            assert(0);
        }
        return 0;
    }

    virtual int DisConnServer(const char *addr) override {
        return 0;
    }
    
    virtual int UserLogon(const char *userContext) override {
        // TODO
        return 0;
    }

};

static inline std::string Price2String(price_t price) {
    return std::to_string(price); // = (float)price / algoapi::PRICE_BASE
}

static inline std::string Amount2String(amount_t amount) {
    return std::to_string(amount);
}

//------------------------- DATraderOkexSpi -------------------------------

int DATraderOkexSpi::OnResponse(TraderApiType apiType, void *pRespData)
{
    int res = 0;
    int api = (int)apiType - TraderApiType::EX_TYPE_OKEX;

    switch (api) {
    case HTTP_API_TYPE_USERINFO:
        OnUserInfo(pRespData);
        break;

    case HTTP_API_TYPE_TRADE:
        //TODO
        break;

    case HTTP_API_TYPE_TRADE_HISTORY:
    case HTTP_API_TYPE_BATCH_TRADE:
    case HTTP_API_TYPE_CANCEL_ORDER:
    case HTTP_API_TYPE_ORDER_INFO:
    case HTTP_API_TYPE_ORDERS_INFO:
    case HTTP_API_TYPE_ORDER_HISTORY:
    case HTTP_API_TYPE_WITHDRAW:
    case HTTP_API_TYPE_CANCEL_WITHDRAW:
    case HTTP_API_TYPE_ORDER_FEE:
    case HTTP_API_TYPE_LEND_DEPTH:
    case HTTP_API_TYPE_BORROWS_INFO:
    case HTTP_API_TYPE_BORROW_MONEY:
    case HTTP_API_TYPE_CANCEL_BORROW:
    case HTTP_API_TYPE_BORROW_ORDER_INFO:
    case HTTP_API_TYPE_REPAYMENT:
    case HTTP_API_TYPE_UNREPAYMENTS_INFO:
    case HTTP_API_TYPE_ACCOUNT_RECORDS:

    case HTTP_API_TYPE_FUTURE_USERINFO:
    case HTTP_API_TYPE_FUTURE_POSITION:
    case HTTP_API_TYPE_FUTURE_TRADE:
    case HTTP_API_TYPE_FUTURE_TRADES_HISTORY:
    case HTTP_API_TYPE_FUTURE_BATCH_TRADE:
    case HTTP_API_TYPE_FUTURE_CANCEL:
    case HTTP_API_TYPE_FUTURE_ORDER_INFO:
    case HTTP_API_TYPE_FUTURE_ORDERS_INFO:
    case HTTP_API_TYPE_FUTURE_USERINFO_4FIX:
    case HTTP_API_TYPE_FUTURE_POSITION_4FIX:
    case HTTP_API_TYPE_FUTURE_EXPLOSIVE:
    
    default:
        LOGFILE(LOG_INFO, "DATraderOkexSpi::OnResponse unknown command %d", (int)apiType);
        res = -1; // CONTINUE
        break;
    }

    // release allocated resource of pRespData
    return res;
}

//------------------------- DATraderOkexApi -------------------------------

DATraderOkexApi *DATraderOkexApi::CreateApi(const char *api_key, const char *secret_key, const char *dpath)
{
    return new _DATraderOkexApiImpl(api_key, secret_key);
}


//-------- Spot ---------
        
int DATraderOkexApi::GetUserInfo()
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_USERINFO);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_USERINFO), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::DoTrade(const char *symbol, const char *type, price_t price, amount_t amount)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_TRADE);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("type", type)
        .AddParam("price", Price2String(price))
        .AddParam("amount", Amount2String(amount))
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_TRADE), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::DoBatchTrade(const char *symbol, const char *type, const char *ordersdata)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_BATCH_TRADE);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("type", type)
        .AddParam("orders_data", ordersdata)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_BATCH_TRADE), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::DoCancelOrder(const char *symbol, const char *orderid)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_CANCEL_ORDER);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("order_id", orderid)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_CANCEL_ORDER), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::GetOrderInfo(const char *symbol, const char *orderid)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_ORDER_INFO);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("order_id", orderid)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_ORDER_INFO), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::GetOrdersInfo(const char *type, const char *symbol, const char *orderid)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_ORDERS_INFO);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("type", type)
        .AddParam("symbol", symbol)
        .AddParam("order_id", orderid)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_ORDERS_INFO), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::DoWithdraw(const char *symbol, price_t chargefee, const char *tradepwd,
                                const char *withdrawAddress, amount_t withdrawAmount)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_WITHDRAW);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("chargefee", Price2String(chargefee))
        .AddParam("trade_pwd", tradepwd)
        .AddParam("withdraw_address", withdrawAddress)
        .AddParam("withdraw_amount", Amount2String(withdrawAmount))
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_WITHDRAW), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::DoCancelWithdraw(const char *symbol, const char *withdrawid)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_CANCEL_WITHDRAW);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("withdraw_id", withdrawid)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_CANCEL_WITHDRAW), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::GetOrderFee(const char *symbol, const char *orderid)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_ORDER_FEE);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("order_id", orderid)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_ORDER_FEE), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::GetLendDepth(const char *symbol)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_LEND_DEPTH);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_LEND_DEPTH), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::GetBorrowsInfo(const char *symbol)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_BORROWS_INFO);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_BORROWS_INFO), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::DoBorrowMoney(const char *symbol, const char *days, amount_t amount, price_t rate)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_BORROW_MONEY);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("days", days)
        .AddParam("amount", Amount2String(amount))
        .AddParam("rate", Price2String(rate))
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_BORROW_MONEY), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::DoCancelBorrow(const char *symbol, const char *borrowid)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_CANCEL_BORROW);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("borrow_id", borrowid)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_CANCEL_BORROW), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::GetBorrowOrderinfo(const char *borrowid)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_BORROW_ORDER_INFO);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("borrow_id", borrowid)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_BORROW_ORDER_INFO), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::DoRepayment(const char *borrowid)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_REPAYMENT);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("borrow_id", borrowid)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_REPAYMENT), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::GetUnrepaymentsInfo(const char *symbol, size_t currentpage, size_t pagelength)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_UNREPAYMENTS_INFO);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("current_page", std::to_string(currentpage))
        .AddParam("page_length", std::to_string(pagelength))
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_UNREPAYMENTS_INFO), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::GetAccountRecords(const char *symbol, const char *type, 
                                       size_t currentpage, size_t pagelength)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_ACCOUNT_RECORDS);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("type", type)
        .AddParam("current_page", std::to_string(currentpage))
        .AddParam("page_length", std::to_string(pagelength))
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_ACCOUNT_RECORDS), const_cast<char *>(json.c_str()));
    });
}

//-------- Spot Trade History ---------

int DATraderOkexApi::GetTradeHistory(const char *symbol, const char *range)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_TRADE_HISTORY);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("since", range)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_TRADE_HISTORY), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::GetOrderHistory(const char *symbol, const char *status,
                                     size_t currentpage, size_t pagelength)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_ORDER_HISTORY);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("status", status)
        .AddParam("current_page", std::to_string(currentpage))
        .AddParam("page_length", std::to_string(pagelength))
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_ORDER_HISTORY), const_cast<char *>(json.c_str()));
    });
}

//------- Future --------

int DATraderOkexApi::GetFutureUserinfo()
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_FUTURE_USERINFO);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_FUTURE_USERINFO), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::GetFuturePosition(const char *symbol, const char *contracttype)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_FUTURE_POSITION);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contracttype)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_FUTURE_POSITION), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::DoFutureTrade(const char *symbol, const char *contracttype, 
                                   price_t price, amount_t amount,
                                   const char *type, price_t matchprice, price_t leverrate)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_FUTURE_TRADE);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contracttype)
        .AddParam("price", Price2String(price))
        .AddParam("amount", Amount2String(amount))
        .AddParam("type", type)
        .AddParam("match_price", Price2String(matchprice))
        .AddParam("lever_rate", Price2String(leverrate))
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_FUTURE_TRADE), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::DoFutureBatchtrade(const char *symbol, const char *contracttype,
                                        const char *ordersdata, price_t leverrate)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_FUTURE_BATCH_TRADE);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contracttype)
        .AddParam("orders_data", ordersdata)
        .AddParam("lever_rate", Price2String(leverrate))
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_FUTURE_BATCH_TRADE), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::DoFutureCancel(const char *symbol, const char *orderid, const char *contracttype)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_FUTURE_CANCEL);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("order_id", orderid)
        .AddParam("contract_type", contracttype)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_FUTURE_CANCEL), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::GetFutureOrderinfo(const char *symbol, const char *contracttype, const char *status,
                                        const char *orderid, 
                                        size_t currentpage, size_t pagelength)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_FUTURE_ORDER_INFO);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contracttype)
        .AddParam("status", status)
        .AddParam("order_id", orderid)
        .AddParam("current_page", std::to_string(currentpage))
        .AddParam("page_length", std::to_string(pagelength))
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_FUTURE_ORDER_INFO), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::GetFutureOrdersinfo(const char *symbol, const char *contracttype, const char *orderid)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_FUTURE_ORDERS_INFO);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contracttype)
        .AddParam("order_id", orderid)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_FUTURE_ORDERS_INFO), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::GetFutureUserinfo4fix()
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_FUTURE_USERINFO_4FIX);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_FUTURE_USERINFO_4FIX), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::GetFuturePosition4fix(const char *symbol, const char *contracttype, const char *type)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_FUTURE_POSITION_4FIX);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contracttype)
        .AddParam("type", type)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_FUTURE_POSITION_4FIX), const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::GetFutureExplosive(const char *symbol, const char *contracttype, const char *status,
                                        size_t currentpage, size_t pagelength)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_FUTURE_EXPLOSIVE);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contracttype)
        .AddParam("status", status)
        .AddParam("current_page", std::to_string(currentpage))
        .AddParam("page_length", std::to_string(pagelength))
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_FUTURE_EXPLOSIVE), const_cast<char *>(json.c_str()));
    });
}

//------- Future Trade History ---------

int DATraderOkexApi::GetFutureTradeHistory(const char *symbol, const char *date, const char *range)
{
    _DATraderOkexApiImpl *impl = static_cast<_DATraderOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, HTTP_API_FUTURE_TRADES_HISTORY);

    request->Init()
        .AddParam("api_key", impl->m_api_key)
        .AddParam("symbol", symbol)
        .AddParam("date", date)
        .AddParam("since", range)
        .Sign(impl->m_secret_key);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponse(_OKEX_(HTTP_API_TYPE_FUTURE_TRADES_HISTORY), const_cast<char *>(json.c_str()));
    });
}