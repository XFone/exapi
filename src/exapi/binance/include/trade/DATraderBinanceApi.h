/*
 * $Id: $
 * 
 * DATraderBinanceApi and DATraderBinanceSpi class declaration
 * 
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file DATraderBinanceApi.h Binance trader client api.
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
#include "trade/ITraderApi.h"

namespace exapi {

    /**
     * DATraderBinanceSpi
     * Binance trader interface for async callback
     */
    class DATraderBinanceSpi : public ITraderSpi {
    public:
        virtual ~DATraderBinanceSpi() {}

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
         * Response to UserLogon
         * @param pLogon userContext
         */
        virtual void OnUserLogon(void *pLogon) {}

        /**
         * Response is received for a pending command
         * @param apiType api command type
         * @param pRespData response data
         * @return process status (0 for success)
         */
        virtual int OnResponse(TraderApiType apiType, void *pRespData);
 
        //------------------ spot callbacks ----------------------

        /**
         * Callback to GetUserInfo
         * @param pUserInfo struct of exapi::UserInfo (TODO)
         */
        void OnUserInfo(void *pUserInfo) {}

        //...MORE

        //----------------- future callbacks ---------------------

    };

    /**
     * DATraderBinanceApi
     * Binance trade service
     */
    class API_EXPORT DATraderBinanceApi : public ITraderApi {
    public:
        virtual ~DATraderBinanceApi() {}

        /**
         * Create TraderApi
         * @param api_key user should applied an api-key from binance.com
         * @param secret_key key for signing parameter
         * @param dpath the local directory to save quotation data, default is work path
         * @return the DATraderOkexApi instance
         */
        static DATraderBinanceApi *CreateApi(const char *api_key, const char *secret_key, const char *dpath = "");
        
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

        //-------------------------  Spot Trade -------------------------------

        /**
         * Send order
         */
        int EnterOrder(const char *symbol, const char *type, const char *side,
                       const char *timeInForce, 
                       d_price_t price, quantity_t quantity,
                       const char *newClientOrderId, 
                       d_price_t stopPrice, quantity_t icebergQty,
                       size_t recvWindow);

        /**
         */
        int TestOrder();

        /**
         */
        int CancelOrder(const char *symbol, const char *orderId, 
                       const char *origClientOrderId, const char *newClientOrderId,
                       size_t recvWindow);

        /**
         */
        int QueryOrder(const char *symbol, const char *orderId,
                       const char *origClientOrderId,
                       size_t recvWindow);


        //--------------------  Spot Trade History ---------------------------

        /**
         */
        int QueryOpenOrders(const char *symbol, size_t recvWindow);

        /**
         */
        int QueryAllOrders(const char *symbol, const char *orderId,
                           size_t limit, size_t recvWindow);

        //------------------------- Future Trade -----------------------------

        // NOTHING

        //------------------------ Account Endpoints -------------------------

        /**
         */
        int QueryUserAccount(size_t recvWindow);

        /**
         */
        int QueryAccountTrades(const char *symbol, size_t limit, 
                               const char *fromId, size_t recvWindow);
        

        //------------------------- Wallet Access ----------------------------

        int DoAccountWithdraw(const char *asset, const char *address,
                              const char *addressTag, amount_t amount,
                              const char *name, size_t recvWindow);

        int QueryAccountDeposit(const char *asset, int status,
                                timestamp_t startTime, timestamp_t endTime,
                                size_t recvWindow);

        int QueryAccountWithdraw(const char *asset, int status,
                                 timestamp_t startTime, timestamp_t endTime,
                                 size_t recvWindow);

        int GetDepositAddress(const char *asset, size_t recvWindow);

        int QueryAccountStatus();
        
        int QuerySystemStatus();

        int GetAccountAssetDribbletLog();

        int QueryAccountTradeFee();

        int QueryAssetDetail();

    };

} // namespace exapi