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

int DATraderBitmexApi::PlaceOrder(const OrderParams &params)
{
    return 0; // TODO
}

int DATraderBitmexApi::PlaceOrdersBulk(const OrderParams *orders[])
{
    return 0; // TODO
}

int DATraderBitmexApi::CancelOrder(const char *orders[])
{
    return 0; // TODO
}

int CancelOrdersAll(const char *symbol, const char *filter, const char *text)
{
    return 0; // TODO
}

int CancelOrdersAfter(timestamp_t timeout)
{
    return 0; // TODO
}

int DATraderBitmexApi::UpdateOrder(const OrderAmendParams &params)
{
    return 0; // TODO
}

int DATraderBitmexApi::UpdateOrders(const char *orders[])
{
    return 0; // TODO
}

int DATraderBitmexApi::QueryLiquidationOrders(const QueryFilterParams &params)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/liquidation"
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
        impl->m_spi->OnLiquidation(json.c_str());
    });
}

int DATraderBitmexApi::QueryExecutions(const QueryFilterParams &params)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/execution"
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
        impl->m_spi->OnExecutions(json.c_str());
    });
}

//------ Instruments -----

int DATraderBitmexApi::QueryInstruments(const QueryFilterParams &params)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/instrument"
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

int DATraderBitmexApi::QueryOrders(const QueryFilterParams &params)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/order"
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
        impl->m_spi->OnQueryOrders(json.c_str());
    });
}

int DATraderBitmexApi::QueryTradeHistory(const QueryFilterParams &params)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/execution/tradeHistory"
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
        impl->m_spi->OnTradeHistory(json.c_str());
    });
}

//------- Future ---------

int DATraderBitmexApi::QueryPosition(const QueryPositionParams &params)
{
    return 0; // TODO
}

int DATraderBitmexApi::SetPositionIsolateMargin(const char *symbol, bool enable)
{
    return 0; // TODO
}

int DATraderBitmexApi::SetPositionTransferMargin(const char *symbol, amount_t amount)
{
    return 0; // TODO
}

int DATraderBitmexApi::SetPositionLeverage(const char *symbol, double leverage)
{
    return 0; // TODO
}

int DATraderBitmexApi::SetPositionRiskLimit(const char *symbol, amount_t riskLimit)
{
    return 0; // TODO
}

int DATraderBitmexApi::ClosePosition(const char *symbol, d_price_t price)
{
    return 0; // TODO
}

int DATraderBitmexApi::QuerySettlement(const QueryFilterParams &params)
{
    return 0; // TODO
}

//------- Wallet ---------


//------ Top Users -------

int DATraderBitmexApi::GetLeadboard(const char *method)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/leaderboard"
    );

    request->Init()
        .AddParam("method", method);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnLeadboard(json.c_str());
    });
}

int DATraderBitmexApi::GetLeadboardName()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/leaderboard/name"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        std::string name = JsonUtils::GetItemString(json, "name");
        impl->m_spi->OnLeadboardName(name.c_str());
    });
}