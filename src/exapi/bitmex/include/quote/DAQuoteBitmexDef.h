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
 * **NOTE**: this file MUST be C header, for json2c's parser and source generator
 */

struct QueryFilterParams {
    const char     *symbol;             ///< Bare series (e.g. "XBU"), or with timestamp ("XBU:monthly")
    const void     *filter;             ///< Generic table filter. Send JSON key/value pairs, such as {"key": "value"}. 
    const char    **columns;            ///< Array of column names to fetch. If omitted, will return all columns.
    size_t          count;              ///< Number of results to fetch. default: 100
    size_t          start;              ///< Starting point for results. default: 0
    bool            reverse;            ///< If true, will sort results newest first. default: false
    time_t          startTime;          ///< Starting date filter for results. format: date-time
    time_t          endTime;            ///< Ending date filter for results. format: date-time

    size_t         _bufsiz;             ///< internal buffer size
    const char     _buf[0];             ///< internal buffer contains strings
};

#endif /* __DAQUOTE_BITMEX_DEF_H__ */
