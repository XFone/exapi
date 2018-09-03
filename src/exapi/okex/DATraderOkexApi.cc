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

//------------------------- DATraderOkexSpi -------------------------------

int DATraderOkexSpi::OnResponse(int cmdType, void *pRespData)
{
    int res = 0;
    switch (cmdType) {
    case HTTP_API_TYPE_USERINFO:
        OnUserInfo(pRespData);
        break;

    default:
        LOGFILE(LOG_INFO, "DATraderOkexSpi::OnResponse unknown command %d", cmdType);
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
        impl->m_spi->OnResponse(HTTP_API_TYPE_USERINFO, const_cast<char *>(json.c_str()));
    });
}

int DATraderOkexApi::DoTrade(const char *symbol, const char *type, price_t price, amount_t amount)
{
    return 0;
}

int DATraderOkexApi::DoBatchTrade(const char *symbol, const char *type, const char *ordersdata)
{
    return 0;
}

int DATraderOkexApi::DoCancelOrder(const char *symbol, const char *orderid)
{
    return 0;
}

int DATraderOkexApi::GetOrderInfo(const char *symbol, const char *orderid)
{
    return 0;
}

int DATraderOkexApi::GetOrdersInfo(const char *type, const char *symbol, const char *orderid)
{
    return 0;
}

int DATraderOkexApi::DoWithdraw(const char *symbol, price_t chargefee, const char *tradepwd,
                                const char *withdrawAddress, amount_t withdrawAmount)
{
    return 0;
}

int DATraderOkexApi::DoCancelWithdraw(const char *symbol, const char *withdrawid)
{
    return 0;
}

int DATraderOkexApi::GetOrderFee(const char *symbol,const char *orderid)
{
    return 0;
}

int DATraderOkexApi::GetLendDepth(const char *symbol)
{
    return 0;
}

int DATraderOkexApi::GetBorrowsInfo(const char *symbol)
{
    return 0;
}

int DATraderOkexApi::DoBorrowMoney(const char *symbol, const char *days, amount_t amount, price_t rate)
{
    return 0;
}

int DATraderOkexApi::DoCancelBorrow(const char *symbol, const char *borrowid)
{
    return 0;
}

int DATraderOkexApi::GetBorrowOrderinfo(const char *borrowid)
{
    return 0;
}

int DATraderOkexApi::DoRepayment(const char *borrowid)
{
    return 0;
}

int DATraderOkexApi::GetUnrepaymentsInfo(const char *symbol, size_t currentpage, size_t pagelength)
{
    return 0;
}

int DATraderOkexApi::GetAccountRecords(const char *symbol, const char *type, 
                                       size_t currentpage, size_t pagelength)
{
    return 0;
}

//-------- Spot Trade History ---------

int DATraderOkexApi::GetTradeHistory(const char *symbol, const char *range)
{
    return 0;
}

int DATraderOkexApi::GetOrderHistory(const char *symbol, const char *status,
                                     size_t currentpage, size_t pagelength)
{
    return 0;
}

//------- Future --------

int DATraderOkexApi::GetFutureUserinfo()
{
    return 0;
}

int DATraderOkexApi::GetFuturePosition(const char *symbol, const char *contracttype)
{
    return 0;
}

int DATraderOkexApi::DoFutureTrade(const char *symbol, const char *contracttype, 
                                   price_t price, amount_t amount,
                                   const char *type, price_t matchprice, price_t leverrate)
{
    return 0;
}

int DATraderOkexApi::DoFutureBatchtrade(const char *symbol, const char *contracttype,
                                        const char *ordersdata, price_t leverrate)
{
    return 0;
}

int DATraderOkexApi::DoFutureCancel(const char *symbol, const char *orderid, const char *contracttype)
{
    return 0;
}

int DATraderOkexApi::GetFutureOrderinfo(const char *symbol, const char *contracttype, const char *status,
                                        const char *orderid, 
                                        size_t currentpage, size_t pagelength)
{
    return 0;
}

int DATraderOkexApi::GetFutureOrdersinfo(const char *symbol, const char *contracttype, const char *orderid)
{
    return 0;
}

int DATraderOkexApi::GetFutureUserinfo4fix()
{
    return 0;
}

int DATraderOkexApi::GetFuturePosition4fix(const char *symbol, const char *contracttype, const char *type)
{
    return 0;
}

int DATraderOkexApi::GetFutureExplosive(const char *symbol, const char *contracttype, const char *status,
                                        size_t currentpage, size_t pagelength)
{
    return 0;
}

//------- Future Trade History ---------

int DATraderOkexApi::GetFutureTradeHistory(const char *symbol, const char *date, const char *range)
{
    return 0;
}