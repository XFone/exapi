/*
 * $Id: $
 * 
 * Interfaces for quotation and data feed service
 * 
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file IQuoteApi.h Interfaces for quotation and data feed service.
 */

// use __declspec(novtable) in MSVC
#ifndef _INTERFACE_
# define _INTERFACE_         struct
#endif

#include <cstddef>
#include <cstdint>

namespace exapi {

#ifndef DEF_TRADE_TYPES
#define DEF_TRADE_TYPES
    typedef int32_t quantity_t;
    typedef int32_t price_t;                /**< d_price_t  * PRICE_BASE */
    typedef int64_t amount_t;               /**< d_amount_t * PRICE_BASE */
    typedef double  d_quantity_t;           /**< quantity in units of the underlying instrument */
    typedef double  d_price_t;
    typedef double  d_amount_t;
    typedef int64_t volume_t;
    typedef int64_t timestamp_t;            /**< timestamp in millisecond(ms) */
#endif // DEF_TRADE_TYPES

    /**
     * QuoteApiType
     * {QuoteApiType} = ({ExType} << 16) + {server_specific_api_enum_value}
     */
    class QuoteApiType {
    private:
        int m_apitype;

    public:
      #if (__cplusplus >= 199711L)
        QuoteApiType() = delete;
        enum ExType : int { // C++11 required
      #else
        enum ExType {
      #endif
            EX_TYPE_BINANCE     = 0x010000,
            EX_TYPE_BITMEX      = 0x020000,
            EX_TYPE_OKEX        = 0x030000,
            EX_TYPE_HUOBI       = 0x040000
        };

        QuoteApiType(const QuoteApiType &r) { m_apitype = r.m_apitype; }

        QuoteApiType(int apiType) : m_apitype(apiType) {}

        QuoteApiType(int type, ExType ex) : m_apitype(ex + type) {}

        operator int() const { return m_apitype; }
    };

    /**
     * IQuoteSpi
     * Common quotation interface for async callback
     */
    _INTERFACE_ IQuoteSpi {
    public:
      // C++0x or C++11
      #if (__cplusplus >= 199711L)
        virtual ~IQuoteSpi() = default;
      #endif

        /**
         * On server connected (not used in HTTP/1.1)
         * @param addr server address
         */
        virtual void OnConnected(const char *addr) = 0;

        /**
         * On server disconnected (not used in HTTP/1.1)
         * @param addr server address
         */
        virtual void OnDisconnected(const char *addr) = 0;

        /**
         * New quotation data is comming
         * @param quoteType quote type
         * @param pQuoteData union of quotation data
         */
        virtual void OnQuoteUpdated(QuoteApiType quoteType, void *pQuoteData) = 0;
    
        /**
         * @param quoteType quote type
         * @param psymbol quote symbol
         * @param status symbol subscription status
         */
        virtual void OnSymbolSubscribed(QuoteApiType quoteType, const char *pSymbol, unsigned status) = 0;

        /**
         * @param quoteType quote type
         * @param psymbol quote symbol
         * @param status symbol subscription status
         */
        virtual void OnSymbolUnsubscribed(QuoteApiType quoteType, const char *pSymbol, unsigned status) = 0;
    };

    /**
     * IQuoteApi
     * Common quotation and data feed service
     */
    _INTERFACE_ IQuoteApi {
    public:
      // C++0x or C++11
      #if (__cplusplus >= 199711L)
        virtual ~IQuoteApi() = default;
      #endif

        /**
         * Connect to quotation or data feed server(s)
         * @param slist server list, last element should be '\0'
         * @param pEventHandler instance of IQuoteSpi for callback
         * @return 0 for sucess, or errono
         */
        virtual int ConnServer(const char *slist[], IQuoteSpi *pEventHandler) = 0;

        /**
         * Disconnect from data feed servers
         * @param addr single server adress, or all servers if no given
         * @return 0 for sucess, or errono
         */
        virtual int DisConnServer(const char *addr = nullptr) = 0;

        /**
         * Subscribe to a stream with filter (or symbol)
         * @param filter_symbol filter or symbol
         * @return 0 for sucess, or errono
         */
        virtual int Subscribe(const char *filter_symbol) = 0;

        /**
         * Unsubscribe from a stream with filter (or symbol)
         * @param filter_symbol filter or symbol, null for unsubscribe all
         * @return 0 for sucess, or errono
         */
        virtual int Unsubscribe(const char *filter_symbol = nullptr) = 0;

        /**
         * Get instant quote data with given conditions
         * @param quoteType quote type
         * @param symbol
         * @param range
         * @param max_size
         * @return 0 for sucess, or errono
         */
        virtual int GetQuote(QuoteApiType quoteType, const char *symbol, const char *range = nullptr, size_t max_size = 0) = 0;

    };

} // namespace exapi