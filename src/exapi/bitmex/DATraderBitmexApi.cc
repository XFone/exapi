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

extern void AssignParams(RestRequest &request, const QueryFilterParams &params);

void AssignParams(RestRequest &request, const OrderParams &params)
{
    request.AddParam("symbol", params.symbol)
        .AddParam("side", params.side);

    if (nullptr != params.text) {
        request.AddParam("text", params.text);
    }

   // TODO

    /*
    const char *    symbol;            ///< Instrument symbol. e.g. 'XBTUSD'.
    const char *    side;              ///< Order side. Valid options: Buy, Sell. Defaults to 'Buy' unless orderQty or  simpleOrderQty is negative.
    d_quantity_t    simpleOrderQty;     ///< Underlying instrument (i.e. Bitcoin). format: double
    quantity_t      orderQty;           ///< Order quantity in units of the instrument (i.e. contracts). format: int32
    d_price_t       price;              ///< Optional limit price for 'Limit', 'StopLimit', and 'LimitIfTouched' orders. format: double
    quantity_t      displayQty;         ///< Optional quantity to display in the book. Use 0 for a fully hidden order. format: int32
    d_price_t       stopPx;             ///< Optional trigger price for 'Stop', 'StopLimit', 'MarketIfTouched', and 'LimitIfTouched' orders. Use a price below the current price for stop-sell orders and buy-if-touched orders. Use  execInst of 'MarkPrice' or 'LastPrice' to define the current price used for triggering. format: double
    const char *    clOrdID;            ///< Optional Client Order ID. This clOrdID will come back on the order and any related executions.
    const char *    clOrdLinkID;        ///< Optional Client Order Link ID for contingent orders.
    d_price_t       pegOffsetValue;     ///< Optional trailing offset from the current price for 'Stop', 'StopLimit', 'MarketIfTouched', and 'LimitIfTouched' orders; use a negative offset for stop-sell orders and buy-if-touched orders. Optional offset from the peg price for 'Pegged' orders. format: double
    const char *    pegPriceType;       ///< Optional peg price type. Valid options: LastPeg, MidPricePeg, MarketPeg, PrimaryPeg, TrailingStopPeg.
    const char *    ordType;            ///< Order type. Valid options: Market, Limit, Stop, StopLimit, MarketIfTouched, LimitIfTouched, MarketWithLeftOverAsLimit, Pegged. Defaults to 'Limit' when price is specified. Defaults to 'Stop' when stopPx is specified. Defaults to 'StopLimit' when price and stopPx are specified. default: Limit
    const char *    timeInForce;        ///< Time in force. Valid options: Day, GoodTillCancel, ImmediateOrCancel, FillOrKill. Defaults to 'GoodTillCancel' for 'Limit', 'StopLimit', 'LimitIfTouched', and 'MarketWithLeftOverAsLimit' orders.
    const char *    execInst;           ///< Optional execution instructions. Valid options: ParticipateDoNotInitiate, AllOrNone, MarkPrice, IndexPrice, LastPrice, Close, ReduceOnly, Fixed. 'AllOrNone' instruction requires  displayQty to be 0. 'MarkPrice', 'IndexPrice' or 'LastPrice' instruction valid for 'Stop', 'StopLimit', 'MarketIfTouched', and 'LimitIfTouched' orders.
    const char *    contingencyType;    ///< Optional contingency type for use with clOrdLinkID. Valid options: OneCancelsTheOther, OneTriggersTheOther, OneUpdatesTheOtherAbsolute, OneUpdatesTheOtherProportional.
    const char *    text;               ///< Optional order annotation. e.g. 'Take profit'.
    */
}


