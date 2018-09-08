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
        virtual void OnQuoteUpdated(int quoteType, void *pQuoteData) {}
    
        /**
         * @param quoteType quote type
         * @param psymbol quote symbol
         * @param status symbol subscription status
         */
        virtual void OnSymbolSubscribed(int quoteType, const char *pSymbol, unsigned status) {}

        /**
         * @param quoteType quote type
         * @param psymbol quote symbol
         * @param status symbol subscription status
         */
        virtual void OnSymbolUnsubscribed(int quoteType, const char *pSymbol, unsigned status) {}
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

        //----------------------  Spot Data ----------------------------------

        /**
         * 获取OKCoin行情
         */
        int GetTicker(const char *symbol);

        /**
         * 获取OKCoin市场深度
         */
        int GetDepth(const char *symbol, const char *range, char *merge);

        /**
         * 获取OKCoin最近600交易信息
         */
        int GetTrades(const char *symbol, const char *since);

        /**
         * 获取K线数据
         */
        int GetKline(int quoteType, const char *symbol, const char *range, size_t max_size);

        //----------------------- Future Data --------------------------------

        /**
         * 获取OKCoin期货行情
         */
        int GetFutureTicker(const char *symbol, const char *contract_type);

        /**
         * 获取OKCoin期货深度信息
         */
        int GetFutureDepth(const char *symbol, const char *contract_type, const char *range, char *merge);
        
        /**
         * 获取OKCoin期货交易记录信息
         */
        int GetFutureTrades(const char *symbol, const char *contract_type);

        /**
         * 获取OKCoin期货指数信息
         */
        int GetFutureIndex(const char *symbol);	

        /**
         * 获取期货合约的K线数据
         */
        int GetFutureKline(int quoteType, const char *symbol, const char *contract_type,
                           const char *range, size_t max_size);

        //--------------------------  Extra Data -----------------------------

		// Public API
		static void get_serverTime( Json::Value &json_result); 	

		static void get_allPrices( Json::Value &json_result );
		static double get_price( const char *symbol );

		static void get_allBookTickers( Json::Value &json_result );
		static void get_bookTicker( const char *symbol, Json::Value &json_result ) ;

		static void get_depth( const char *symbol, int limit, Json::Value &json_result );
		static void get_aggTrades( const char *symbol, int fromId, time_t startTime, time_t endTime, int limit, Json::Value &json_result ); 
		static void get_24hr( const char *symbol, Json::Value &json_result ); 
		static void get_klines( const char *symbol, const char *interval, int limit, time_t startTime, time_t endTime,  Json::Value &json_result );


		// API key required
		static void start_userDataStream( Json::Value &json_result );
		static void keep_userDataStream( const char *listenKey  );
		static void close_userDataStream( const char *listenKey );


        /**
         * 获取当前可用合约总持仓量
         */
        int GetFutureHoldAmount(const char *symbol, const char *contract_type);

    };

} // namespace exapi