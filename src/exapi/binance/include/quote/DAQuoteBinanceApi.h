/*
 * $Id: $
 * 
 * DAQuoteBinanceApi and DAQuoteBinanceSpi class declaration
 * 
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file DAQuoteBinanceApi.h Binance quotation client api.
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
     * DAFilterBinance
     * Filters (PRICE_FILTER, LOT_SIZE, MIN_NOTIONAL, MAX_NUM_ORDERS, MAX_NUM_ALGO_ORDERS, ICEBERG_PARTS)
     * Exchange Filters (EXCHANGE_MAX_NUM_ORDERS, EXCHANGE_MAX_NUM_ALGO_ORDERS)
     */
    class DAFilterBinance {
    public:
        /** The PRICE_FILTER defines the price rules for a symbol */
        struct PRICE_FILTER {
            d_price_t minPrice;
            d_price_t maxPrice;
            d_price_t tickSize;
        };

        /** The LOT_SIZE filter defines the quantity rules for a symbol */
        struct LOT_SIZE {
            quantity_t minQty;
            quantity_t maxQty;
            quantity_t stepSize;
        };

        /** The MIN_NOTIONAL filter defines the minimum notional value allowed 
         *  for an order on a symbol
         */
        struct MIN_NOTIONAL {
            d_amount_t minNotional;
        };

        /** The MAX_NUM_ORDERS filter defines the maximum number of orders an 
         * account is allowed to have open on a symbol
         */
        struct MAX_NUM_ORDERS {
            size_t limit;
        };

        /** The MAX_ALGO_ORDERS filter defines the maximum number of "algo" 
         * orders an account is allowed to have open on a symbol
         */
        struct MAX_NUM_ALGO_ORDERS {
            size_t maxNumAlgoOrders;
        };

        /** The ICEBERG_PARTS filter defines the maximum parts an iceberg order
         * can have
         */
        struct ICEBERG_PARTS {
            size_t limit;
        };

        /** The MAX_NUM_ORDERS filter defines the maximum number of orders an 
         * account is allowed to have open on the exchange
         */
        struct EXCHANGE_MAX_NUM_ORDERS {
            size_t limit;
        };

        /** The MAX_ALGO_ORDERS filter defines the maximum number of "algo" 
         * orders an account is allowed to have open on the exchange
         */
        struct EXCHANGE_MAX_NUM_ALGO_ORDERS {
            size_t limit;
        };

    protected:
        bool                            has_pf;
        bool                            has_ls;
        bool                            has_mn;
        bool                            has_no;
        bool                            has_ao;
        bool                            has_ip;
        bool                            has_ex_o;
        bool                            has_ex_ao;

        PRICE_FILTER                    m_pf;
        LOT_SIZE                        m_ls;
        MIN_NOTIONAL                    m_mn;
        MAX_NUM_ORDERS                  m_no;
        MAX_NUM_ALGO_ORDERS             m_ao;
        ICEBERG_PARTS                   m_ip;
        EXCHANGE_MAX_NUM_ORDERS         m_ex_o;
        EXCHANGE_MAX_NUM_ALGO_ORDERS    m_ex_ao;

    public:
        DAFilterBinance() {}
    };

    /**
     * DAQuoteBinanceSpi
     * Binance quotation interface for async callback
     */
    class DAQuoteBinanceSpi : public IQuoteSpi {
    public:
        virtual ~DAQuoteBinanceSpi() {}

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
        
        virtual void OnStartUserDataStream(const char *listenKey) {}

        virtual void OnCloseUserDataStream(const char *listenKey) {}

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
     * DAQuoteBinanceApi
     * Binance quotation service
     */
    class API_EXPORT DAQuoteBinanceApi : public IQuoteApi {
    public:
        virtual ~DAQuoteBinanceApi() {}

        /**
         * Create TraderApi
         * @param api_key user should applied an api-key from binance.com
         * @param secret_key key for signing parameter
         * @param dpath the local directory to save quotation data, default is work path
         * @return the DAQuoteOkexApi instance
         */
        static DAQuoteBinanceApi *CreateApi(const char *api_key, const char *secret_key, const char *dpath = "");
        
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

        //------------------------  Spot Data --------------------------------

        /** 
         * Order book
         * @param symbol e.g., "ETHBTC"
         * @param limit default 100; max 1000. Valid limits:[5, 10, 20, 50, 100, 500, 1000]
         * @return 0 for success, or errno
         */
        int QueryDepth(const char *symbol, size_t limit);

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
         * Get compressed, aggregate trades list
         * @param symbol
         * @param limit default 500; max 1000.
         * @param fromId TradeId to fetch from. Default gets most recent trades.
         * @param startTime timestamp in ms to get aggregate trades from INCLUSIVE
         * @param endTime timestamp in ms to get aggregate trades until INCLUSIVE
         * @return 0 for success, or errno
         */
        int QueryTradesAggregate(const char *symbol, size_t limit, long fromId, timestamp_t startTime, timestamp_t endTime);
       
        /**
         * Kline/candlestick bars for a symbol
         * @param symbol
         * @param limit default 500; max 1000.
         * @oaram interval 
         * @param startTime timestamp in ms to get aggregate trades from INCLUSIVE
         * @param endTime timestamp in ms to get aggregate trades until INCLUSIVE
         * @return 0 for success, or errno
         */
        int QueryKline(const char *symbol, size_t limit, size_t interval, timestamp_t startTime, timestamp_t endTime);

        /**
         * 24 hour price change statistics
         * @param symbol
         * @return 0 for success, or errno
         */
        int QueryTicker(const char *symbol);

        /**
         * Latest price for a symbol or symbols
         * @param symbol
         * @return 0 for success, or errno
         */
        int QueryPrices(const char *symbol);

        /**
         * Best price/qty on the order book for a symbol or symbols
         * @param symbol
         * @return 0 for success, or errno
         */
        int QueryBookTicker(const char *symbol);

        //------------------------- Stream Data ------------------------------

        /**
         * Start a new user data stream
         * The stream will close after 60 minutes unless a keepalive is sent.
         */
        int StartUserDataStream();

        /**
         * Keepalive a user data stream to prevent a time out.
         * It's recommended to call this about every 30 minutes
         * @param listenKey the listenKey is returned by \ref StartUserDataStream
         * @return 0 for success, or errno
         */
        int KeepAliveUserDataStream(const char *listenKey);

        /**
         * Close out a user data stream.
         * @param listenKey the listenKey is returned by \ref StartUserDataStream
         * @return 0 for success, or errno
         */
        int CloseUserDataStream(const char *listenKey);

        //-------------------------- Future Data -----------------------------

        // NOTHING

        //-------------------------- Extra Data ------------------------------

        /**
         * Test connectivity to the Rest API
         * synchronized
         * @return 0 for sucessfully receiving server response, elsewise errno 
         */
        int PingServer();

        /**
         * Get the current server time.
         * \ref OnServerTime
         */
        int GetServerTime();

        /** 
         * Current exchange trading rules and symbol information
         * \ref OnExchangeInfo
         */
        int GetExchangeInfo();

    };

} // namespace exapi