void AssignParams(RestRequest &request, const OrderAmendParams &params)
{
    if (nullptr != params.orderId) {
        request.AddParam("orderId", params.orderId);
    }

    if (nullptr != params.origClOrdID) {
        request.AddParam("origClOrdID", params.origClOrdID);
    }

    if (nullptr != params.clOrdID) {
        request.AddParam("clOrdID", params.clOrdID);
    }

    // TODO

    /*
    const char *    orderId;            ///< Order ID
    const char *    origClOrdID;        ///< Client Order ID. See POST /order
    const char *    clOrdID;            ///< Optional new Client Order ID, requires origClOrdID.
    d_quantity_t    simpleOrderQty;     ///< Optional order quantity in units of the underlying instrument (i.e. Bitcoin). 
    quantity_t      orderQty;           ///< Optional order quantity in units of the instrument (i.e. contracts)
    d_quantity_t    simpleLeavesQty;    ///< Optional leaves quantity in units of the underlying instrument (i.e. Bitcoin)
    quantity_t      leavesQty;          ///< Optional leaves quantity in units of the instrument (i.e. contracts)
    d_price_t       price;              ///< Optional limit price for 'Limit', 'StopLimit', and 'LimitIfTouched' orders.
    d_price_t       stopPx;             ///< Optional trigger price for 'Stop', 'StopLimit', 'MarketIfTouched', and 'LimitIfTouched' orders. Use a price below the current price for stop-sell orders and buy-if-touched orders.
    double          pegOffsetValue;     ///< Optional trailing offset from the current price for 'Stop', 'StopLimit', 'MarketIfTouched', and 'LimitIfTouched' orders; use a negative offset for stop-sell orders and buy-if-touched orders. Optional offset from the peg price for 'Pegged' orders.
    const char *    text;
    */
}

template <typename T>
void AssignParams(RestRequest &request, const T *orders[])
{
    if (nullptr != orders) {
        const T *param = orders[0];
        while (nullptr != param) {
            AssignParams(request, *param++);
        }
    }
}

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
        impl->m_spi->OnChatMessages(data);  // array[Chat]
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
        impl->m_spi->OnChatChannels(data); // array[ChatChannel]
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
        impl->m_spi->OnConnectedUsers(data);    // ConnectedUsers
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
        impl->m_spi->OnSentMessage(data);   // Chat
    });
}

//-------- Trade ---------

int DATraderBitmexApi::PlaceOrder(const OrderParams &params)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/order"
    );

    AssignParams(request->Init(), params);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnOrderPlaced(json.c_str());   // Order
    });
}

int DATraderBitmexApi::PlaceOrdersBulk(const OrderParams *orders[])
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/order/bulk"
    );

    AssignParams(request->Init(), orders);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnOrdersPlaced(json.c_str());   // array[Order]
    });
}

int DATraderBitmexApi::CancelOrder(const char *orderID[], const char *clOrdID[], 
                                   const char *text)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_DELETE, "/api/v1/order"
    );

    request->Init();

    if (nullptr != orderID) {
        request->AddParam("orderID", JsonUtils::to_json(orderID));
    }

    if (nullptr != clOrdID) {
        request->AddParam("clOrdID", JsonUtils::to_json(clOrdID));
    }

    if (nullptr != text) {
        request->AddParam("text", text);
    }

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnOrdersCancelled(json.c_str());   // array[Order]
    });
}

int DATraderBitmexApi::CancelOrdersAll(const char *symbol, json_t filter, const char *text)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_DELETE, "/api/v1/order/all"
    );

    request->Init();

    if (nullptr != symbol) {
        request->AddParam("symbol", symbol);
    }

    if (nullptr != filter) {
        request->AddParam("filter", JsonUtils::to_json(filter));
    }

    if (nullptr != text) {
        request->AddParam("text", text);
    }

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnOrdersCancelled(json.c_str());   // array[Order]
    });
}

int DATraderBitmexApi::CancelOrdersAfter(timestamp_t timeout)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/order/cancelAllAfter"
    );

    request->Init()
        .AddParam("timeout", std::to_string(timeout));

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnOrdersCancelled(json.c_str());   // Object
    });
}

int DATraderBitmexApi::UpdateOrder(const OrderAmendParams &params)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_PUT, "/api/v1/order"
    );

    AssignParams(request->Init(), params);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnOrderUpdated(json.c_str());   // Order
    });
}

int DATraderBitmexApi::UpdateOrders(const OrderAmendParams *orders[])
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_PUT, "/api/v1/order/bulk"
    );

    AssignParams(request->Init(), orders);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnOrdersUpdated(json.c_str());   // array[Order]
    });
}

int DATraderBitmexApi::QueryLiquidationOrders(const QueryFilterParams &params)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/liquidation"
    );

    AssignParams(request->Init(), params);

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

    AssignParams(request->Init(), params);

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

    AssignParams(request->Init(), params);

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

    AssignParams(request->Init(), params);

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

    AssignParams(request->Init(), params);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnTradeHistory(json.c_str());
    });
}

