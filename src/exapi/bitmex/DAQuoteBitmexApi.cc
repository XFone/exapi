/*
 * $Id: $
 *
 * DAQuoteBitmexApi implementation
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
#include "BitmexApi.h"
#include "JsonUtils.h"
#include "quote/DAQuoteBitmexApi.h"

using namespace exapi;

#define _BITMEX_(api_type)         (int)(QuoteApiType::EX_TYPE_BITMEX + api_type)

/**
 * Internal class implemented DAQuoteBitmexApi
 */
class _DAQuoteBitmexApiImpl : public DAQuoteBitmexApi {
private:
    friend class DAQuoteBitmexApi;
    const char          *m_domain;
    std::string          m_api_key;
    std::string          m_secret_key;
    DAQuoteBitmexSpi   *m_spi;

    _DAQuoteBitmexApiImpl(const char *api_key, const char *secret_key) 
      : m_domain(BITMEX_REST_URL), 
        m_api_key(api_key), m_secret_key(secret_key), m_spi(nullptr) {
        // NOTHING
    }

public:

    //------------------- overrides DAQuoteBitmexApi ----------------------

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
        m_spi = dynamic_cast<DAQuoteBitmexSpi *>(pEventHandler);
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

//------------------------- DAQuoteBitmexApi ------------------------------

DAQuoteBitmexApi *DAQuoteBitmexApi::CreateApi(const char *api_key, const char *secret_key, const char *dpath)
{
    return new _DAQuoteBitmexApiImpl(api_key, secret_key);
}


//------- Spot --------

int DAQuoteBitmexApi::QueryDepth(const char *symbol, size_t limit)
{
    return 0; //TODO
}

int DAQuoteBitmexApi::QueryTrades(const char *symbol, size_t limit)
{
    return 0; //TODO
}

int DAQuoteBitmexApi::QueryTradesHistory(const char *symbol, size_t limit, long fromId)
{
    return 0; //TODO
}

int DAQuoteBitmexApi::QueryTicker(const char *symbol)
{
    return 0; //TODO
}

//----- Extra Data -------

int DAQuoteBitmexApi::GetAnnouncement()
{
    return 0; //TODO
}

int DAQuoteBitmexApi::GetAnnouncementUrgent()
{
    return 0; //TODO
}

int DAQuoteBitmexApi::GetSchema()
{
    return 0; //TODO
}

int DAQuoteBitmexApi::GetSchemaWebsocket()
{
    return 0; //TODO
}

int DAQuoteBitmexApi::GetStats()
{
    return 0; //TODO
}

int DAQuoteBitmexApi::GetStatsHistory()
{
    return 0; //TODO
}

int DAQuoteBitmexApi::GetStatsHistoryUsd()
{
    return 0; //TODO
}