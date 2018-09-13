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

        //-------------------------- Quote Data ------------------------------
        
        virtual void OnFundingHistory(const void *fundings) {}

        virtual void OnInsuranceHistory(const void *insurances) {}

        //-------------------------- Extra Data ------------------------------

        /**
         * Received site announcements
         * @data array of announcements
         */
        virtual void OnAnnouncement(const void *data) {}

        virtual void OnAnnouncementUrgent(const void *data) {}

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

        //--------------- Funding -------------------
        /**
         * Get funding history
         * \ref OnFundingHistory
         */
        int QueryFundingHistory(const char *symbol, const char *filter, 
                                const char *columns[], size_t count = 100, 
                                size_t start = 0, bool reverse = false, 
                                time_t startTime = 0,
                                time_t endTime = 0);

        /**
         * Get insurance fund history
         * \ref OnInsuranceHistory
         */
        int QueryInsuranceHistory(const char *symbol, const char *filter, 
                                  const char *columns[], size_t count = 100, 
                                  size_t start = 0, bool reverse = false, 
                                  time_t startTime = 0,
                                  time_t endTime = 0);

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

        int GetSchema();

        int GetSchemaWebsocket();

        //---------- Exchange Statistics ------------

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