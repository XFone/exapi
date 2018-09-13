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
#include "trade/DATraderBitmexDef.h"
#include "quote/DAQuoteBitmexDef.h"

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

        virtual void OnApiKey(const void *pRespData) {}

        //------------------ Chat callbacks ----------------------

        virtual void OnChatMessages(const void *msgs) {}

        virtual void OnChatChannels(const void *channels) {}

        virtual void OnConnectedUsers(const void *users) {}

        virtual void OnSentMessage(const void *chat) {}

        //------------------ Account callbacks ------------------

        /**
         * Callback to GetUserInfo
         * @param pUserInfo struct of exapi::UserInfo (TODO)
         */
        virtual void OnUserInfo(const void *pUserInfo) {}

        //...MORE

        virtual void OnNotification(const void *notifies) {}

        //--------------------- Trade callbacks ------------------

        virtual void OnOrderPlaced(const void *orders) {}

        virtual void OnOrderCancelled(const void *orders) {}

        virtual void OnOrderUpdated(const void *orders) {}

        virtual void OnExecutions(const void *executions) {}

        virtual void OnLiquidation(const void *liquidation) {}

        //------------------ Instrument callbacks ----------------

        virtual void OnInstruments(const void *instruments) {}

        virtual void OnPriceIndices(const void *instruments) {}

        virtual void OnActiveIntervals(const void *instruments) {}

        virtual void OnCompositeIndex(const void *instruments) {}

        //----------------- Trade history callbacks --------------

        virtual void OnQueryOrders(const void *orders) {}

        virtual void OnTradeHistory(const void *executions) {}

        //----------------- Future callbacks ---------------------

        virtual void OnQueryPositions(const void *positions) {}

        virtual void OnPositionIsolateMargin(const void *position) {}

        virtual void OnPositionTransferMargin(const void *position) {}

        virtual void OnPositionLeverage(const void *position) {}

        virtual void OnPositionRiskLimit(const void *position) {}

        virtual void OnQuerySettlement(const void *settlements) {}

        //----------------- Top user callbacks -------------------

        virtual void OnLeadboard(const void *leaderboards) {}

        virtual void OnLeadboardName(const char *name) {}
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
         * Create a new order.
         * \ref OnOrderPlaced
         * @param params fields for placing order
         */
        int PlaceOrder(const OrderParams &params);

        /**
         * Create multiple new orders for the same symbol.
         * This endpoint is much faster for getting many orders into the book at once. 
         * \ref OnOrderPlaced
         * @param params fields for placing order
         */
        int PlaceOrdersBulk(const OrderParams *orders[]);

        /**
         * Cancel order(s). Send multiple order IDs to cancel in bulk.
         * \ref OnOrderCancelled
         */
        int CancelOrder(const char *orders[]);

        /**
         * Cancels all of your orders.
         * \ref OnOrderCancelled
         */
        int CancelOrdersAll(const char *symbol, const char *filter, const char *text);

        /**
         * Automatically cancel all your orders after a specified timeout
         * \ref OnOrderCancelled
         * @param timeout Timeout in ms. Set to 0 to cancel this timer.
         */
        int CancelOrdersAfter(timestamp_t timeout);

        /**
         * Amend the quantity or price of an open order
         * Send an orderID or origClOrdID to identify the order you wish to amend.
         * Both order quantity and price can be amended. Only one qty field 
         * can be used to amend.
         * \ref OnOrderUpdated
         */
        int UpdateOrder(const OrderAmendParams &params);
        
        /**
         * Amend multiple orders for the same symbol.
         * \ref OnOrdersUpdated
         */
        int UpdateOrders(const char *orders[]);

        /**
         * Get liquidation orders.
         * \ref OnLiquidation
         * @param qfilter query filter and parameters
         */
        int QueryLiquidationOrders(const QueryFilterParams &qfilter);

        /**
         * Get all raw executions for user account.
         * \ref OnExecutions
         * @param qfilter query filter and parameters
         */
        int QueryExecutions(const QueryFilterParams &qfilter);

        //-------------------------  Instruments ------------------------------
 
        /**
         * Get instruments.
         * This returns all instruments and indices, including those that have 
         * settled or are unlisted.
         * \ref OnInstruments
         */
        int QueryInstruments(const QueryFilterParams &qfilter);

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

        /**
         * Get your orders.
         * \ref OnQueryOrders
         */
        int QueryOrders(const QueryFilterParams &qfilter);

        /**
         * Get all balance-affecting executions.
         * This includes each trade, insurance charge, and settlement. 
         * \ref OnTradeHistory
         * @param qfilter filter and query parameters
         */
        int QueryTradeHistory(const QueryFilterParams &qfilter);

        //------------------------- Future Trade -----------------------------

        /**
         * Get your positions.
         * \ref OnQueryPositions
         */
        int QueryPosition(const QueryPositionParams &params);

        /**
         * Enable isolated margin or cross margin per-position.
         * \ref OnPositionIsolateMargin
         */
        int SetPositionIsolateMargin(const char *symbol, bool enable = true);

        /**
         * Transfer equity in or out of a position
         * \ref OnPositionTransferMargin
         * @param symbol
         * @param amount Amount to transfer, in Satoshis. May be negative. 
         */
        int SetPositionTransferMargin(const char *symbol, amount_t amount);

        /**
         * Choose leverage for a position.
         * \ref OnPositionLeverage
         * @param symbol
         * @param leverage Leverage value. Send a number between 0.01 and 100 to 
         *        enable isolated margin with a fixed leverage. Send 0 to enable
         *        cross margin.
         */
        int SetPositionLeverage(const char *symbol, double leverage);

        /**
         * Update your risk limit.
         * \ref OnPositionRiskLimit
         * @param symbol Symbol of position to update risk limit on.
         * @param riskLimit New Risk Limit, in Satoshis.
         */
        int SetPositionRiskLimit(const char *symbol, amount_t riskLimit);

        /**
         * Close a position.
         * [Deprecated, use POST /order with execInst: 'Close']
         */
        int ClosePosition(const char *symbol, d_price_t price);

        /**
         * Get settlement history.
         * \ref OnQuerySettlement
         */
        int QuerySettlement(const QueryFilterParams &params);

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

        /**
         * Get your current notifications.
         * This is an upcoming feature and currently does not return data.
         * \ref OnNotification
         */
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

        /**
         * Get current leaderboard
         * \ref OnLeadboard
         * @param method ranking type, options: "notional", "ROE"
         */
        int GetLeadboard(const char *method);

        /**
         * Get your alias on the leaderboard.
         * \ref OnLeadboardName
         */
        int GetLeadboardName();

    };

} // namespace exapi