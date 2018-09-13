/*
 * $Id: $
 *
 * DATraderBitmexApi implementation
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */

#include "Base.h"
#include "Log.h"

#include "JsonUtils.h"
#include "RestRequest.h"
#include "BitmexApi.h"
#include "trade/DATraderBitmexApi.h"
using namespace exapi;

/**
 * Internal class implemented DATraderOkexApi
 */
class _DATraderBitmexApiImpl : public DATraderBitmexApi {
private:
    friend class DATraderBitmexApi;
    const char         *m_domain;
    std::string         m_api_key;
    std::string         m_secret_key;
    DATraderBitmexSpi  *m_spi;

    _DATraderBitmexApiImpl(const char *api_key, const char *secret_key)
      : m_domain(BITMEX_REST_URL), 
        m_api_key(api_key), m_secret_key(secret_key), m_spi(nullptr) {
        // TODO
    }

public:

    //----------------- Overrides DATraderBitmexApi ---------------------

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
        assert(nullptr != pEventHandler);
        m_spi = dynamic_cast<DATraderBitmexSpi *>(pEventHandler);
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

#define GET_IMPL(p, i) _DATraderBitmexApiImpl *i = static_cast<_DATraderBitmexApiImpl *>(p)

//------------------------- DATraderBitmexSpi -----------------------------

int DATraderBitmexSpi::OnResponse(TraderApiType apiType, void *pRespData)
{
    int res = 0;
    int api = (int)apiType - TraderApiType::EX_TYPE_BITMEX;

    switch (api) {

    default:
        LOGFILE(LOG_INFO, "DATraderBitmexSpi::OnResponse unknown command %d", 
                (int)apiType);
        res = -1; // CONTINUE
        break;
    }

    // release allocated resource of pRespData
    return res;
}

//------------------------- DATraderBitmexApi -----------------------------

DATraderBitmexApi *DATraderBitmexApi::CreateApi(const char *api_key, const char *secret_key, const char *dpath)
{
    return new _DATraderBitmexApiImpl(api_key, secret_key);
}

//-------- APIKey --------

int DATraderBitmexApi::SetApiKey(bool enable)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, 
        enable ? "api/v1/apiKey/enable" : "api/v1/apiKey/disable"
    );

    request->Init()
        .AddParam("apiKeyID", impl->m_api_key);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        const char *data = json.c_str();
        impl->m_spi->OnApiKey(data);
    });
}

int DATraderBitmexApi::GetApiKey(bool reverse)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/apiKey"
    );

    request->Init()
        .AddParam("reverse", std::to_string(reverse));

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        const char *keys = json.c_str();
        impl->m_spi->OnApiKey(keys);
    });
}

int DATraderBitmexApi::CreateApiKey(const char *name, const char *cidr, 
                                    const char *permissions, bool enabled, 
                                    const char *token)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/apiKey"
    );

    request->Init()
        .AddParam("name", name)
        .AddParam("cidr", cidr)
        .AddParam("permissions", permissions)
        .AddParam("enabled", enabled ? "true" : "false")
        .AddParam("token", token);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        const char *keys = json.c_str();
        impl->m_spi->OnApiKey(keys);
    });
}

int DATraderBitmexApi::RemoveApiKey(const char *apiKeyID)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_DELETE, "/api/v1/apiKey"
    );

    request->Init()
        .AddParam("apiKeyID", apiKeyID == nullptr ? impl->m_api_key : apiKeyID);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        const char *keys = json.c_str();
        impl->m_spi->OnApiKey(keys);
    });
}

//--------- Chat ---------

int DATraderBitmexApi::GetMessage(size_t count, size_t start, bool reverse, double channelID)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/chat"
    );

    request->Init()
        .AddParam("count", std::to_string(count))
        .AddParam("start", std::to_string(start))
        .AddParam("reverse", std::to_string(reverse));

    if (channelID != 0.0) {
        request->AddParam("channelID", std::to_string(channelID));
    }

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        const char *data = json.c_str();
        impl->m_spi->OnChatMessages(data);
    });
}

int DATraderBitmexApi::GetChannels()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/chat/channels"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        const char *data = json.c_str();
        impl->m_spi->OnChatChannels(data);
    });
}

int DATraderBitmexApi::GetConnectedUsers()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/chat/connected"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        const char *data = json.c_str();
        impl->m_spi->OnConnectedUsers(data);
    });
}

int DATraderBitmexApi::SendMessage(const char *message, double channelID)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/chat"
    );

    request->Init()
        .AddParam("message", message)
        .AddParam("channelID", std::to_string(channelID));

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        const char *data = json.c_str();
        impl->m_spi->OnSentMessage(data);
    });
}

//-------- Trade ---------


int DATraderBitmexApi::QueryExecutions(const char *symbol, const char *filter, 
                                       const char *columns[], size_t count, 
                                       size_t start, bool reverse, 
                                       time_t startTime, time_t endTime)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/execution"
    );

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("filter", filter)
        .AddParam("count", std::to_string(count))
        .AddParam("start", std::to_string(start))
        .AddParam("reverse", std::to_string(reverse))
        .AddParam("startTime", JsonUtils::to_json(startTime))
        .AddParam("endTime", JsonUtils::to_json(endTime));

    if (nullptr != columns) {
        request->AddParam("columns", JsonUtils::to_json(columns));
    }

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        const char *data = json.c_str();
        impl->m_spi->OnExecutions(data);
    });
}

//------ Instruments -----

int DATraderBitmexApi::QueryInstruments(const char *symbol, const char *filter, 
                                        const char *columns[], size_t count, 
                                        size_t start, bool reverse, 
                                        time_t startTime, time_t endTime)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/instrument"
    );

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("filter", filter)
        .AddParam("count", std::to_string(count))
        .AddParam("start", std::to_string(start))
        .AddParam("reverse", std::to_string(reverse))
        .AddParam("startTime", JsonUtils::to_json(startTime))
        .AddParam("endTime", JsonUtils::to_json(endTime));

    if (nullptr != columns) {
        request->AddParam("columns", JsonUtils::to_json(columns));
    }

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnInstruments(json.c_str());
    });
}

int DATraderBitmexApi::QueryInstrumentsActive()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/instrument/active"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnInstruments(json.c_str());
    });
}

int DATraderBitmexApi::QueryInstrumentActiveAndIndices()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/instrument/activeAndIndices"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnInstruments(json.c_str());
    });
}

int DATraderBitmexApi::QueryInstrumentActiveIntervals()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/instrument/activeIntervals"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnActiveIntervals(json.c_str());
    });
}

int DATraderBitmexApi::QueryCompositeIndex()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/instrument/compositeIndex"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnCompositeIndex(json.c_str());
    });
}

int DATraderBitmexApi::QueryPriceIndices()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/instrument/indices"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnPriceIndices(json.c_str());
    });
}

//----- Trade History ----

int DATraderBitmexApi::QueryTradeHistory(const char *symbol, const char *filter, 
                                         const char *columns[], size_t count, 
                                         size_t start, bool reverse, 
                                         time_t startTime, time_t endTime)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/tradeHistory"
    );

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("filter", filter)
        .AddParam("count", std::to_string(count))
        .AddParam("start", std::to_string(start))
        .AddParam("reverse", std::to_string(reverse))
        .AddParam("startTime", JsonUtils::to_json(startTime))
        .AddParam("endTime", JsonUtils::to_json(endTime));

    if (nullptr != columns) {
        request->AddParam("columns", JsonUtils::to_json(columns));
    }

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnTradeHistory(json.c_str());
    });
}

//------- Future ---------


//------- Wallet ---------
