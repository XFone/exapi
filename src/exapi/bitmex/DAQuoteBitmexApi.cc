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
#include "BitmexApiDef.h"
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
    DAQuoteBitmexSpi    *m_spi;

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
        //TODO: call QueryQuotes();
        return 0;
    }
};

#define GET_IMPL(p, i) _DAQuoteBitmexApiImpl *i = static_cast<_DAQuoteBitmexApiImpl *>(p)

//------------------------- DAQuoteBitmexApi ------------------------------

DAQuoteBitmexApi *DAQuoteBitmexApi::CreateApi(const char *api_key, const char *secret_key, const char *dpath)
{
    return new _DAQuoteBitmexApiImpl(api_key, secret_key);
}

//----- Quote Data ------

int DAQuoteBitmexApi::QueryOrderBookLevel2(const char *symbol, size_t depth)
{
    return 0; //TODO
}

int DAQuoteBitmexApi::QueryQuotes(const QueryFilterParams& params)
{
    return 0; //TODO
}

int DAQuoteBitmexApi::QueryQuotesBucketed(const char *binSize, bool partial,
                                          const QueryFilterParams &params)
{
    return 0; //TODO
}

//------- Funding ------

int DAQuoteBitmexApi::QueryFundingHistory(const QueryFilterParams &params)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/funding"
    );

    request->Init()
        .AddParam("symbol", params.symbol)
        .AddParam("filter", JsonUtils::to_json(params.filter))
        .AddParam("count", std::to_string(params.count))
        .AddParam("start", std::to_string(params.start))
        .AddParam("reverse", std::to_string(params.reverse))
        .AddParam("startTime", JsonUtils::to_datetime(params.startTime))
        .AddParam("endTime", JsonUtils::to_datetime(params.endTime));

    if (nullptr != params.columns) {
        request->AddParam("columns", JsonUtils::to_json(params.columns));
    }

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnFundingHistory(json.c_str());
    });
}

int DAQuoteBitmexApi::QueryInsuranceHistory(const QueryFilterParams &params)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/insurance"
    );

    request->Init()
        .AddParam("symbol", params.symbol)
        .AddParam("filter", JsonUtils::to_json(params.filter))
        .AddParam("count", std::to_string(params.count))
        .AddParam("start", std::to_string(params.start))
        .AddParam("reverse", std::to_string(params.reverse))
        .AddParam("startTime", JsonUtils::to_datetime(params.startTime))
        .AddParam("endTime", JsonUtils::to_datetime(params.endTime));

    if (nullptr != params.columns) {
        request->AddParam("columns", JsonUtils::to_json(params.columns));
    }

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnInsuranceHistory(json.c_str());
    });
}

//------- Trades Data ----

int DAQuoteBitmexApi::QueryTrades(const QueryFilterParams &params)
{
    return 0;   // TODO
}

int DAQuoteBitmexApi::QueryTradesBucketed(const char *binSize, bool partial,
                                          const QueryFilterParams &params)
{
    return 0;   // TODO
}

//----- Extra Data -------

int DAQuoteBitmexApi::GetAnnouncement(const char *columns[])
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/announcement"
    );

    request->Init();

    if (nullptr != columns) {
        request->AddParam("columns", JsonUtils::to_json(columns));
    }

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnAnnouncement(json.c_str());
    });
}

int DAQuoteBitmexApi::GetAnnouncementUrgent()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/announcement/urgent"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        const char *data = json.c_str();
        impl->m_spi->OnAnnouncementUrgent(data);
    });
}

int DAQuoteBitmexApi::GetSchema(const char *model)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/schema"
    );

    request->Init();
    if (nullptr != model) {
        request->AddParam("model", model);
    }

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        const char *data = json.c_str();
        impl->m_spi->OnSchema(data);
    });
}

int DAQuoteBitmexApi::GetSchemaWebsocket()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/schema/websocketHelp"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        const char *data = json.c_str();
        impl->m_spi->OnSchemaWebsocket(data);
    });
}

int DAQuoteBitmexApi::GetStats()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/stats"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnStats(json.c_str());
    });
}

int DAQuoteBitmexApi::GetStatsHistory()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/stats/history"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnStatsHistory(json.c_str());
    });
}

int DAQuoteBitmexApi::GetStatsHistoryUsd()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/stats/historyUSD"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnStatsHistoryUsd(json.c_str());
    });
}