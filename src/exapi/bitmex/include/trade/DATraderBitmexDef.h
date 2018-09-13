/*
 * $Id: $
 * 
 * DATraderBitmexDef defines trade request and response data types
 * 
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file DATraderBitmexDef.h Defines trade request and response data types
 */

#include "trade/ITraderApi.h"

namespace exapi {

    struct OrderParams {
        const char      *symbol;            ///< Instrument symbol. e.g. 'XBTUSD'.
        const char      *side;              ///< Order side. Valid options: Buy, Sell. Defaults to 'Buy' unless orderQty or  simpleOrderQty is negative.
        d_quantity_t    simpleOrderQty;     ///< Underlying instrument (i.e. Bitcoin). format: double
        quantity_t      orderQty;           ///< Order quantity in units of the instrument (i.e. contracts). format: int32
        d_price_t       price;              ///< Optional limit price for 'Limit', 'StopLimit', and 'LimitIfTouched' orders. format: double
        quantity_t      displayQty;         ///< Optional quantity to display in the book. Use 0 for a fully hidden order. format: int32
        d_price_t       stopPx;             ///< Optional trigger price for 'Stop', 'StopLimit', 'MarketIfTouched', and 'LimitIfTouched' orders. Use a price below the current price for stop-sell orders and buy-if-touched orders. Use  execInst of 'MarkPrice' or 'LastPrice' to define the current price used for triggering. format: double
        const char     *clOrdID;            ///< Optional Client Order ID. This clOrdID will come back on the order and any related executions.
        const char     *clOrdLinkID;        ///< Optional Client Order Link ID for contingent orders.
        d_price_t       pegOffsetValue;     ///< Optional trailing offset from the current price for 'Stop', 'StopLimit', 'MarketIfTouched', and 'LimitIfTouched' orders; use a negative offset for stop-sell orders and buy-if-touched orders. Optional offset from the peg price for 'Pegged' orders. format: double
        const char     *pegPriceType;       ///< Optional peg price type. Valid options: LastPeg, MidPricePeg, MarketPeg, PrimaryPeg, TrailingStopPeg.
        const char     *ordType;            ///< Order type. Valid options: Market, Limit, Stop, StopLimit, MarketIfTouched, LimitIfTouched, MarketWithLeftOverAsLimit, Pegged. Defaults to 'Limit' when price is specified. Defaults to 'Stop' when stopPx is specified. Defaults to 'StopLimit' when price and stopPx are specified. default: Limit
        const char     *timeInForce;        ///< Time in force. Valid options: Day, GoodTillCancel, ImmediateOrCancel, FillOrKill. Defaults to 'GoodTillCancel' for 'Limit', 'StopLimit', 'LimitIfTouched', and 'MarketWithLeftOverAsLimit' orders.
        const char     *execInst;           ///< Optional execution instructions. Valid options: ParticipateDoNotInitiate, AllOrNone, MarkPrice, IndexPrice, LastPrice, Close, ReduceOnly, Fixed. 'AllOrNone' instruction requires  displayQty to be 0. 'MarkPrice', 'IndexPrice' or 'LastPrice' instruction valid for 'Stop', 'StopLimit', 'MarketIfTouched', and 'LimitIfTouched' orders.
        const char     *contingencyType;    ///< Optional contingency type for use with clOrdLinkID. Valid options: OneCancelsTheOther, OneTriggersTheOther, OneUpdatesTheOtherAbsolute, OneUpdatesTheOtherProportional.
        const char     *text;               ///< Optional order annotation. e.g. 'Take profit'.

        size_t         _bufsiz;             ///< internal buffer size
        const char     _buf[0];             ///< internal buffer contains strings
    };

    /**
     * Amend order parameters
     * ref [the FIX Spec](https://www.onixs.biz/fix-dictionary/5.0.SP2/msgType_D_68.html)
     */
    struct OrderAmendParams {
        const char     *orderId;            ///< Order ID
        const char     *origClOrdID;        ///< Client Order ID. See POST /order
        const char     *clOrdID;            ///< Optional new Client Order ID, requires origClOrdID.
        d_quantity_t    simpleOrderQty;     ///< Optional order quantity in units of the underlying instrument (i.e. Bitcoin). 
        quantity_t      orderQty;           ///< Optional order quantity in units of the instrument (i.e. contracts)
        d_quantity_t    simpleLeavesQty;    ///< Optional leaves quantity in units of the underlying instrument (i.e. Bitcoin)
        quantity_t      leavesQty;          ///< Optional leaves quantity in units of the instrument (i.e. contracts)
        d_price_t       price;              ///< Optional limit price for 'Limit', 'StopLimit', and 'LimitIfTouched' orders.
        d_price_t       stopPx;             ///< Optional trigger price for 'Stop', 'StopLimit', 'MarketIfTouched', and 'LimitIfTouched' orders. Use a price below the current price for stop-sell orders and buy-if-touched orders.
        double          pegOffsetValue;     ///< Optional trailing offset from the current price for 'Stop', 'StopLimit', 'MarketIfTouched', and 'LimitIfTouched' orders; use a negative offset for stop-sell orders and buy-if-touched orders. Optional offset from the peg price for 'Pegged' orders.
        const char     *text;

        size_t         _bufsiz;             ///< internal buffer size
        const char     _buf[0];             ///< internal buffer contains strings
    };

    /**
     * Position query parameters
     * ref [the FIX Spec](http://www.onixs.biz/fix-dictionary/5.0.SP2/msgType_AP_6580.html)
     */
    struct QueryPositionParams {
        const void     *filter;             ///< Generic table filter. Send JSON key/value pairs, such as {"key": "value"}. 
        const char    **columns;            ///< Array of column names to fetch. If omitted, will return all columns.
        size_t          count;              ///< Number of results to fetch. default: 100
    };

} // namespace exapi