# API

<!-- markdownlint-disable MD004 MD007 MD012 MD036 -->
_last updated by xiaofeng 2018-09-07_

## Instro

- [BitMEX](https://github.com/BitMEX/api-connectors)

### BitMEX-Delta-Server

> BitMEX-Delta-Server is the easiest way to get started with realtime market data.
> This project is a NodeJS server that automatically subscribes to [BitMEX's RealTime Feed](https://www.bitmex.com/app/wsAPI). 

- [delta-server - README.md](./github/api-connectors/official-ws/delta-server/README.md)


## Implement Status

| Path                             | enum BitmexApiType             | QuoteApi | TradeApi | Test Status |
|:---------------------------------|:-------------------------------|----------|----------|:-----------:|
|     APIKey                       |                                |          |          |             |
| POST /apiKey/disable             | BITMEX_TYPE_APIKEY_DISALBE     |          |          |             |
| POST /apiKey/enable              | BITMEX_TYPE_APIKEY_ENABLE      |          |          |             |
| GET /apiKey                      | BITMEX_TYPE_APIKEY_GET         |          |          |             |
| POST /apiKey                     | BITMEX_TYPE_APIKEY_CREATE      |          |          |             |
| DELETE /apiKey                   | BITMEX_TYPE_APIKEY_REMOVE      |          |          |             |
|     Announcement                 |                                |          |          |             |
| GET /announcement                | BITMEX_TYPE_ANNOUNCEMENT       |          |          |             |
| GET /announcement/urgent         | BITMEX_TYPE_ANNOUNCEMENT_URGENT|          |          |             |
|     Chat                         |                                |          |          |             |
| GET /chat                        | BITMEX_TYPE_CHAT_GET_MSG       |          |          |             |
| GET /chat/channels               | BITMEX_TYPE_CHAT_CHANNELS      |          |          |             |
| GET /chat/connected              |BITMEX_TYPE_CHAT_CONNECTED_USERS|          |          |             |
| POST /chat                       | BITMEX_TYPE_CHAT_SEND_MSG      |          |          |             |
|     Execution                    |                                |          |          |             |
| GET /execution                   | BITMEX_TYPE_EXECUTION          |          |          |             |
| GET /execution/tradeHistory      | BITMEX_TYPE_TRADE_HISTORY      |          |          |             |
|     Funding                      |                                |          |          |             |
| GET /funding                     | BITMEX_TYPE_FUNDING_HISTORY    |          |          |             |
|     Instrument                   |                                |          |          |             |
| GET /instrument                  | BITMEX_TYPE_INSTRUMENT         |          |          |             |
| GET /instrument/active           | BITMEX_TYPE_INSTRUMENT_ACTIVE  |          |          |             |
| GET /instrument/activeAndIndices | BITMEX_TYPE_ACTIVE_INDICES     |          |          |             |
| GET /instrument/activeIntervals  | BITMEX_TYPE_ACTIVE_INTERVALS   |          |          |             |
| GET /instrument/compositeIndex   | BITMEX_TYPE_COMPOSITE_INDEX    |          |          |             |
| GET /instrument/indices          | BITMEX_TYPE_PRICE_INDICES      |          |          |             |
|     Insurance                    |                                |          |          |             |
| GET /insurance                   | BITMEX_TYPE_INSURANCE_HISTORY  |          |          |             |
|     Leaderboard                  |                                |          |          |             |
| GET /leaderboard                 | BITMEX_TYPE_LEADBOARD          |          |          |             |
| GET /leaderboard/name            | BITMEX_TYPE_LEADBOARD_NAME     |          |          |             |
|     Liquidation                  |                                |          |          |             |
| GET /liquidation                 | BITMEX_TYPE_LIQUIDATION_ORDERS |          |          |             |
|     Notification                 |                                |          |          |             |
| GET /notification                | BITMEX_TYPE_NOTIFICATION       |          |          |             |
|     Order                        |                                |          |          |             |
| PUT /order                       | BITMEX_TYPE_ORDER_AMEND        |          |          |             |
| PUT /order/bulk                  | BITMEX_TYPE_ORDERS_AMEND       |          |          |             |
| DELETE /order                    | BITMEX_TYPE_CANCEL             |          |          |             |
| DELETE /order/all                | BITMEX_TYPE_CANCEL_ALL         |          |          |             |
| POST /order/cancelAllAfter       | BITMEX_TYPE_CANCEL_AFTER       |          |          |             |
| POST /order/closePosition        | BITMEX_TYPE_POSITION_CLOSE     |          |          |             |
| GET /order                       | BITMEX_TYPE_ORDERS_QUERY       |          |          |             |
| POST /order                      | BITMEX_TYPE_ORDER              |          |          |             |
| POST /order/bulk                 | BITMEX_TYPE_ORDERS             |          |          |             |
|     OrderBook                    |                                |          |          |             |
| GET /orderBook/L2                | BITMEX_TYPE_ORDER_BOOK         |          |          |             |
|     Position                     |                                |          |          |             |
| GET /position                    | BITMEX_TYPE_POSITION           |          |          |             |
| POST /position/isolate           | BITMEX_TYPE_POSITION_ISOLATE   |          |          |             |
| POST /position/transferMargin    |BITMEX_TYPE_POSITION_TRANSFER_MARGIN|      |          |             |
| POST /position/leverage          | BITMEX_TYPE_POSITION_LEVERAGE  |          |          |             |
| POST /position/riskLimit         | BITMEX_TYPE_POSITION_RISK_LIMIT|          |          |             |
|     Quote                        |                                |          |          |             |
| GET /quote                       | BITMEX_TYPE_QUOTE              |          |          |             |
| GET /quote/bucketed              | BITMEX_TYPE_QUOTE_HISTORY      |          |          |             |
|     Schema                       |                                |          |          |             |
| GET /schema                      | BITMEX_TYPE_SCHEMA             |          |          |             |
| GET /schema/websocketHelp        | BITMEX_TYPE_WEBSOCKET_HELP     |          |          |             |
|     Settlement                   |                                |          |          |             |
| GET /settlement                  | BITMEX_TYPE_SETTLEMENT_HISTORY |          |          |             |
|     Stats                        |                                |          |          |             |
| GET /stats                       | BITMEX_TYPE_STATS              |          |          |             |
| GET /stats/history               | BITMEX_TYPE_STATS_HISTORY      |          |          |             |
| GET /stats/historyUSD            | BITMEX_TYPE_STATS_HISTORY_USD  |          |          |             |
|     Trade                        |                                |          |          |             |
| GET /trade                       | BITMEX_TYPE_TRADES             |          |          |             |
| GET /trade/bucketed              | BITMEX_TYPE_TRADES_HISTORY     |          |          |             |
|     User                         |                                |          |          |             |
| POST /user/cancelWithdrawal      | BITMEX_TYPE_CANCEL_WITHDRAW    |          |          |             |
| GET /user/checkReferralCode      | BITMEX_TYPE_CHECK_REFERRAL_CODE|          |          |             |
| POST /user/confirmEmail          | BITMEX_TYPE_CONFIRM_EMAIL      |          |          |             |
| POST /user/confirmEnableTFA      | BITMEX_TYPE_CONFIRM_ENABLE_TFA |          |          |             |
| POST /user/confirmWithdrawal     | BITMEX_TYPE_CONFIRM_WITHDRAW   |          |          |             |
| POST /user/disableTFA            | BITMEX_TYPE_CONFIRM_DISABLE_TFA|          |          |             |
| GET /user                        | BITMEX_TYPE_USER               |          |          |             |
| GET /user/affiliateStatus        | BITMEX_TYPE_AFFILIATE_STATUS   |          |          |             |
| GET /user/commission             | BITMEX_TYPE_COMMISSION         |          |          |             |
| GET /user/depositAddress         | BITMEX_TYPE_DEPOSIT_ADDRESS    |          |          |             |
| GET /user/margin                 | BITMEX_TYPE_MARGIN             |          |          |             |
| GET /user/wallet                 | BITMEX_TYPE_WALLET             |          |          |             |
| GET /user/walletHistory          | BITMEX_TYPE_WALLET_HISTORY     |          |          |             |
| GET /user/walletSummary          | BITMEX_TYPE_WALLET_SUMMARY     |          |          |             |
| POST /user/logout                | BITMEX_TYPE_LOGOUT             |          |          |             |
| POST /user/logoutAll             | BITMEX_TYPE_LOGOUT_ALL         |          |          |             |
| GET /user/minWithdrawalFee       | BITMEX_TYPE_MIN_WITHDRAWAL_FEE |          |          |             |
| POST /user/requestEnableTFA      | BITMEX_TYPE_ENABLE_TFA         |          |          |             |
| POST /user/requestWithdrawal     | BITMEX_TYPE_REQUEST_WITHDRAW   |          |          |             |
| POST /user/preferences           | BITMEX_TYPE_USER_PREFERENCES   |          |          |             |
| PUT /user                        | BITMEX_TYPE_USER_UPDATE        |          |          |             |


