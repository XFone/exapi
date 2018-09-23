/*
 * BitmexAPI C++
 *
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 * 
 * $Log: $
 */

#ifndef __BITMEX_API_H__
#define __BITMEX_API_H__

/** @file BitmexApi.h Bitmex API Definitions
 */

#define BITMEX_REST_URL            "https://www.bitmex.com"
#define BITMEX_WSS_URL             "wss://www.bitmex.com"

#define BITMEX_REST_TESTNET        "https://testnet.bitmex.com"
#define BITMEX_WSS_TESTNET         "wss://testnet.bitmex.com"

#define SATOSHI                    0.00000001

/** 
 * Rest API for Bitmex
 * ref [REST API](https://testnet.bitmex.com/app/restAPI)
 */
enum BitmexApiType {
    BITMEX_TYPE_UNKNOWN    = 0,

    // Public Announcement
    BITMEX_TYPE_ANNOUNCEMENT,                   ///< GET /api/v1/announcement
    BITMEX_TYPE_ANNOUNCEMENT_URGENT,            ///< GET /api/v1/announcement/urgent

    // APIKey - Persistent API Keys for Developers
    BITMEX_TYPE_APIKEY_DISALBE,                 ///< POST /api/v1/apiKey/disable
    BITMEX_TYPE_APIKEY_ENABLE,                  ///< POST /api/v1/apiKey/enable
    BITMEX_TYPE_APIKEY_GET,                     ///< GET /api/v1/apiKey
    BITMEX_TYPE_APIKEY_CREATE,                  ///< POST /api/v1/apiKey
    BITMEX_TYPE_APIKEY_REMOVE,                  ///< DELETE /api/v1/apiKey

    // Chat - Trollbox Data
    BITMEX_TYPE_CHAT_GET_MSG,                   ///< GET /api/v1/chat
    BITMEX_TYPE_CHAT_CHANNELS,                  ///< GET /api/v1/chat/channels
    BITMEX_TYPE_CHAT_CONNECTED_USERS,           ///< GET /api/v1/chat/connected
    BITMEX_TYPE_CHAT_SEND_MSG,                  ///< POST /api/v1/chat

    // Execution - Raw Order and Balance Data
    BITMEX_TYPE_EXECUTION,                      ///< GET /api/v1/execution
    BITMEX_TYPE_TRADE_HISTORY,                  ///< GET /api/v1/execution/tradeHistory

    // Funding - Swap Funding History
    BITMEX_TYPE_FUNDING_HISTORY,                ///< GET /api/v1/funding

    // Instrument - Tradeable Contracts, Indices, and History
    BITMEX_TYPE_INSTRUMENT,                     ///< GET /api/v1/instrument
    BITMEX_TYPE_INSTRUMENT_ACTIVE,              ///< GET /api/v1/instrument/active
    BITMEX_TYPE_ACTIVE_INDICES,                 ///< GET /api/v1/instrument/activeAndIndices
    BITMEX_TYPE_ACTIVE_INTERVALS,               ///< GET /api/v1/instrument/activeIntervals
    BITMEX_TYPE_COMPOSITE_INDEX,                ///< GET /api/v1/instrument/compositeIndex
    BITMEX_TYPE_PRICE_INDICES,                  ///< GET /api/v1/instrument/indices

    // Insurance - Insurance Fund Data
    BITMEX_TYPE_INSURANCE_HISTORY,              ///< GET /api/v1/insurance

    // Leaderboard - Information on Top Users
    BITMEX_TYPE_LEADBOARD,                      ///< GET /api/v1/leaderboard
    BITMEX_TYPE_LEADBOARD_NAME,                 ///< GET /api/v1/leaderboard/name

    // Liquidation - Active Liquidations
    BITMEX_TYPE_LIQUIDATION_ORDERS,             ///< GET /api/v1/liquidation

    // Account Notification
    BITMEX_TYPE_NOTIFICATION,                   ///< GET /api/v1/notification

    // Order - Placement, Cancellation, Amending, and History
    BITMEX_TYPE_ORDER_AMEND,                    ///< PUT /api/v1/order
    BITMEX_TYPE_ORDERS_AMEND,                   ///< PUT /api/v1/order/bulk
    BITMEX_TYPE_CANCEL,                         ///< DELETE /api/v1/order
    BITMEX_TYPE_CANCEL_ALL,                     ///< DELETE /api/v1/order/all
    BITMEX_TYPE_CANCEL_AFTER,                   ///< POST /api/v1/order/cancelAllAfter
    BITMEX_TYPE_POSITION_CLOSE,                 ///< POST /api/v1/order/closePosition
    BITMEX_TYPE_ORDERS_QUERY,                   ///< GET /api/v1/order
    BITMEX_TYPE_ORDER,                          ///< POST /api/v1/order
    BITMEX_TYPE_ORDERS,                         ///< POST /api/v1/order/bulk

