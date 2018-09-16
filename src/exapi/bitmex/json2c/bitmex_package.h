/*
 * $Id: $
 *
 * BitMex package definition
 *
 * Copyright (c) 2014-2018 Zerone.IO . All rights reserved.
 *
 * $Log: $ *
 */

#ifndef _BITMEX_PACKAGE_H_
#define _BITMEX_PACKAGE_H_

#ifndef USE_ZPROTO
#include "quote/IQuoteApi.h"
using namespace exapi;
#else /* USE_ZPROTO */
typedef long          int32_t;
typedef long long     int64_t;
typedef unsigned long size_t;

// DEF_TRADE_TYPES
typedef int32_t quantity_t;
typedef int32_t price_t;                /**< d_price_t  * PRICE_BASE */
typedef int64_t amount_t;               /**< d_amount_t * PRICE_BASE */
typedef double  d_quantity_t;           /**< quantity in units of the underlying instrument */
typedef double  d_price_t;
typedef double  d_amount_t;
typedef int64_t volume_t;
typedef int64_t timestamp_t;            /**< timestamp in millisecond(ms) */
#endif // USE_ZPROTO

#include "quote/DAQuoteBitmexDef.h"
#include "trade/DATraderBitmexDef.h"

/* Trace package entry */
union CODE_MAIN {
    QueryFilterParams  queryFilterParams;
    OrderParams        qrderParams;
    OrderAmendParams   drderAmendParams;
    /* More modificator here */
};

#endif /* _BITMEX_PACKAGE_H_ */

