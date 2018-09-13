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
 
        //--------------------- Callbacks ------------------------

        void OnApiKey(const void *pRespData) {}

        //------------------ Chat callbacks ----------------------

        void OnChatMessages(const void *msgs) {}

        void OnChatChannels(const void *channels) {}

        void OnConnectedUsers(const void *users) {}

        void OnSentMessage(const void *chat) {}

        //------------------ spot callbacks ----------------------

        /**
         * Callback to GetUserInfo
         * @param pUserInfo struct of exapi::UserInfo (TODO)
         */
        void OnUserInfo(const void *pUserInfo) {}

        //...MORE

        //--------------------- Trade callbacks ------------------

        void OnExecutions(const void *executions) {}

        //------------------ Instrument callbacks ----------------
        
        void OnInstruments(const void *instruments) {}

        void OnPriceIndices(const void *instruments) {}

        void OnActiveIntervals(const void *instruments) {}

        void OnCompositeIndex(const void *instruments) {}

        //----------------- Trade history callbacks --------------

        void OnTradeHistory(const void *executions) {}

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
    
        /**
         * Enable or disable an API Key
         * \ref OnApiKeyEnabled, \ref OnApiKeyDisabled
         * @param true for enable, elsewise disable
         */
        int SetApiKey(bool enable);

        /**
         * Get user's API Keys. 
         * \ref OnApiKey
         * @param reverse true will sort results newest first.
         */
        int GetApiKey(bool reverse = false);

        /**
         * Create a new API Key. 
         * \ref OnApiKey
         * @param name Key name. This name is for reference only.
         * @param cidr Classless Inter-Domain Routing, e.g., "10.0.0/24"
         * @param permissions available: ["order", "orderCancel", "withdraw"]
         * @param enabled set to true to enable this key on creation
         * @param token OTP Token (YubiKey, Google Authenticator)
         */
        int CreateApiKey(const char *name, const char *cidr, 
                         const char *permissions, bool enabled, 
                         const char *token);

         /**
         * Remove an API Key. 
         * \ref OnApiKey
         * @param apiKeyID key to remove, null to remove current used apikey
         */
        int RemoveApiKey(const char *apiKeyID = nullptr);

        //------------------------------- Chat --------------------------------

        /**
         * Get chat messages.
         * \ref OnChatMessages
         * @param count number of results to fetch. default: 100
         * @param start starting ID for results. default: 0
         * @param reverse If true, will sort results newest first. default: true
         * @param channelID Channel id. Leave blank for all. 
         */
        int GetMessage(size_t count = 100, size_t start = 0, bool reverse = true, double channelID = 0.0);
        
        /**
         * Get available channels.
         * \ref OnChatChannels
         */
        int GetChannels();

        /**
         * Get connected users.
         * \ref OnConnectedUsers
         */
        int GetConnectedUsers();

        /**
         * Send a chat message.
         * \ref OnSentMessage
         * @param message
         * @param channelID channel to post to, default 1 (English)
         */
        int SendMessage(const char *message, double channelID = 1);

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

        int QueryLiquidationOrders();

        /**
         * Get all raw executions for user account.
         * \ref OnExecutions
         * @param symbol e.g. bare series (e.g. "XBU"), or with timestamp ("XBU:monthly")
         */
        int QueryExecutions(const char *symbol, const char *filter, 
                            const char *columns[], size_t count = 100, 
                            size_t start = 0, bool reverse = false, 
                            time_t startTime = 0,
                            time_t endTime = 0);

        //-------------------------  Instruments ------------------------------
 
        /**
         * Get instruments.
         * This returns all instruments and indices, including those that have 
         * settled or are unlisted.
         * \ref OnInstruments
         */
        int QueryInstruments(const char *symbol, const char *filter, 
                             const char *columns[], size_t count = 100, 
                             size_t start = 0, bool reverse = false, 
                             time_t startTime = 0,
                             time_t endTime = 0);

        /**
         * Get all active instruments and instruments that have expired in <24hrs.
         * \ref OnInstruments
         */
        int QueryInstrumentsActive();

        /**
         * Gets all active instruments and all indices.
         * This is a join of the result of /indices and /active
         * \ref OnInstruments
         */
        int QueryInstrumentActiveAndIndices();

        /**
         * Get all active contract series and interval pairs.
         * \ref OnActiveIntervals
         */
        int QueryInstrumentActiveIntervals();

        /**
         * Show constituent parts of an index. 
         * \ref OnCompositeIndex
         */
        int QueryCompositeIndex();

        /**
         * Get all price indices
         * \ref OnPriceIndices
         */
        int QueryPriceIndices();

        //------------------------  Trade History -----------------------------

        int QueryOrder();

        int QueryOrders();

        /**
         * Get all balance-affecting executions.
         * This includes each trade, insurance charge, and settlement. 
         * \ref OnTradeHistory
         * @param symbol e.g. bare series (e.g. "XBU"), or with timestamp ("XBU:monthly")
         */
        int QueryTradeHistory(const char *symbol, const char *filter, 
                              const char *columns[], size_t count = 100, 
                              size_t start = 0, bool reverse = false, 
                              time_t startTime = 0,
                              time_t endTime = 0);

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