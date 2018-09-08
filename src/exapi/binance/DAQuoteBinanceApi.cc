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

#include "RestRequest.h"
#include "BinanceApi.h"
#include "quote/DAQuoteBinanceApi.h"

using namespace exapi;


/**
 * Internal class implemented DAQuoteBinanceApi
 */
class _DAQuoteBinanceApiImpl : public DAQuoteBinanceApi {
private:
    friend class DAQuoteBinanceApi;
    const char          *m_domain;
    DAQuoteBinanceSpi   *m_spi;

    _DAQuoteBinanceApiImpl() : m_domain(BINANCE_REST_URL), m_spi(nullptr) {
        // TODO
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

//------------------------- DAQuoteBinanceApi ------------------------------

DAQuoteBinanceApi *DAQuoteBinanceApi::CreateApi(const char *api_key, const char *secret_key, const char *dpath)
{
    return new _DAQuoteBinanceApiImpl();
}


//------- Spot --------


//------- Future --------