    // OrderBook - Level 2 Book Data
    BITMEX_TYPE_ORDER_BOOK,                     ///< GET /api/v1/orderBook/L2

    // Position - Summary of Open and Closed Positions
    BITMEX_TYPE_POSITION,                       ///< GET /api/v1/position
    BITMEX_TYPE_POSITION_ISOLATE,               ///< POST /api/v1/position/isolate
    BITMEX_TYPE_POSITION_TRANSFER_MARGIN,       ///< POST /api/v1/position/transferMargin
    BITMEX_TYPE_POSITION_LEVERAGE,              ///< POST /api/v1/position/leverage
    BITMEX_TYPE_POSITION_RISK_LIMIT,            ///< POST /api/v1/position/riskLimit

    // Quote - Best Bid/Offer Snapshots & Historical Bins
    BITMEX_TYPE_QUOTE,                          ///< GET /api/v1/quote
    BITMEX_TYPE_QUOTE_HISTORY,                  ///< GET /api/v1/quote/bucketed

    // Schema - Dynamic Schemata for Developers
    BITMEX_TYPE_SCHEMA,                         ///< GET /api/v1/schema
    BITMEX_TYPE_WEBSOCKET_HELP,                 ///< GET /api/v1/schema/websocketHelp

    // Settlement - Historical Settlement Data
    BITMEX_TYPE_SETTLEMENT_HISTORY,             ///< GET /api/v1/settlement

    // Stats - Exchange Statistics
    BITMEX_TYPE_STATS,                          ///< GET /api/v1/stats
    BITMEX_TYPE_STATS_HISTORY,                  ///< GET /api/v1/stats/history
    BITMEX_TYPE_STATS_HISTORY_USD,              ///< GET /api/v1/stats/historyUSD

    // Trade - Individual & Bucketed Trades
    BITMEX_TYPE_TRADES,                         ///< GET /api/v1/trade
    BITMEX_TYPE_TRADES_HISTORY,                 ///< GET /api/v1/trade/bucketed

    // User - Account Operations
    BITMEX_TYPE_CANCEL_WITHDRAW,                ///< POST /api/v1/user/cancelWithdrawal
    BITMEX_TYPE_CHECK_REFERRAL_CODE,            ///< GET /api/v1/user/checkReferralCode
    BITMEX_TYPE_CONFIRM_EMAIL,                  ///< POST /api/v1/user/confirmEmail
    BITMEX_TYPE_CONFIRM_ENABLE_TFA,             ///< POST /api/v1/user/confirmEnableTFA
    BITMEX_TYPE_CONFIRM_WITHDRAW,               ///< POST /api/v1/user/confirmWithdrawal
    BITMEX_TYPE_CONFIRM_DISABLE_TFA,            ///< POST /api/v1/user/disableTFA
    BITMEX_TYPE_USER,                           ///< GET /api/v1/user
    BITMEX_TYPE_AFFILIATE_STATUS,               ///< GET /api/v1/user/affiliateStatus
    BITMEX_TYPE_COMMISSION,                     ///< GET /api/v1/user/commission
    BITMEX_TYPE_DEPOSIT_ADDRESS,                ///< GET /api/v1/user/depositAddress
    BITMEX_TYPE_MARGIN,                         ///< GET /api/v1/user/margin
    BITMEX_TYPE_WALLET,                         ///< GET /api/v1/user/wallet
    BITMEX_TYPE_WALLET_HISTORY,                 ///< GET /api/v1/user/walletHistory
    BITMEX_TYPE_WALLET_SUMMARY,                 ///< GET /api/v1/user/walletSummary
    BITMEX_TYPE_LOGOUT,                         ///< POST /api/v1/user/logout
    BITMEX_TYPE_LOGOUT_ALL,                     ///< POST /api/v1/user/logoutAll
    BITMEX_TYPE_MIN_WITHDRAWAL_FEE,             ///< GET /api/v1/user/minWithdrawalFee
    BITMEX_TYPE_ENABLE_TFA,                     ///< POST /api/v1/user/requestEnableTFA
    BITMEX_TYPE_REQUEST_WITHDRAW,               ///< POST /api/v1/user/requestWithdrawal
    BITMEX_TYPE_USER_PREFERENCES,               ///< POST /api/v1/user/preferences
    BITMEX_TYPE_USER_UPDATE,                    ///< PUT /api/v1/user

};

/**
 * Web Socket Streams for Bitmex
 * see [WebSocket API]("https://www.bitmex.com/app/wsAPI")
 */
#define BITMEX_WSS_PATH         "/realtime"

/**
 * Web Socket Streams Multiplexing
 * see [Multiplexing](https://www.bitmex.com/app/wsAPI#Multiplexing)
 * Packet Format: [<type>,<id>,<topic>,<payload>]
 */
#define BITMEX_WSS_MUX_PATH     "/realtimemd"

#endif /* __BITMEX_API_H__ */