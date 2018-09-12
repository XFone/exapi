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

        //------------------------- Stream Data ------------------------------
        

        //-------------------------- Extra Data ------------------------------

        /**
         * Current server time
         * @param serverTime in miliseconds time_since_epoch (e.g., "1499827319559")
         */
        virtual void OnServerTime(time_t serverTime) {}

        /** 
         * Current exchange trading rules and symbol information
         */
        virtual void OnExchangeInfo(const char *exinfo) {}

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
        int QueryDepth(const char *symbol, size_t limit);
        int QueryOrderBookLevel2(const char *symbol);

        /** 
         * Recent trades list
         * @param symbol
         * @param limit default 500; max 1000.
         * @return 0 for success, or errno
         */
        int QueryTrades(const char *symbol, size_t limit);

        /** 
         * Historical trades list
         * @param symbol
         * @param limit default 500; max 1000.
         * @param fromId TradeId to fetch from. Default gets most recent trades.
         * @return 0 for success, or errno
         */
        int QueryTradesHistory(const char *symbol, size_t limit, long fromId);
       
        /**
         * 24 hour price change statistics
         * @param symbol
         * @return 0 for success, or errno
         */
        int QueryTicker(const char *symbol);

        int QueryCompositeIndex();

        /**
         * Latest price for a symbol or symbols
         * @param symbol
         * @return 0 for success, or errno
         */
        int QueryPriceIndices(const char *symbol);

        int QueryInstrument();
        int QueryInstrumentsActive();
        int QueryInstrumentActiveAndIndices();
        int QueryInstrumentActiveIntervals();

        int QueryFundingHistory();

        int QueryInsuranceHistory();

        //-------------------------- Extra Data ------------------------------

        //---------- Public Announcement --------------

        int GetAnnouncement();

        int GetAnnouncementUrgent();


        //------ Dynamic Schemata for Developers ------

        int GetSchema();

        int GetSchemaWebsocket();

        //---------- Exchange Statistics --------------

        int GetStats();

        int GetStatsHistory();

        int GetStatsHistoryUsd();

        /**
         * Get the current server time.
         * \ref OnServerTime
         */
        int GetServerTime();


    };

} // namespace exapi