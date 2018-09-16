/*
 * $Id: $
 * 
 * DAQuoteBitmexApi and DAQuoteBitmexSpi class declaration
 * 
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file DAQuoteBitmexApi.h Bitmex quotation client api.
 */

#if defined(SHARELIB) && defined(WIN32)
# ifdef SHARELIB_API_EXPORT
#  define API_EXPORT __declspec(dllexport)
# else
#  define API_EXPORT __declspec(dllimport)
# endif
#else
# define  API_EXPORT 
#endif

#include "quote/IQuoteApi.h"

namespace exapi {

    /* C header without namespace */
    #include "quote/DAQuoteBitmexDef.h"

    /**
     * DAQuoteBitmexSpi
     * Bitmex quotation interface for async callback
     */
    class DAQuoteBitmexSpi : public IQuoteSpi {
    public:
        virtual ~DAQuoteBitmexSpi() {}

        /**
         * On server connected (not used in HTTP/1.1)
         * @param addr server address
         */
        virtual void OnConnected(const char *addr) {}

        /**
         * On server disconnected (not used in HTTP/1.1)
         * @param addr server address
         */
        virtual void OnDisconnected(const char *addr) {}

        /**
         * New quotation data is comming
         * @param quoteType quote type
         * @param pQuoteData union of quotation data
         */
        virtual void OnQuoteUpdated(QuoteApiType quoteType, void *pQuoteData) {}
    
        /**
         * @param quoteType quote type
         * @param psymbol quote symbol
         * @param status symbol subscription status
         */
        virtual void OnSymbolSubscribed(QuoteApiType quoteType, const char *pSymbol, unsigned status) {}

        /**
         * @param quoteType quote type
         * @param psymbol quote symbol
         * @param status symbol subscription status
         */
        virtual void OnSymbolUnsubscribed(QuoteApiType quoteType, const char *pSymbol, unsigned status) {}

        //-------------------------- Quote Data ------------------------------
        
        virtual void OnOrderBook(const void *l2data) {}

        //------------------------- Funding callbacks ------------------------

        virtual void OnFundingHistory(const void *fundings) {}

        virtual void OnInsuranceHistory(const void *insurances) {}

        //-------------------------- Trades Data -----------------------------

        virtual void OnTrades(const void *trades) {}

        virtual void OnTradesBucketd(const void *tradebin) {}

        //-------------------------- Extra Data ------------------------------

        /**
         * Received site announcements
         * @data array of announcements
         */
        virtual void OnAnnouncement(const void *data) {}

        virtual void OnAnnouncementUrgent(const void *data) {}

        virtual void OnSchema(const void *schema) {}

        virtual void OnSchemaWebsocket(const void *schema) {}

        virtual void OnStats(const void *stats) {}

        virtual void OnStatsHistory(const void *statsHist) {}

        virtual void OnStatsHistoryUsd(const void *statsUsd) {}

    };

    /**
     * DAQuoteBitmexApi
     * Bitmex quotation service
     */
    class API_EXPORT DAQuoteBitmexApi : public IQuoteApi {
    public:
        virtual ~DAQuoteBitmexApi() {}

        /**
         * Create TraderApi
         * @param api_key user should applied an api-key from bitmex.com
         * @param secret_key key for signing parameter
         * @param dpath the local directory to save quotation data, default is work path
         * @return the DAQuoteOkexApi instance
         */
        static DAQuoteBitmexApi *CreateApi(const char *api_key, const char *secret_key, const char *dpath = "");
        
        /**
         * Delete current instance and free all resources
         */
        virtual void Dispose() = 0;

        /**
         * Initialized runtime and worker threads, start receiving events
         */
        virtual void Init() = 0;
        
        /**
         * Waiting for stopping of worker threads
         * @return the exit value of thread
         */
        virtual int Join() = 0;

        //------------------------  Quote Data -------------------------------

        /** 
         * Order book
         * @param symbol e.g., "ETHBTC"
         * @param limit default 100; max 1000. Valid limits:[5, 10, 20, 50, 100, 500, 1000]
         * @return 0 for success, or errno
         */
        int QueryDepth(const char *symbol, size_t limit) {
            return QueryOrderBookLevel2(symbol, 0);
        }

        /**
         * Get current orderbook in vertical format. 
         * \ref OnOrderBook
         * @param symbol Instrument symbol. Send a series (e.g. XBT) to get data for the nearest contract in that series.
         * @param depth Orderbook depth per side. Send 0 for full depth. default: 25
         */
        int QueryOrderBookLevel2(const char *symbol, size_t depth);

        /**
         * Get Quotes
         * \ref OnQuotes
         */
        int QueryQuotes(const QueryFilterParams &params);

        /**
         * Get previous quotes in time buckets.
         * \ref OnQuotesBucketed
         * @param binSize Time interval to bucket by. Available options: [1m,5m,1h,1d]. default: 1m
         * @param partial If true, will send in-progress (incomplete) bins for the current time period. default: false
         * @param params query and filter parameters
         */
        int QueryQuotesBucketed(const char *binSize, bool partial,
                                const QueryFilterParams &params);

        //---------------------------- Funding -------------------------------

        /**
         * Get funding history
         * \ref OnFundingHistory
         * @param params query or filter parameters
         */
        int QueryFundingHistory(const QueryFilterParams &params);

        /**
         * Get insurance fund history
         * \ref OnInsuranceHistory
         * @param params query or filter parameters
         */
        int QueryInsuranceHistory(const QueryFilterParams &params);

        //-------------------------- Trades Data------------------------------

        /**
         * Get Trades.
         * \ref OnQueryTrades
         */
        int QueryTrades(const QueryFilterParams &params);

        /**
         * Get previous trades in time buckets.
         * \ref OnQueryTradesBucketd
         */
        int QueryTradesBucketed(const char *binSize, bool partial,
                                const QueryFilterParams &params);

        //-------------------------- Extra Data ------------------------------

        //---------- Public Announcement ------------

        /**
         * Get site announcements.
         * \ref OnAnnouncement
         * @param columns array of column names to fetch
         */
        int GetAnnouncement(const char *columns[] = nullptr);

        /**
         * Get urgent (banner) announcements.
         * \ref OnAnnouncementUrgent
         */
        int GetAnnouncementUrgent();


        //------ Dynamic Schemata for Developers ----

        /**
         * Get model schemata for data objects returned by this API.
         * \ref OnSchema
         * @param model model filter. If omitted, will return all models.
         */
        int GetSchema(const char *model = nullptr);

        /**
         * Returns help text & subject list for websocket usage.
         * \ref OnSchemaWebsocket
         */
        int GetSchemaWebsocket();

        //---------- Exchange Statistics ------------

        /**
         * Get exchange-wide and per-series turnover and volume statistics.
         * \ref OnStats
         */
        int GetStats();

        /**
         * Get historical exchange-wide and per-series turnover and volume statistics. 
         * \ref OnStatsHistory
         */
        int GetStatsHistory();

        /**
         * Get a summary of exchange statistics in USD. 
         * \ref OnStatsHistoryUsd
         */
        int GetStatsHistoryUsd();

    };

} // namespace exapi