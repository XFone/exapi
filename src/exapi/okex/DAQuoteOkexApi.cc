/*
 * $Id: $
 *
 * DAQuoteOkexApi implementation
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
#include "quote/DAQuoteOkexApi.h"

using namespace exapi;


/**
 * Internal class implemented DAQuoteOkexApi
 */
class _DAQuoteOkexApiImpl : public DAQuoteOkexApi {
private:
    friend class DAQuoteOkexApi;
    const char      *m_domain;
    DAQuoteOkexSpi  *m_spi;

    _DAQuoteOkexApiImpl() : m_domain(URL_DOMAIN_COM), m_spi(nullptr) {
        // TODO
    }

public:

    int VerifyConnection();

    //------------------- overrides DAQuoteOkexApi ------------------------

    virtual void Dispose() override {
        delete this;
    }

    virtual void Init() override {
    }

    virtual int Join() override {
        // TODO
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return 0;
    }

    //------------------- overrides IQuoteApi -----------------------------

    virtual int ConnServer(const char *slist[], IQuoteSpi *pEventHandler) override {
  //virtual int ConnServer(const char *slist[], DAQuoteOkexSpi *pEventHandler) override {
        assert(nullptr != pEventHandler);
        m_spi = dynamic_cast<DAQuoteOkexSpi *>(pEventHandler);
        if (m_spi == nullptr) {
            assert(0);
        }
        return 0;
    }

    virtual int DisConnServer(const char *addr) override {
        //TODO
        return 0;
    }

    virtual int Subscribe(const char *filter_symbol) override {
        //TODO: use websocket
        return 0;
    }

    virtual int Unsubscribe(const char *filter_symbol) override {
        //TODO: use websocket
        return 0;
    }

    virtual int GetQuote(int quoteType, const char *symbol, 
                         const char *range, size_t max_size) override {
        //TODO: use Rest API
        return 0;
    }
};

int _DAQuoteOkexApiImpl::VerifyConnection()
{
    if (nullptr == m_spi) return -1;
    // TODO
    return 0;
}

//------------------------- DAQuoteOkexApi --------------------------------

DAQuoteOkexApi *DAQuoteOkexApi::CreateApi(const char *dpath)
{
    return new _DAQuoteOkexApiImpl();
}


//------- Spot --------

int DAQuoteOkexApi::GetTicker(const char *symbol)
{
    int res;
    _DAQuoteOkexApiImpl *impl = static_cast<_DAQuoteOkexApiImpl *>(this);

#if 1
    if (0 != (res = impl->VerifyConnection())) {
        return res;
    }
#endif

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, HTTP_API_ticker);

    request->Init()
        .AddParam("symbol", symbol);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQuoteUpdated(HTTP_API_TYPE_TICKER, const_cast<char *>(json.c_str()));
    });
}

int DAQuoteOkexApi::GetDepth(const char *symbol, const char *range, char *merge)
{
    _DAQuoteOkexApiImpl *impl = static_cast<_DAQuoteOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, HTTP_API_depth);

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("size", range)
        .AddParam("merge", merge);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQuoteUpdated(HTTP_API_TYPE_DEPTH, const_cast<char *>(json.c_str()));
    });
}

int DAQuoteOkexApi::GetTrades(const char *symbol, const char *since)
{
    _DAQuoteOkexApiImpl *impl = static_cast<_DAQuoteOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, HTTP_API_trades);

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("since", since);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQuoteUpdated(HTTP_API_TYPE_TRADES, const_cast<char *>(json.c_str()));
    });
}

int DAQuoteOkexApi::GetKline(int quoteType, const char *symbol, const char *range, size_t max_size)
{
    _DAQuoteOkexApiImpl *impl = static_cast<_DAQuoteOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, HTTP_API_kline);

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("type", std::to_string(quoteType))
        .AddParam("size", std::to_string(max_size))
        .AddParam("since", range);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQuoteUpdated(HTTP_API_TYPE_KLINE, const_cast<char *>(json.c_str()));
    });
}

//------- Future --------

int DAQuoteOkexApi::GetFutureTicker(const char *symbol, const char *contract_type)
{
    _DAQuoteOkexApiImpl *impl = static_cast<_DAQuoteOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, HTTP_API_FUTURE_TICKER);

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contract_type);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQuoteUpdated(HTTP_API_TYPE_FUTURE_TICKER, const_cast<char *>(json.c_str()));
    });
}

int DAQuoteOkexApi::GetFutureDepth(const char *symbol, const char *contract_type, const char *range, char *merge)
{
    _DAQuoteOkexApiImpl *impl = static_cast<_DAQuoteOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, HTTP_API_FUTURE_DEPTH);

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contract_type)
        .AddParam("size", range)
        .AddParam("merge",merge);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQuoteUpdated(HTTP_API_TYPE_FUTURE_DEPTH, const_cast<char *>(json.c_str()));
    });
}

int DAQuoteOkexApi::GetFutureTrades(const char *symbol, const char *contract_type)
{
    _DAQuoteOkexApiImpl *impl = static_cast<_DAQuoteOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, HTTP_API_FUTURE_TRADES);

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contract_type);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQuoteUpdated(HTTP_API_TYPE_FUTURE_TRADES, const_cast<char *>(json.c_str()));
    });
}

int DAQuoteOkexApi::GetFutureIndex(const char *symbol)
{
    _DAQuoteOkexApiImpl *impl = static_cast<_DAQuoteOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, HTTP_API_FUTURE_INDEX);

    request->Init()
        .AddParam("symbol", symbol);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQuoteUpdated(HTTP_API_TYPE_FUTURE_INDEX, const_cast<char *>(json.c_str()));
    });
}

int DAQuoteOkexApi::GetFutureKline(int quoteType, const char *symbol, const char *contract_type,
                                   const char *range, size_t max_size)
{
    _DAQuoteOkexApiImpl *impl = static_cast<_DAQuoteOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, HTTP_API_FUTURE_KLINE);

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("type", std::to_string(quoteType))
        .AddParam("contract_type", contract_type)
        .AddParam("size", std::to_string(max_size))
        .AddParam("since", range);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQuoteUpdated(HTTP_API_TYPE_FUTURE_KLINE, const_cast<char *>(json.c_str()));
    });
}


int DAQuoteOkexApi::GetExchangeRate()
{
    _DAQuoteOkexApiImpl *impl = static_cast<_DAQuoteOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, HTTP_API_EXCHANGE_RATE);

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQuoteUpdated(HTTP_API_TYPE_EXCHANGE_RATE, const_cast<char *>(json.c_str()));
    });
}

int DAQuoteOkexApi::GetFutureEstimatedPrice(const char *symbol)
{
    _DAQuoteOkexApiImpl *impl = static_cast<_DAQuoteOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, HTTP_API_FUTURE_ESTIMATED_PRICE);

    request->Init()
        .AddParam("symbol", symbol);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQuoteUpdated(HTTP_API_TYPE_FUTURE_ESTIMATED_PRICE, const_cast<char *>(json.c_str()));
    });
}

int DAQuoteOkexApi::GetFutureHoldAmount(const char *symbol, const char *contract_type)
{
    _DAQuoteOkexApiImpl *impl = static_cast<_DAQuoteOkexApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, HTTP_API_FUTURE_HOLD_AMOUNT);

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("contract_type", contract_type);

    return RestRequest::SendAsync(request, 
      [impl](const shared_ptr<restbed::Request>req, const shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQuoteUpdated(HTTP_API_TYPE_FUTURE_HOLD_AMOUNT, const_cast<char *>(json.c_str()));
    });
}
