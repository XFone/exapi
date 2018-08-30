/*
 * $Id: $
 * 
 * DAQuoteOkexApi and DAQuoteOkexSpi class declaration
 * 
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file DAQuoteOkexApi.h Okcoin and Okex quotation client api.
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
     * DAQuoteOkexSpi
     * Okex quotation interface for async callback
     */
    class DAQuoteOkexSpi : public IQuoteSpi {
    public:
        virtual ~DAQuoteOkexSpi() {}

        /**
         * @param addr server address
         */
        virtual void OnConnected(const char *addr) {}

        /**
         * @param addr server address
         */
        virtual void OnDisconnected(const char *addr) {}

        /**
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
     * DAQuoteOkexApi
     * Okex quotation service
     */
    class API_EXPORT DAQuoteOkexApi : public IQuoteApi {
    public:
        virtual ~DAQuoteOkexApi() {}

        /**
         * Create TraderApi
         * @param dpath the local directory to save quotation data, default is work path
         * @return the DAQuoteOkexApi instance
         */
        static DAQuoteOkexApi *CreateApi(const char *dpath = "");
        
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

        /**
         * 获取美元人民币汇率
         */
        int GetExchangeRate(void);

        /**
         * 获取交割预估价
         */
        int GetFutureEstimatedPrice(const char *symbol);

    };

} // namespace exapi