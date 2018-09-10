/*
 * BinanceAPI C++
 *
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 * 
 * $Log: $
 */

#ifndef __BINANCE_API_H__
#define __BINANCE_API_H__

/** @file BinanceApi.h Binance API Definitions
 */

#define BINANCE_REST_URL            "https://api.binance.com"
#define BINANCE_REST_URL_CO         "https://www.binance.co"
#define BINANCE_WSS_URL             "wss://stream.binance.com:9443"
#define BINANCE_WSS_URL_CO          "wss://stream.binance.co:9443"

#include <map>

/** 
 * Error codes for Binance (2018-07-18)
 * ref <a href="https://github.com/binance-exchange/binance-official-api-docs/blob/master/errors.md">errors.md</a>
 */
typedef std::map<int, const char *> ErrorCodesBinance;

inline void GetErrorCodes(ErrorCodesBinance &m) {
    m = { // requires C++11
        { -1000,  "UNKNOWN" },
        // ...
    };
}

/** 
 * Rest API for Binance (2018-07-18)
 * ref <a href="https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md">rest-api.md</a>
 */
enum BinanceApiType {
    BINANCE_TYPE_UNKNOWN    = 0,

    // General endpoints
    BINANCE_TYPE_CONNECTIVITY,          ///< GET /api/v1/ping
    BINANCE_TYPE_SERVER_TIME,           ///< GET /api/v1/time
    BINANCE_TYPE_EXCHANGE_INFO,         ///< GET /api/v1/exchangeInfo

    // Market Data endpoints (requires API-Key)
    BINANCE_TYPE_DEPTH,                 ///< GET /api/v1/depth
    BINANCE_TYPE_TRADES_LIST,           ///< GET /api/v1/trades
    BINANCE_TYPE_TRADES_HISTORY,        ///< GET /api/v1/historicalTrades
    BINANCE_TYPE_TRADES_AGGREGATE,      ///< GET /api/v1/aggTrades
    BINANCE_TYPE_KLINES,                ///< GET /api/v1/klines
    BINANCE_TYPE_TICKER_24HR,           ///< GET /api/v1/ticker/24hr
    BINANCE_TYPE_TICKER_PRICE,          ///< GET /api/v3/ticker/price
    BINANCE_TYPE_TICKER_BOOKTICKER,     ///< GET /api/v3/ticker/bookTicker

    // Account endpoints (requires API-Key and signature)
    BINANCE_TYPE_ORDER,                 ///< POST /api/v3/order  (HMAC SHA256)
    BINANCE_TYPE_ORDER_TEST,            ///< POST /api/v3/order/test (HMAC SHA256)
    BINANCE_TYPE_QUERY_ORDER,           ///< GET /api/v3/order (HMAC SHA256)
    BINANCE_TYPE_CANCEL,                ///< DELETE /api/v3/order  (HMAC SHA256)
    BINANCE_TYPE_OPEN_ORDERS,           ///< GET /api/v3/openOrders  (HMAC SHA256)
    BINANCE_TYPE_ALL_ORDERS,            ///< GET /api/v3/allOrders (HMAC SHA256)
    BINANCE_TYPE_ACCOUNT_INFO,          ///< GET /api/v3/account (HMAC SHA256)
    BINANCE_TYPE_ACCOUNT_TRADES,        ///< GET /api/v3/myTrades  (HMAC SHA256)
    
    // User data stream endpoints: \ref user-data-stream.md  (requires API-Key)
    BINANCE_TYPE_START_DATA_STREAM,     ///< POST /api/v1/userDataStream
    BINANCE_TYPE_KEEPALIVE_DATA_STREAM, ///< PUT /api/v1/userDataStream
    BINANCE_TYPE_CLOSE_DATA_STREAM,     ///< DELETE /api/v1/userDataStream

};

/** @note Filters is defined in class \ref DAFilterBinance */

/**
 * User Data Streams for Binance (2018-07-18)
 * ref <a href="https://github.com/binance-exchange/binance-official-api-docs/blob/master/user-data-stream.md">user-data-stream.md</a>
 */
enum BinanceExecutionType {
    BINANCE_ET_NEW = 0,
    BINANCE_ET_CANCELED,
    BINANCE_ET_REPLACED,        // (currently unused)
    BINANCE_ET_REJECTED,
    BINANCE_ET_TRADE,
    BINANCE_ET_EXPIRED,
    BINANCE_ET_MAX_UNDEFINED
};

/**
 * Web Socket Streams for Binance (2018-07-18)
 * ref <a href="https://github.com/binance-exchange/binance-official-api-docs/blob/master/web-socket-streams.md">web-socket-streams.md</a>
 */
#define BINANCE_WSS_RAW_PATH    "/ws/"

#define BINANCE_WSS_COMB_PATH   "/stream?streams="

// TODO "How to manage a local order book correctly"

/**
 * Public WAPI (Wallet API) for Binance (2018-07-18)
 * ref <a href="https://github.com/binance-exchange/binance-official-api-docs/blob/master/wapi-api.md">wapi-api.md</a>
 */
enum BinanceWalletApiType {
    BINANCE_WAPI_UNKNOWN    = 0,

    BINANCE_WAPI_WITHDRAW,          ///< POST /wapi/v3/withdraw.html (HMAC SHA256)
    BINANCE_WAPI_DEPOSIT_HISTORY,   ///< GET /wapi/v3/depositHistory.html (HMAC SHA256)
    BINANCE_WAPI_WITHDRAW_HISTORY,  ///< GET /wapi/v3/withdrawHistory.html (HMAC SHA256)
    BINANCE_WAPI_DEPOSIT_ADDRESS,   ///< GET /wapi/v3/depositAddress.html (HMAC SHA256)
    
    BINANCE_WAPI_ACCOUNT_STATUS,    ///< GET /wapi/v3/accountStatus.html
    BINANCE_WAPI_SYSTEM_STATUS,     ///< GET /wapi/v3/systemStatus.html
    
    BINANCE_WAPI_DUST_LOG,          ///< GET /wapi/v3/userAssetDribbletLog.html (HMAC SHA256)
    BINANCE_WAPI_TRADE_FEE,         ///< GET /wapi/v3/tradeFee.html (HMAC SHA256)
    BINANCE_WAPI_ASSET_DETAIL       ///< GET /wapi/v3/assetDetail.html (HMAC SHA256)
};

#endif /* __BINANCE_API_H__ */