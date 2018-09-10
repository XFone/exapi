/*
 * $Id: $
 *
 * DAQuoteBinanceApi implementation
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */

#include "Base.h"
#include "Log.h"
#include "Trace.h"

#include <cerrno>

#include "RestRequest.h"
#include "BinanceApi.h"
#include "JsonUtils.h"
#include "quote/DAQuoteBinanceApi.h"

using namespace exapi;

#define _BINANCE_(api_type)         (int)(QuoteApiType::EX_TYPE_BINANCE + api_type)

/**
 * Internal class implemented DAQuoteBinanceApi
 */
class _DAQuoteBinanceApiImpl : public DAQuoteBinanceApi {
private:
    friend class DAQuoteBinanceApi;
    const char          *m_domain;
    std::string          m_api_key;
    std::string          m_secret_key;
    DAQuoteBinanceSpi   *m_spi;

    _DAQuoteBinanceApiImpl(const char *api_key, const char *secret_key) 
      : m_domain(BINANCE_REST_URL), 
        m_api_key(api_key), m_secret_key(secret_key), m_spi(nullptr) {
        // NOTHING
    }

public:

    //------------------- overrides DAQuoteBinanceApi ----------------------

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
        assert(nullptr != pEventHandler);
        m_spi = dynamic_cast<DAQuoteBinanceSpi *>(pEventHandler);
        if (m_spi == nullptr) {
            assert(0);
        }
        if (nullptr != slist) {
            m_domain = slist[0];    // TODO: try fastest server
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

    virtual int GetQuote(QuoteApiType quoteType, const char *symbol, 
                         const char *range, size_t max_size) override {
        //TODO: use Rest API
        return 0;
    }
};

//------------------------- DAQuoteBinanceApi ------------------------------

DAQuoteBinanceApi *DAQuoteBinanceApi::CreateApi(const char *api_key, const char *secret_key, const char *dpath)
{
    return new _DAQuoteBinanceApiImpl(api_key, secret_key);
}


//------- Spot --------

int DAQuoteBinanceApi::QueryDepth(const char *symbol, size_t limit)
{
    _DAQuoteBinanceApiImpl *impl = static_cast<_DAQuoteBinanceApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/depth"
    );

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("limit", std::to_string(limit));

    return RestRequest::SendAsync(request, 
      [impl](const std::shared_ptr<restbed::Request>req, const std::shared_ptr<restbed::Response>rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQuoteUpdated(_BINANCE_(BINANCE_TYPE_DEPTH), const_cast<char *>(json.c_str()));
    });
}

int DAQuoteBinanceApi::QueryTrades(const char *symbol, size_t limit)
{
    return 0; //TODO
}

int DAQuoteBinanceApi::QueryTradesHistory(const char *symbol, size_t limit, long fromId)
{
    return 0; //TODO
}

int DAQuoteBinanceApi::QueryTradesAggregate(const char *symbol, size_t limit, long fromId, timestamp_t startTime, timestamp_t endTime)
{
    return 0; //TODO
}

int DAQuoteBinanceApi::QueryKline(const char *symbol, size_t limit, size_t interval, timestamp_t startTime, timestamp_t endTime)
{
    return 0; //TODO
}

int DAQuoteBinanceApi::QueryTicker(const char *symbol)
{
    return 0; //TODO
}

int DAQuoteBinanceApi::QueryPrices(const char *symbol)
{
    return 0; //TODO
}

int DAQuoteBinanceApi::QueryBookTicker(const char *symbol)
{
    return 0; //TODO
}

//----- Stream Data ------

int DAQuoteBinanceApi::StartUserDataStream()
{
    _DAQuoteBinanceApiImpl *impl = static_cast<_DAQuoteBinanceApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, /* POST */
        "/api/v1/userDataStream"
    );

    request->Init().ApiKey(impl->m_api_key);

    return RestRequest::SendAsync(request, 
      [impl](const std::shared_ptr<restbed::Request>req, const std::shared_ptr<restbed::Response>rsp) {
        std::string jsonstr;
        RestRequest::ParseReponse(rsp, jsonstr);
        std::string listenKey = JsonUtils::GetItem(jsonstr, "listenKey");
        impl->m_spi->OnStartUserDataStream(listenKey.c_str());
    });
}

int DAQuoteBinanceApi::KeepAliveUserDataStream(const char *listenKey)
{
    _DAQuoteBinanceApiImpl *impl = static_cast<_DAQuoteBinanceApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_PUT,   /* PUT */
        "/api/v1/userDataStream"
    );

    request->Init()
        .ApiKey(impl->m_api_key)
        .AddParam("listenKey", listenKey);

    std::string lkey = listenKey;

    return RestRequest::SendAsync(request, 
      [impl, lkey](const std::shared_ptr<restbed::Request>req, const std::shared_ptr<restbed::Response>rsp) {
        std::string jsonstr;
        RestRequest::ParseReponse(rsp, jsonstr);
        // re-use OnStartUserDataStream()
        impl->m_spi->OnStartUserDataStream(lkey.c_str());
    });
}

int DAQuoteBinanceApi::CloseUserDataStream(const char *listenKey)
{
    _DAQuoteBinanceApiImpl *impl = static_cast<_DAQuoteBinanceApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_DELETE,   /* DELETE */
        "/api/v1/userDataStream"
    );

    request->Init()
        .ApiKey(impl->m_api_key)
        .AddParam("listenKey", listenKey);

    std::string lkey = listenKey;

    return RestRequest::SendAsync(request, 
      [impl, lkey](const std::shared_ptr<restbed::Request>req, const std::shared_ptr<restbed::Response>rsp) {
        std::string jsonstr;
        RestRequest::ParseReponse(rsp, jsonstr);
        impl->m_spi->OnCloseUserDataStream(lkey.c_str());
    });
}

//----- Extra Data -------

int DAQuoteBinanceApi::PingServer()
{
    _DAQuoteBinanceApiImpl *impl = static_cast<_DAQuoteBinanceApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, 
        "/api/v1/ping"
    );

    request->Init();

    std::string resp = RestRequest::SendSync(request);

    // TODO: check request->m_sent_time and show latency

    return (resp.size() > 0) ? 0 : -ETIMEDOUT;
}

int DAQuoteBinanceApi::GetServerTime()
{
    _DAQuoteBinanceApiImpl *impl = static_cast<_DAQuoteBinanceApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET,
        "/api/v1/time"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const std::shared_ptr<restbed::Request>req, const std::shared_ptr<restbed::Response>rsp) {
        std::string jsonstr;
        RestRequest::ParseReponse(rsp, jsonstr);
        time_t srvtime = JsonUtils::GetItemUint64(jsonstr, "serverTime");
        impl->m_spi->OnServerTime(srvtime);
    });
}

int DAQuoteBinanceApi::GetExchangeInfo()
{
    _DAQuoteBinanceApiImpl *impl = static_cast<_DAQuoteBinanceApiImpl *>(this);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET,
        "/api/v1/exchangeInfo"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const std::shared_ptr<restbed::Request>req, const std::shared_ptr<restbed::Response>rsp) {
        std::string jsonstr;
        RestRequest::ParseReponse(rsp, jsonstr);
        impl->m_spi->OnExchangeInfo(jsonstr.c_str());
    });
}