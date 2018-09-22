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

    /* C header without namespace */
    #include "quote/DAQuoteBitmexDef.h"
    #include "trade/DATraderBitmexDef.h"

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

        /**
         * @param msgs array of \ref Chat messages
         */
        virtual void OnChatMessages(const void *msgs) {}

        /**
         * @param channels array of \ref ChatChannel
         */
        virtual void OnChatChannels(const void *channels) {}

        /**
         * @param users \ref ConnectedUsers
         */
        virtual void OnConnectedUsers(const void *users) {}

        /**
         * @param chat \ref Chat
         */ 
        virtual void OnSentMessage(const void *chat) {}

        //------------------ Account callbacks ------------------

        /**
         * Callback to GetUserInfo
         * @param pUserInfo struct of exapi::UserInfo (TODO)
         */
        virtual void OnUserInfo(const void *pUserInfo) {}

        /**
         * @param nothing
         */
        virtual void OnLogout(const void *nothing) {}

        /**
         * @param value magic string, may TTL
         */
        virtual void OnLogoutAll(const char *value) {}
        
        /**
         * @param accessToken \ref AccessToken
         */
        virtual void OnEmailConfirmed(const void *accessToken) {}

        /**
         * @param token 
         * @param enabled
         */
        virtual void OnTFAComfirmed(const char *token, bool enabled) {}

        /**
         * @param enabled
         */
        virtual void OnTFAEnabled(bool enabled) {}

        /**
         * @param token 
         * @param disabled
         */
        virtual void OnTFADisabled(const char *token, bool disabled) {}

        /**
         * @param user \ref User
         */
        virtual void OnQueryUser(const void *user) {}

        /**
         * @param user \ref User
         */
        virtual void OnUserUpdated(const void *user) {}

        /**
         * @param affiliate \ref Affiliate
         */
        virtual void OnUserAffiliateStatus(const void *affiliate) {}

        /**
         * @param commissions array of \ref UserCommission
         */
        virtual void OnUserCommissionStatus(const void *commissions) {}

        /**
         * @param margin \ref Margin
         */
        virtual void OnUserMarginStatus(const void *margin) {}

        /**
         * @param user \ref User
         */
        virtual void OnUserPreferences(const void *user) {}

        /**
         * @param notifies array of \ref Notification
         */
        virtual void OnNotification(const void *notifies) {}

        //--------------------- Trade callbacks ------------------

        /**
         * @param order \ref Order
         */ 
        virtual void OnOrderPlaced(const void *order) {}

        /**
         * @param orders array of \ref Order
         */ 
        virtual void OnOrdersPlaced(const void *orders) {}

        /**
         * @param orders array of \ref Order
         */ 
        virtual void OnOrdersCancelled(const void *orders) {}

        /**
         * @param order \ref Order
         */ 
        virtual void OnOrderUpdated(const void *order) {}

        /**
         * @param orders array of \ref Order
         */ 
        virtual void OnOrdersUpdated(const void *orders) {}

        /**
         * @param executions array of \ref Execution
         */ 
        virtual void OnExecutions(const void *executions) {}

        /**
         * @param liquidation \ref Liquidation
         */
        virtual void OnLiquidation(const void *liquidation) {}

        //------------------ Instrument callbacks ----------------

        /**
         * @param instruments array of \ref Instrument
         */
        virtual void OnInstruments(const void *instruments) {}

        /**
         * @param instruments array of \ref Instrument
         */
        virtual void OnPriceIndices(const void *instruments) {}

        /**
         * @param instruments array of \ref Instrument
         */
        virtual void OnActiveIntervals(const void *instruments) {}

        /**
         * @param instruments array of \ref Instrument
         */
        virtual void OnCompositeIndex(const void *instruments) {}

        //----------------- Trade history callbacks --------------

        /**
         * @param orders array of \ref Order
         */
        virtual void OnQueryOrders(const void *orders) {}

        /**
         * @param executions array of \ref Execution
         */
        virtual void OnTradeHistory(const void *executions) {}

        //----------------- Future callbacks ---------------------

        /**
         * @param positions array of \ref Position
         */
        virtual void OnQueryPositions(const void *positions) {}

        /**
         * @param position \ref Position
         */
        virtual void OnPositionIsolateMargin(const void *position) {}

        /**
         * @param position \ref Position
         */
        virtual void OnPositionTransferMargin(const void *position) {}

        /**
         * @param position \ref Position
         */
        virtual void OnPositionLeverage(const void *position) {}

        /**
         * @param position \ref Position
         */
        virtual void OnPositionRiskLimit(const void *position) {}

        /**
         * @param positions array of \ref Settlement
         */
        virtual void OnQuerySettlement(const void *settlements) {}

        //-------------- Account wallet callbacks ----------------

        /**
         * @param transactions array of \ref Transactions
         */
        virtual void OnResponseWithdraw(const void *transaction) {}

        /**
         * @param transactions array of \ref Transactions
         */
        virtual void OnWithdrawCancelled(const void *transaction) {}

        /**
         * @param transactions array of \ref Transactions
         */
        virtual void OnWithdrawConfirmed(const void *transaction) {}

        /**
         * @param address deposit address
         */
        virtual void OnDepositAddress(const char *address) {}

        /**
         * @param wallet \ref Wallet
         */
        virtual void OnQueryWallet(const void *wallet) {}

        /**
         * @param transactions array of \ref Transactions
         */
        virtual void OnWalletHistory(const void *transactions) {}

        /**
         * @param transactions array of \ref Transactions
         */
        virtual void OnWalletSummary(const void *transactions) {}

        /**
         * @param object
         */
        virtual void OnMinWithdrawalFee(const void *object) {}

        //----------------- Top user callbacks -------------------

        /**
         * @param leaderboard array of \ref Leaderboard
         */
        virtual void OnLeadboard(const void *leaderboards) {}

        /**
         * @param nams leaderboard alias
         */
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
         * \ref OnOrdersPlaced
         * @param orders fields for placing order(s)
         */
        int PlaceOrdersBulk(const OrderParams *orders[]);

        /**
         * Cancel order(s). Send multiple order IDs to cancel in bulk.
         * \ref OnOrdersCancelled
         * @param orderID Order ID(s)
         * @param clOrdID Client Order ID(s)
         * @param text Optional cancellation annotation. e.g. 'Spread Exceeded'.
         */
        int CancelOrder(const char *orderID[], const char *clOrdID[] = nullptr, 
                        const char *text = nullptr);

        /**
         * Cancels all of your orders.
         * \ref OnOrdersCancelled
         * @param symbol If provided, only cancels orders for that symbol.
         * @param filter filter for cancellation. Use to only cancel some orders, e.g.  {"side": "Buy"}. 
         * @param text Optional cancellation annotation. e.g. 'Spread Exceeded'.
         */
        int CancelOrdersAll(const char *symbol, json_t filter, 
                            const char *text = nullptr);

        /**
         * Automatically cancel all your orders after a specified timeout
         * \ref OnOrdersCancelled
         * @param timeout Timeout in ms. Set to 0 to cancel this timer.
         */
        int CancelOrdersAfter(timestamp_t timeout);

        /**
         * Amend the quantity or price of an open order
         * Send an orderID or origClOrdID to identify the order you wish to amend.
         * Both order quantity and price can be amended. Only one qty field 
         * can be used to amend.
         * \ref OnOrderUpdated
         * @param params Send an orderID or origClOrdID to identify the order 
         *        you wish to amend.
         */
        int UpdateOrder(const OrderAmendParams &params);
        
        /**
         * Amend multiple orders for the same symbol.
         * \ref OnOrdersUpdated
         * @param orders An array of orders.
         */
        int UpdateOrders(const OrderAmendParams *orders[]);

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
        int QueryPosition(json_t filter, const char *columns[], size_t count);

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

        /**
         * Log out of BitMEX.
         * \ref OnLogout
         */
        int Logout();

        /**
         * Log all systems out of BitMEX. This will revoke all of your 
         * account's access tokens, logging you out on all devices.
         * \ref OnLogoutAll
         */
        int LogoutAll();

        /**
         * Confirm your email address with a token. 
         * \ref OnEmailConfirmed
         * @param token 
         */
        int ConfirmEmail(const char *token);

        /**
         * Confirm two-factor auth for this account.
         * If using a Yubikey, simply send a token to this endpoint.
         * \ref OnTFAComfirmed
         * @param ttype Two-factor auth type. Supported types: 'GA', 'Yubikey'
         * @param token Token from your selected TFA type.
         */
        int ConfirmEnableTFA(const char *ttype, const char *token);

        /**
         * Get secret key for setting up two-factor auth.
         * Use /confirmEnableTFA directly for Yubikeys. This fails if TFA is already enabled.
         * \ref OnTFAEnabled
         * @param ttype Two-factor auth type. Supported types: 'GA'
         */
        int EnableTFA(const char *ttype = "GA");

        /**
         * Disable two-factor auth for this account. 
         * \ref OnTFADisabled
         * @param ttype Two-factor auth type. Supported types: 'GA', 'Yubikey'
         * @param token Token from your selected TFA type.
         */
        int DisableTFA(const char *ttype, const char *token);
        
        /**
         * Get your user model. 
         * \ref OnQueryUser
         */
        int QueryUser();

        /**
         * Update your password, name, and other attributes. 
         * \ref OnUserUpdated
         * @param firstname
         * @param lastname
         * @param oldPassword
         * @param newPassword
         * @param newPasswordConfirm
         * @param username Username can only be set once. To reset, email support.
         * @param country Country of residence.
         * @param pgpPubKey PGP Public Key. If specified, automated emails will be sentwith this key.
         */
        int UpdateUser(const char *firstname = nullptr, const char *lastname = nullptr,
                       const char *oldPassword = nullptr, const char *newPassword = nullptr,
                       const char *newPasswordConfirm = nullptr, const char *username = nullptr,
                       const char *country = nullptr, const char *pgpPubKey = nullptr);

        /**
         * Get your current affiliate/referral status.
         * \ref OnUserAffiliateStatus
         */
        int QueryUserAffiliateStatus();

        /**
         * Get your account's commission status.
         * \ref OnUserCommissionStatus
         */
        int QueryUserCommission();

        /**
         * Get your account's margin status. 
         * Send a currency of "all" to receive an array of all supported currencies.
         * \ref OnUserMarginStatus
         * @param currency default: XBt
         */
        int QueryUserMargin(const char *currency = "XBt");

        /**
         * Save user preferences.
         * \ref OnUserPreferences
         * @param prefs 
         * @param overwrite If true, will overwrite all existing preferences
         */
        int SaveUserPreferences(const json_t prefs, bool overwrite = false);

        /**
         * Get your current notifications.
         * This is an upcoming feature and currently does not return data.
         * \ref OnNotification
         */
        int GetNotification();

        //----------------- Account: Wallet Access ---------------------------

        /**
         * Request a withdrawal to an external wallet. 
         * This will send a confirmation email to the email address on record, 
         * unless requested via an **API Key** with the withdraw permission.
         * \ref OnResponseWithdraw
         * @param otpToken 2FA token. Required if 2FA is enabled on your account.
         * @param currency Currency you're withdrawing. default "XBt"
         * @param amount Amount of withdrawal currency
         * @param dest_addr Destination Address.
         * @param fee Network fee for Bitcoin withdrawals. If not specified, a 
         *        default value will be calculated based on Bitcoin network 
         *        conditions. You will have a chance to confirm this via email.
         */
        int RequestWithdraw(const char *otpToken, const char *currency, 
                            amount_t amount, const char *dest_addr, price_t fee = 0);

        /**
         * Cancel a withdrawal.
         * \ref OnWithdrawCancelled
         * @param token 2FA token
         */
        int CancelWithdraw(const char *token);

        /**
         * Confirm a withdrawal.
         * \ref OnWithdrawConfirmed
         * @param token 2FA token
         */
        int ConfirmWithdraw(const char *token);

        /**
         * Get a deposit address.
         * \ref OnDepositAddress
         * @param currency default: "XBt"
         */
        int QueryDepositAddress(const char *currency = "XBt");

        /**
         * Get your current wallet information.
         * \ref OnQueryWallet
         * @param currency default: "XBt"
         */
        int QueryWallet(const char *currency = "XBt");

        /**
         * Get a history of all of your wallet transactions (deposits, withdrawals, PNL)
         * \ref OnWalletHistory
         * @param curreny
         */
        int QueryWalletHistory(const char *currency = "XBt");

        /**
         * Get a summary of all of your wallet transactions (deposits, withdrawals, PNL).
         * \ref OnWalletSummary
         * @param currency
         */
        int QueryWalletSummary(const char *currency = "XBt");

        /**
         * Get the minimum withdrawal fee for a currency. 
         * \ref OnMinWithdrawalFee
         * This is changed based on network conditions to ensure timely withdrawals. 
         * During network congestion, this may be high. The fee is returned in 
         * the same currency.
         * @param currency default: "XBt"
         */
        int QueryMinWithdrawalFee(const char *currency = "XBt");

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