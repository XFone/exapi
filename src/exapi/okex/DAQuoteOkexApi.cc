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
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return 0;
    }

    //------------------- overrides IQuoteApi -----------------------------

    virtual int ConnServer(const char *slist[], IQuoteSpi *pEventHandler) override {
    // virtual int ConnServer(const char *slist[], DAQuoteOkexSpi *pEventHandler) override {
        assert(nullptr != pEventHandler);
        m_spi = dynamic_cast<DAQuoteOkexSpi *>(pEventHandler);
        if (m_spi == nullptr) {
            assert(0);
        }
        return 0;
    }

    virtual int DisConnServer(const char *addr) override {
        return 0;
    }

    virtual int Subscribe(const char *filter_symbol) override {
        return 0;
    }

    virtual int Unsubscribe(const char *filter_symbol) override {
        return 0;
    }

    virtual int GetQuote(int quoteType, const char *symbol, 
                         const char *range, size_t max_size) override {
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


int DAQuoteOkexApi::GetTicker(const char *symbol)
{
    int res;
    _DAQuoteOkexApiImpl *impl = static_cast<_DAQuoteOkexApiImpl *>(this);

    if (0 != (res = impl->VerifyConnection())) {
        return res;
    }
    
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

int GetDepth(const char *symbol, const char *range, char *merge)
{
    return 0;
}

int GetTrades(const char *symbol, const char *since)
{
    return 0;
}

int GetKline(int quoteType, const char *symbol, const char *range, size_t max_size)
{
    return 0;
}

int GetFutureTicker(const char *symbol, const char *contract_type)
{
    return 0;
}

int GetFutureTrades(const char *symbol, const char *contract_type)
{
    return 0;
}

int GetFutureIndex(const char *symbol)
{
    return 0;
}

int GetFutureKline(int quoteType, const char *symbol, const char *contract_type,
                   const char *range, size_t max_size)
{
    return 0;
}


int GetExchange_Rate()
{
    return 0;
}

int GetFutureEstimatedPrice(const char *symbol)
{
    return 0;
}