//------- Future ---------

int DATraderBitmexApi::QueryPosition(json_t filter, const char *columns[], size_t count)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/position"
    );

    request->Init()
        .AddParam("count", std::to_string(count));

    if (nullptr != filter) {
        request->AddParam("filter", JsonUtils::to_json(filter));
    }

    if (nullptr != columns) {
        request->AddParam("columns", JsonUtils::to_json(columns));
    }

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQueryPositions(json.c_str()); // array[Position]
    });
}

int DATraderBitmexApi::SetPositionIsolateMargin(const char *symbol, bool enable)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/position/isolate"
    );

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("enable", std::to_string(enable));

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnPositionIsolateMargin(json.c_str()); // Position
    });
}

int DATraderBitmexApi::SetPositionTransferMargin(const char *symbol, amount_t amount)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/position/transferMargin"
    );

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("amount", std::to_string(amount));

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnPositionTransferMargin(json.c_str()); // Position
    });
}

int DATraderBitmexApi::SetPositionLeverage(const char *symbol, double leverage)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/position/leverage"
    );

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("leverage", std::to_string(leverage));

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnPositionLeverage(json.c_str()); // Position
    });
}

int DATraderBitmexApi::SetPositionRiskLimit(const char *symbol, amount_t riskLimit)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/position/riskLimit"
    );

    request->Init()
        .AddParam("symbol", symbol)
        .AddParam("riskLimit", std::to_string(riskLimit));

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnPositionLeverage(json.c_str()); // Position
    });
}

int DATraderBitmexApi::QuerySettlement(const QueryFilterParams &params)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/settlement"
    );

    AssignParams(request->Init(), params);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQuerySettlement(json.c_str()); // Position
    });
}

//------- Account --------

int DATraderBitmexApi::Logout()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/user/logout"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnLogout(json.c_str()); // NOTHING
    });
}

int DATraderBitmexApi::LogoutAll()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/user/logoutAll"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        // double ttl = atof(json.c_str());
        impl->m_spi->OnLogoutAll(json.c_str()); // double ? ttl ?
    });
}

int DATraderBitmexApi::ConfirmEmail(const char *token)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/user/confirmEmail"
    );

    request->Init()
        .AddParam("token", token);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnEmailConfirmed(json.c_str()); // AccessToken
    });
}

int DATraderBitmexApi::ConfirmEnableTFA(const char *ttype, const char *token)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/user/confirmEnableTFA"
    );

    request->Init()
        .AddParam("type", ttype)
        .AddParam("token", token);

    return RestRequest::SendAsync(request, 
      [impl, token](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        bool enabled = atof(json.c_str());
        impl->m_spi->OnTFAComfirmed(token, enabled); // Boolean
    });
}

int DATraderBitmexApi::EnableTFA(const char *ttype)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/user/requestEnableTFA"
    );

    request->Init()
        .AddParam("type", ttype);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        bool enabled = atof(json.c_str());
        impl->m_spi->OnTFAEnabled(enabled); // Boolean
    });
}

int DATraderBitmexApi::DisableTFA(const char *ttype, const char *token)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/user/disableTFA"
    );

    request->Init()
        .AddParam("type", ttype)
        .AddParam("token", token);

    return RestRequest::SendAsync(request, 
      [impl, token](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        bool enabled = atof(json.c_str());
        impl->m_spi->OnTFADisabled(token, enabled); // Boolean
    });
}

int DATraderBitmexApi::QueryUser()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/user"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQueryUser(json.c_str()); // User
    });
}

int DATraderBitmexApi::UpdateUser(const char *firstname, const char *lastname,
                                  const char *oldPassword, const char *newPassword,
                                  const char *newPasswordConfirm, const char *username,
                                  const char *country, const char *pgpPubKey)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_PUT, "/api/v1/user"
    );

    request->Init();

    if (nullptr != firstname) {
        request->AddParam("firstname", firstname);
    }

    if (nullptr != lastname) {
        request->AddParam("lastname", lastname);
    }

    if (nullptr != oldPassword || nullptr != newPassword) {
        request->AddParam("oldPassword", oldPassword)
            .AddParam("newPassword", newPassword)
            .AddParam("newPasswordConfirm", newPasswordConfirm);
    }

    if (nullptr != country) {
        request->AddParam("country", country);
    }

    if (nullptr != pgpPubKey) {
        request->AddParam("pgpPubKey", pgpPubKey);
    }

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnUserUpdated(json.c_str()); // User
    });
}

