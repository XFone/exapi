# BitMEX C++ API

<!-- markdownlint-disable MD004 MD007 MD012 MD036 -->
_last updated by ziven 2018-09-25_

## 1. Instro

- BitMEX's Online Documents
    - [HTTP API](https://testnet.bitmex.com/app/security)
    - [REST API](https://testnet.bitmex.com/app/restAPI)
    - [WebSocket API](https://testnet.bitmex.com/app/wsAPI)

- [BitMEX api-connectors](https://github.com/BitMEX/api-connectors)

### 1.1 Limits

REST API: 300 requests every 5 minutes

```Json
"x-ratelimit-limit": 300
"x-ratelimit-remaining": 297
"x-ratelimit-reset": 1489791662
```

Code 429 - Retry-After

- 委托数量限制
    为了保持有序的市场，BitMEX 就每个账户的待交易委托数量设置上限。这些限制是：
    - 每个账户每个合约最多 200 笔未执行交易委托数量;
    - 每个账户每个合约最多 10 笔止损交易委托数量;
    - 每个账户每个合约最多 10 笔条件委托数量.

- 最小交易限制
    - 未执行交易订单过多且总值低于 0.0025 XBT ，则每个账户都会被标记为垃圾账户

### 1.2 BitMEX-Delta-Server

> BitMEX-Delta-Server is the easiest way to get started with realtime market data.
> This project is a NodeJS server that automatically subscribes to [BitMEX's RealTime Feed](https://www.bitmex.com/app/wsAPI). 

- [delta-server - README.md](https://github.com/BitMEX/api-connectors/tree/master/official-ws/delta-server)

## 2. Implementation Status

1. Don't modify following files:

    - **include/BitmexApiDef.h** is auto-generated from _swagger.json_ by xJSON's swagger2c tool

    - **json2c/json_decode.cc**  is auto-generated from _include/BitmexApiDef.h_ by xJSON's c-parser tool

2. Use [Boost.Beast](https://github.com/boostorg/beast) for HTTP and WebSocket handling, **restbed** library will be removed.

### 2.1 REST APIs

| Path                             | enum BitmexApiType             | QuoteApi            | TradeApi            | Test Status |
|:---------------------------------|:-------------------------------|---------------------|---------------------|:------------|
|   **APIKey**                      |                                |                     |                     |             |
| POST /apiKey/disable             | BITMEX_TYPE_APIKEY_DISALBE     |                     |SetApiKey            | PASS        |
| POST /apiKey/enable              | BITMEX_TYPE_APIKEY_ENABLE      |                     |SetApiKey            | PASS        |
| GET /apiKey                      | BITMEX_TYPE_APIKEY_GET         |                     |GetApiKey            | PASS        |
| POST /apiKey                     | BITMEX_TYPE_APIKEY_CREATE      |                     |CreateApiKey         | NOT TEST    |
| DELETE /apiKey                   | BITMEX_TYPE_APIKEY_REMOVE      |                     |RemoveApiKey         | NOT TEST    |
|   **Announcement**               |                                |                     |                     |             |
| GET /announcement                | BITMEX_TYPE_ANNOUNCEMENT       |GetAnnouncement      |                     | PASS        |
| GET /announcement/urgent         | BITMEX_TYPE_ANNOUNCEMENT_URGENT|GetAnnouncementUrgent|                     | PASS        |
|   **Chat**                       |                                |                     |                     |             |
| GET /chat                        | BITMEX_TYPE_CHAT_GET_MSG       |                     |GetMessage           | PASS        |
| GET /chat/channels               | BITMEX_TYPE_CHAT_CHANNELS      |                     |GetChannels          | PASS        |
| GET /chat/connected              |BITMEX_TYPE_CHAT_CONNECTED_USERS|                     |GetConnectedUsers    | PASS        |
| POST /chat                       | BITMEX_TYPE_CHAT_SEND_MSG      |                     |SendMessage          | PASS        |
|   **Execution**                  |                                |                     |                     |             |
| GET /execution                   | BITMEX_TYPE_EXECUTION          |                     |QueryExecutions      | NOT TEST    |
| GET /execution/tradeHistory      | BITMEX_TYPE_TRADE_HISTORY      |                     |QueryTradeHistory    | NOT TEST    |
|   **Funding**                    |                                |                     |                     |             |
| GET /funding                     | BITMEX_TYPE_FUNDING_HISTORY    |QueryFundingHistory  |                     | NOT TEST    |
|   **Instrument**                 |                                |                     |                     |             |
| GET /instrument                  | BITMEX_TYPE_INSTRUMENT         |                     |QueryInstruments     | NOT TEST    |
| GET /instrument/active           | BITMEX_TYPE_INSTRUMENT_ACTIVE  |                     |QueryInstrumentsActive|NOT TEST    |
| GET /instrument/activeAndIndices | BITMEX_TYPE_ACTIVE_INDICES     |                     |QueryInstrumentActiveAndIndices|NOT TEST|
| GET /instrument/activeIntervals  | BITMEX_TYPE_ACTIVE_INTERVALS   |                     |QueryInstrumentActiveIntervals |NOT TEST|
| GET /instrument/compositeIndex   | BITMEX_TYPE_COMPOSITE_INDEX    |                     |QueryCompositeIndex  | NOT TEST    |
| GET /instrument/indices          | BITMEX_TYPE_PRICE_INDICES      |                     |QueryPriceIndices    | NOT TEST    |
|   **Insurance**                  |                                |                     |                     |             |
| GET /insurance                   | BITMEX_TYPE_INSURANCE_HISTORY  |QueryInsuranceHistory|                     | NOT TEST    |
|   **Leaderboard**                |                                |                     |                     |             |
| GET /leaderboard                 | BITMEX_TYPE_LEADBOARD          |                     |GetLeadboard         | PASS        |
| GET /leaderboard/name            | BITMEX_TYPE_LEADBOARD_NAME     |                     |GetLeadboardName     | PASS        |
|   **Liquidation**                |                                |                     |                     |             |
| GET /liquidation                 | BITMEX_TYPE_LIQUIDATION_ORDERS |                     |QueryLiquidationOrders|NOT TEST    |
|   **Notification**               |                                |                     |                     |             |
| GET /notification                | BITMEX_TYPE_NOTIFICATION       |                     |GetNotification|     | PASS        |
|   **Order**                      |                                |                     |                     |             |
| PUT /order                       | BITMEX_TYPE_ORDER_AMEND        |                     |UpdateOrder          | NOT TEST    |
| PUT /order/bulk                  | BITMEX_TYPE_ORDERS_AMEND       |                     |UpdateOrders         | NOT TEST    |
| DELETE /order                    | BITMEX_TYPE_CANCEL             |                     |CancelOrder          | NOT TEST    |
| DELETE /order/all                | BITMEX_TYPE_CANCEL_ALL         |                     |CancelOrdersAll      | NOT TEST    |
| POST /order/cancelAllAfter       | BITMEX_TYPE_CANCEL_AFTER       |                     |CancelOrdersAfter    | NOT TEST    |
| POST /order/closePosition        | BITMEX_TYPE_POSITION_CLOSE     |                     |ClosePosition        | Deprecated  |
| GET /order                       | BITMEX_TYPE_ORDERS_QUERY       |                     |QueryOrders          | NOT TEST    |
| POST /order                      | BITMEX_TYPE_ORDER              |                     |PlaceOrder           | NOT TEST    |
| POST /order/bulk                 | BITMEX_TYPE_ORDERS             |                     |PlaceOrdersBulk      | NOT TEST    |
|   **OrderBook**                  |                                |                     |                     |             |
| GET /orderBook/L2                | BITMEX_TYPE_ORDER_BOOK         |QueryOrderBookLevel2 |                     | PASS        |
|   **Position**                   |                                |                     |                     |             |
| GET /position                    | BITMEX_TYPE_POSITION           |                     |QueryPosition        | NOT TEST    |
| POST /position/isolate           | BITMEX_TYPE_POSITION_ISOLATE   |                     |SetPositionIsolateMargin |NOT TEST |
| POST /position/transferMargin    |BITMEX_TYPE_POSITION_TRANSFER_MARGIN|                 |SetPositionTransferMargin|NOT TEST |
| POST /position/leverage          | BITMEX_TYPE_POSITION_LEVERAGE  |                     |SetPositionLeverage  | NOT TEST    |
| POST /position/riskLimit         | BITMEX_TYPE_POSITION_RISK_LIMIT|                     |SetPositionRiskLimit | NOT TEST    |
|   **Quote**                      |                                |                     |                     |             |
| GET /quote                       | BITMEX_TYPE_QUOTE              |QueryQuotes          |                     | PASS        |
| GET /quote/bucketed              | BITMEX_TYPE_QUOTE_HISTORY      |QueryQuotesBucketed  |                     | NOT TEST    |
|   **Schema**                     |                                |                     |                     |             |
| GET /schema                      | BITMEX_TYPE_SCHEMA             |GetSchema            |                     | PASS        |
| GET /schema/websocketHelp        | BITMEX_TYPE_WEBSOCKET_HELP     |GetSchemaWebsocket   |                     | PASS        |
|   **Settlement**                 |                                |                     |                     |             |
| GET /settlement                  | BITMEX_TYPE_SETTLEMENT_HISTORY |                     |QuerySettlement      | NOT TEST    |
|   **Stats**                      |                                |                     |                     |             |
| GET /stats                       | BITMEX_TYPE_STATS              |GetStats             |                     | PASS        |
| GET /stats/history               | BITMEX_TYPE_STATS_HISTORY      |GetStatsHistory      |                     | PASS        |
| GET /stats/historyUSD            | BITMEX_TYPE_STATS_HISTORY_USD  |GetStatsHistoryUsd   |                     | PASS        |
|   **Trade**                      |                                |                     |                     |             |
| GET /trade                       | BITMEX_TYPE_TRADES             |                     |                     |             |
| GET /trade/bucketed              | BITMEX_TYPE_TRADES_HISTORY     |                     |                     |             |
|   **User**                       |                                |                     |                     |             |
| POST /user/cancelWithdrawal      | BITMEX_TYPE_CANCEL_WITHDRAW    |                     |CancelWithdraw       | NOT TEST    |
| GET /user/checkReferralCode      | BITMEX_TYPE_CHECK_REFERRAL_CODE|                     |                     |             |
| POST /user/confirmEmail          | BITMEX_TYPE_CONFIRM_EMAIL      |                     |ConfirmEmail         | NOT TEST    |
| POST /user/confirmEnableTFA      | BITMEX_TYPE_CONFIRM_ENABLE_TFA |                     |ConfirmEnableTFA     | NOT TEST    |
| POST /user/confirmWithdrawal     | BITMEX_TYPE_CONFIRM_WITHDRAW   |                     |ConfirmWithdraw      | NOT TEST    |
| POST /user/disableTFA            | BITMEX_TYPE_CONFIRM_DISABLE_TFA|                     |DisableTFA           | NOT TEST    |
| GET /user                        | BITMEX_TYPE_USER               |                     |QueryUser            | PASS        |
| GET /user/affiliateStatus        | BITMEX_TYPE_AFFILIATE_STATUS   |                     |QueryUserAffiliateStatus|NOT TEST  |
| GET /user/commission             | BITMEX_TYPE_COMMISSION         |                     |QueryUserCommission  | NOT TEST    |
| GET /user/depositAddress         | BITMEX_TYPE_DEPOSIT_ADDRESS    |                     |QueryDepositAddress  | PASS        |
| GET /user/margin                 | BITMEX_TYPE_MARGIN             |                     |QueryUserMargin      | NOT TEST    |
| GET /user/wallet                 | BITMEX_TYPE_WALLET             |                     |QueryWallet          | PASS        |
| GET /user/walletHistory          | BITMEX_TYPE_WALLET_HISTORY     |                     |QueryWalletHistory   | PASS        |
| GET /user/walletSummary          | BITMEX_TYPE_WALLET_SUMMARY     |                     |QueryWalletSummary   | PASS        |
| POST /user/logout                | BITMEX_TYPE_LOGOUT             |                     |Logout               | PASS        |
| POST /user/logoutAll             | BITMEX_TYPE_LOGOUT_ALL         |                     |LogoutAll            | PASS        |
| GET /user/minWithdrawalFee       | BITMEX_TYPE_MIN_WITHDRAWAL_FEE |                     |QueryMinWithdrawalFee| PASS        |
| POST /user/requestEnableTFA      | BITMEX_TYPE_ENABLE_TFA         |                     |EnableTFA            | NOT TEST    |
| POST /user/requestWithdrawal     | BITMEX_TYPE_REQUEST_WITHDRAW   |                     |RequestWithdraw      | NOT TEST    |
| POST /user/preferences           | BITMEX_TYPE_USER_PREFERENCES   |                     |SaveUserPreferences  | NOT TEST    |
| PUT /user                        | BITMEX_TYPE_USER_UPDATE        |                     |UpdateUser           | NOT TEST    |

### 2.2 Websocket APIs

| Operator            |Subject / Topic | Json Schema / Class   | QuoteApi            | TradeApi            | Test Status |
|:--------------------|----------------|:----------------------|---------------------|---------------------|:------------|
| **authKey**         |                |                       |                     |                     | NOT PASS    |
| **authKeyExpires**  |                |                       | Authentication      |                     | NOT PASS    |
| **cancelAllAfter**  |                |                       |                     | CancelOrdersAfter   | NOT TEST    |
| **subscribe** (private) |            |                       | Subscribe           |                     | PASS        |
|                     |privateNotifications| Notification      |                     |                     | NOT TEST    |
|                     |account         | Account               |                     |                     | NOT TEST    |
|                     |wallet          | Wallet                |                     |                     | NOT TEST    |
|                     |affiliate       | Affiliate             |                     |                     | NOT TEST    |
|                     |margin          | Margin                |                     |                     | NOT TEST    |
|                     |position        | Position              |                     |                     | NOT TEST    |
|                     |transact        | Transaction           |                     |                     | NOT TEST    |
|                     |order           | Order                 |                     |                     | NOT TEST    |
|                     |execution       | Execution             |                     |                     | NOT TEST    |
| **subscribe** (public)  |            |                       | Subscribe           |                     | PASS        |
|                     |announcement    | Announcement          |                     |                     | PASS        |
|                     |connected       | ConnectedUsers        |                     |                     | PASS        |
|                     |chat            | Chat                  |                     |                     | PASS        |
|                     |publicNotifications| Notification       |                     |                     | PASS        |
|                     |instrument      | Instrument            |                     |                     | PASS        |
|                     |settlement      | Settlement            |                     |                     | PASS        |
|                     |funding         | Funding               |                     |                     | PASS        |
|                     |insurance       | Insurance             |                     |                     | PASS        |
|                     |liquidation     | Liquidation           |                     |                     | PASS        |
|                     |orderBookL2     | OrderBookL2           |                     |                     | PASS        |
|                     |orderBookL2_25  | OrderBookL2           |                     |                     | PASS        |
|                     |orderBook10     | OrderBookL2           |                     |                     | PASS        |
|                     |quote           | Quote                 |                     |                     | PASS        |
|                     |trade           | Trade                 |                     |                     | PASS        |
|                     |quoteBin1m      |                       |                     |                     | NOT TEST    |
|                     |quoteBin5m      |                       |                     |                     | NOT TEST    |
|                     |quoteBin1h      |                       |                     |                     | NOT TEST    |
|                     |quoteBin1d      |                       |                     |                     | NOT TEST    |
|                     |tradeBin1m      | TradeBin              |                     |                     | NOT TEST    |
|                     |tradeBin5m      | TradeBin              |                     |                     | NOT TEST    |
|                     |tradeBin1h      | TradeBin              |                     |                     | NOT TEST    |
|                     |tradeBin1d      | TradeBin              |                     |                     | NOT TEST    |
| **unsubscribe**     |                |                       | Unsubscribe         |                     | PASS        |
