# API

<!-- markdownlint-disable MD004 MD007 MD012 MD036 -->
_last updated by xiaofeng 2018-08-23_

## Instro

- [Binance](https://github.com/binance-exchange/binance-official-api-docs)

Name | Description
------------ | ------------
[rest-api.md](https://github.com/binance-exchange/binance-official-api-docs/blob/master//rest-api.md) | Details on the Rest API (/api)
[errors.md](https://github.com/binance-exchange/binance-official-api-docs/blob/master//errors.md) | Descriptions of possible error messages from the Rest API
[web-socket-streams.md](https://github.com/binance-exchange/binance-official-api-docs/blob/master//web-socket-streams.md) | Details on available streams and payloads
[user-data-stream.md](https://github.com/binance-exchange/binance-official-api-docs/blob/master//user-data-stream.md) | Details on the dedicated account stream
[wapi-api.md](https://github.com/binance-exchange/binance-official-api-docs/blob/master//wapi-api.md) | Details on the Withdrawal API (/wapi)

### LIMITS

* The `/api/v1/exchangeInfo` `rateLimits` array contains objects related to the exchange's `REQUEST_WEIGHT` and `ORDER` rate limits.
* A 429 will be returned when either rate limit is violated.
* Each route has a `weight` which determines for the number of requests each endpoint counts for. Heavier endpoints and endpoints that do operations on multiple symbols will have a heavier `weight`.
* When a 429 is recieved, it's your obligation as an API to back off and not spam the API.
* **Repeatedly violating rate limits and/or failing to back off after receiving 429s will result in an automated IP ban (http status 418).**
* IP bans are tracked and **scale in duration** for repeat offenders, **from 2 minutes to 3 days**.

### ENUM definitions

**Symbol status:**

* PRE_TRADING
* TRADING
* POST_TRADING
* END_OF_DAY
* HALT
* AUCTION_MATCH
* BREAK

**Symbol type:**

* SPOT

**Order status:**

* NEW
* PARTIALLY_FILLED
* FILLED
* CANCELED
* PENDING_CANCEL (currently unused)
* REJECTED
* EXPIRED

**Order types:**

* LIMIT
* MARKET
* STOP_LOSS
* STOP_LOSS_LIMIT
* TAKE_PROFIT
* TAKE_PROFIT_LIMIT
* LIMIT_MAKER

**Order side:**

* BUY
* SELL

**Time in force:**

* GTC
* IOC
* FOK

**Kline/Candlestick chart intervals:**

m -> minutes; h -> hours; d -> days; w -> weeks; M -> months

**Rate limiters (rateLimitType)**

* REQUESTS_WEIGHT
* ORDERS

**Rate limit intervals**

* SECOND
* MINUTE
* DAY

## C++ Reference API

- [Binance C++ API](https://github.com/binance-exchange/binacpp)