int DATraderBitmexApi::QueryUserAffiliateStatus()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/affiliateStatus"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnUserAffiliateStatus(json.c_str()); // Affiliate
    });
}

int DATraderBitmexApi::QueryUserCommission()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/commission"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnUserCommissionStatus(json.c_str()); // array[UserCommission]
    });
}

int DATraderBitmexApi::QueryUserMargin(const char *currency)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/margin"
    );

    request->Init()
        .AddParam("currency", currency);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnUserMarginStatus(json.c_str()); // Margin
    });
}

int DATraderBitmexApi::SaveUserPreferences(const json_t prefs, bool overwrite)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/preferences"
    );

    request->Init()
        .AddParam("prefs", JsonUtils::to_json(prefs))
        .AddParam("overwrite", std::to_string(overwrite));

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnUserPreferences(json.c_str()); // User
    });
}

int DATraderBitmexApi::GetNotification()
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/notification"
    );

    request->Init();

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnNotification(json.c_str()); // array[Notification]
    });
}

//------- Wallet ---------

int DATraderBitmexApi::RequestWithdraw(const char *otpToken, const char *currency, 
                                       amount_t amount, const char *dest_addr, 
                                       price_t fee)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/user/requestWithdrawal"
    );

    request->Init()
        .AddParam("otpToken", otpToken)
        .AddParam("currency", currency == nullptr ? "XBt" : currency)
        .AddParam("amount", std::to_string(amount))
        .AddParam("address", dest_addr);

    if (fee > 0.0) {
        request->AddParam("fee", std::to_string(fee));
    }

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnResponseWithdraw(json.c_str()); // Transaction
    });
}

int DATraderBitmexApi::CancelWithdraw(const char *token)
{
    GET_IMPL(this, impl);

    if (token == nullptr) {
        return -EINVAL; // invalid argument
    }
    
    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/user/cancelWithdrawal"
    );

    request->Init()
        .AddParam("token", token);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnWithdrawCancelled(json.c_str()); // Transaction
    });
}

int DATraderBitmexApi::ConfirmWithdraw(const char *token)
{
    GET_IMPL(this, impl);

    if (token == nullptr) {
        return -EINVAL; // invalid argument
    }

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_POST, "/api/v1/user/confirmWithdrawal"
    );

    request->Init()
        .AddParam("token", token);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnWithdrawConfirmed(json.c_str()); // Transaction
    });
}

int DATraderBitmexApi::QueryDepositAddress(const char *currency)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/user/depositAddress"
    );

    request->Init()
        .AddParam("currency", currency);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnDepositAddress(json.c_str()); // address string
    });
}

int DATraderBitmexApi::QueryWallet(const char *currency)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/user/wallet"
    );

    request->Init()
        .AddParam("currency", currency);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnQueryWallet(json.c_str()); // Wallet
    });
}

int DATraderBitmexApi::QueryWalletHistory(const char *currency)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/user/walletHistory"
    );

    request->Init()
        .AddParam("currency", currency);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnWalletHistory(json.c_str()); // array[Transaction]
    });
}

int DATraderBitmexApi::QueryWalletSummary(const char *currency)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/user/walletSummary"
    );

    request->Init()
        .AddParam("currency", currency);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnWalletSummary(json.c_str()); // array[Transaction]
    });
}

int DATraderBitmexApi::QueryMinWithdrawalFee(const char *currency)
{
    GET_IMPL(this, impl);

    auto request = RestRequest::CreateBuilder(
        impl->m_domain, HTTP_PROTOCOL_HTTPS, METHOD_GET, "/api/v1/user/minWithdrawalFee"
    );

    request->Init()
        .AddParam("currency", currency);

    return RestRequest::SendAsync(request, 
      [impl](const request_t req, const response_t rsp) {
        std::string json;
        RestRequest::ParseReponse(rsp, json);
        impl->m_spi->OnMinWithdrawalFee(json.c_str()); // object
    });
}

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