/*
 * $Id: $
 * 
 * DATraderBitmexApi and DATraderBitmexSpi class declaration
 * 
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file DATraderBitmexApi.h Bitmex trader client api.
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
     * DATraderBitmexSpi
     * Bitmex trader interface for async callback
     */
    class DATraderBitmexSpi : public ITraderSpi {
    public:
        virtual ~DATraderBitmexSpi() {}

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
     * DATraderBitmexApi
     * Bitmex trade service
     */
    class API_EXPORT DATraderBitmexApi : public ITraderApi {
    public:
        virtual ~DATraderBitmexApi() {}

        /**
         * Create TraderApi
         * @param api_key user should applied an api-key from bitmex.com
         * @param secret_key key for signing parameter
         * @param dpath the local directory to save quotation data, default is work path
         * @return the DATraderOkexApi instance
         */
        static DATraderBitmexApi *CreateApi(const char *api_key, const char *secret_key, const char *dpath = "");
        
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

        //----------- APIKey - Persistent API Keys for traders ---------------
    
        int SetApiKey(bool enable);

        int GetApiKey();

        int CreateApiKey();

        int RemoveApiKey();

        //------------------------------- Chat --------------------------------

        int GetMessage();
        int GetChannels();
        int GetConnectedUsers();
        int SendMessage();

        //----------------------------- Trade --------------------------------

        /**
         * Send order
         */
        int EnterOrder();

        int EnterOrderBulk();

        /**
         */
        int CancelOrder();

        int CancelOrdersAll();
        int CancelOrdersAfter();


        int UpdateOrder();
        int UpdateOrders();

        int QueryExecutions();
        int QueryLiquidationOrders();


        //------------------------  Trade History -----------------------------

        /**
         */
        int QueryOrder();

        int QueryOrders();

        int QueryTradeHistory();

        //------------------------- Future Trade -----------------------------

        int QueryPosition();

        int SetPositionIsolateMargin();

        int SetPositionTransferMargin();

        int SetPositionLeverage();

        int SetPositionRiskLimit();

        int ClosePosition();

        int QuerySettlement();

        //-------------------- User Account Operations ----------------------

        int Logout();
        int LogoutAll();

        int ConfirmEmail();
        int ConfirmEnableTFA();
        int ConfirmDisableTFA();

        int EnableTFA();
        
        int QueryUser();
        int UpdateUser();

        int QueryUserAffiliateStatus();
        int QueryUserCommission();
        int QueryUserMargin();

        int QueryUserPreferences();

        int GetNotification();

        //----------------- Account: Wallet Access ---------------------------

        int RequestWithdraw();
        int CancelWithdraw();
        int ConfirmWithdraw();
        int QueryDepositAddress();
        int QueryWallet();
        int QueryWalletHistory();
        int QueryWalletSummary();

        int QueryMinWithdrawalFee();

        //--------------------- Account: Top Users ---------------------------

        int GetLeadboard();

        int GetLeadboardName();

    };

} // namespace exapi