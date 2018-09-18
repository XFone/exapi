/*
 * $Id: $
 * 
 * DAQuoteBitmexDef defines data feed types
 * 
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */

#ifndef __DAQUOTE_BITMEX_DEF_H__
#define __DAQUOTE_BITMEX_DEF_H__

/** @file DAQuoteBitmexDef.h Defines data feed types
 */

/**
 * Query Filter Parameters
 * 
 * See BitmexApiDef.h, following classes use this filter:
 * \ref ExecutionGetParams, \ref ExecutionGetTradeHistoryParams,
 * \ref FundingGetParams, \ref InstrumentGetParams, 
 * \ref InstrumentGetCompositeIndexParams,
 * \ref InsuranceGetParams, \ref LiquidationGetParams,
 * \ref QuoteGetParams, \ref QuoteGetBucketedParams, \ref SettlementGetParams,
 * \ref TradeGetParams, \ref TradeGetBucketedParams
 */
struct QueryFilterParams {
    const char *    symbol;             ///< Bare series (e.g. "XBU"), or with timestamp ("XBU:monthly")
    json_t          filter;             ///< Generic table filter. Send JSON key/value pairs, such as {"key": "value"}. 
    json_t          columns;            ///< Array of column names to fetch. If omitted, will return all columns.
    size_t          count;              ///< Number of results to fetch. default: 100
    size_t          start;              ///< Starting point for results. default: 0
    bool            reverse;            ///< If true, will sort results newest first. default: false
    time_t          startTime;          ///< Starting date filter for results. format: date-time
    time_t          endTime;            ///< Ending date filter for results. format: date-time
};

#endif /* __DAQUOTE_BITMEX_DEF_H__ */
