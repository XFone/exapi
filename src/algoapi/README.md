# Common Trading API

<!-- markdownlint-disable MD004 MD007 MD012 MD036 -->

- [Common Trading API](#common-trading-api)
    - [Definition](#definition)
    - [REST API and WebSocket API](#rest-api-and-websocket-api)
        - [1. OKCoin OKEx](#1-okcoin-okex)
        - [2. Binance](#2-binance)
        - [3. Huobi](#3-huobi)
    - [Protobuf Files](#protobuf-files)

## Definition

|-------------|---------------------|--------------------|
| name        | Chinese             | Description        |
|:------------|:--------------------|:-------------------|
| order_group | 母单号               |                    |
| order_id    | 子单号               |                    |
| orderno     | 委托号               ｜ matched_id        |
|-------------|---------------------|--------------------|


## REST API and WebSocket API

### 1. OKCoin OKEx

- github: [okcoin-okex](https://github.com/okcoin-okex/API-docs-OKEx.com)

- agent: [../dcapi/okex](../exapi/okex/README.md)

### 2. Binance

- github: [binance-exchange](https://github.com/binance-exchange/binance-official-api-docs)

- agent: [../dcapi/binance](../exapi/binance/README.md)

### 3. Huobi

- github: [huobiapi](https://github.com/huobiapi/API_Docs/wiki)

- agent: [../dcapi/huobi](../exapi/huobi/README.md)

## Protobuf Files

- 数字货币 **行情** [coin_quote](./protos/coin_quote.proto)
  
- 数字货币 **交易** [coin_trade](./protos/coin_trade.proto)