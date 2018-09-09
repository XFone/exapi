/*
 * $Id: $
 * 
 * DATraderOkexApi and DATraderOkexSpi class declaration
 * 
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file DATraderOkexApi.h Okcoin and Okex trader client api.
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
     * DATraderOkexSpi
     * Okex trader interface for async callback
     */
    class DATraderOkexSpi : public ITraderSpi {
    public:
        virtual ~DATraderOkexSpi() {}

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
     * DATraderOkexApi
     * Okex trade service
     */
    class API_EXPORT DATraderOkexApi : public ITraderApi {
    public:
        virtual ~DATraderOkexApi() {}

        /**
         * Create TraderApi
         * @param api_key user should applied an api-key from okcoin.com
         * @param secret_key key for signing parameter
         * @param dpath the local directory to save quotation data, default is work path
         * @return the DATraderOkexApi instance
         */
        static DATraderOkexApi *CreateApi(const char *api_key, const char *secret_key, const char *dpath = "");
        
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
         * 获取用户信息
         */
        int GetUserInfo();

        /**
         * 下单交易
         */
        int DoTrade(const char *symbol, const char *type, d_price_t price, d_amount_t amount);

        /**
         * 批量下单
         */
        int DoBatchTrade(const char *symbol, const char *type, const char *ordersdata);

        /**
         * 撤销订单
         */
        int DoCancelOrder(const char *symbol, const char *orderid);

        /**
         * 获取用户的订单信息
         */
        int GetOrderInfo(const char *symbol, const char *orderid);

        /**
         * 批量获取用户订单
         */
        int GetOrdersInfo(const char *type, const char *symbol, const char *orderid);	

        /**
         * 提币 BTC/LTC
         */
        int DoWithdraw(const char *symbol, d_price_t chargefee, const char *tradepwd,
                       const char *withdrawAddress, d_amount_t withdrawAmount);

        /**
         * 取消提币BTC/LTC
         */
        int DoCancelWithdraw(const char *symbol, const char *withdrawid);

        /**
         * 查询手续费
         */
        int GetOrderFee(const char *symbol,const char *orderid);

        /**
         * 获取放款深度前10
         */
        int GetLendDepth(const char *symbol);

        /**
         * 查询用户借款信息
         */
        int GetBorrowsInfo(const char *symbol);

        /**
         * 申请借款
         */
        int DoBorrowMoney(const char *symbol, const char *days, d_amount_t amount, d_price_t rate);

        /**
         * 取消借款申请
         */
        int DoCancelBorrow(const char *symbol, const char *borrowid);

        /**
         * 获取借款订单记录
         */
        int GetBorrowOrderinfo(const char *borrowid);

        /**
         * 用户还全款
         */
        int DoRepayment(const char *borrowid);

        /**
         * 未还款列表
         */
        int GetUnrepaymentsInfo(const char *symbol, 
                                size_t currentpage, size_t pagelength);
        
        /**
         * 获取用户提现/充值记录
         */
        int GetAccountRecords(const char *symbol, const char *type, 
                              size_t currentpage, size_t pagelength);

        //--------------------  Spot Trade History ---------------------------

        /**
         * 获取OKCoin历史交易信息
         */
        int GetTradeHistory(const char *symbol, const char *range);

        /**
         * 获取历史订单信息，只返回最近七天的信息
         */
        int GetOrderHistory(const char *symbol, const char *status,
                            size_t currentpage, size_t pagelength);

        //------------------------- Future Trade -----------------------------

        /**
         * 获取OKCoin期货账户信息（全仓）
         */
        int GetFutureUserinfo();

        /**
         * 获取用户持仓获取OKCoin期货账户信息（全仓）
         */
        int GetFuturePosition(const char *symbol, const char *contracttype);	

        /**
         * 期货下单
         */
        int DoFutureTrade(const char *symbol, const char *contracttype, 
                          d_price_t price, d_amount_t amount,
                          const char *type, d_price_t matchprice, d_price_t leverrate);

        /**
         * 批量下单
         */
        int DoFutureBatchtrade(const char *symbol, const char *contracttype,
                               const char *ordersdata, d_price_t leverrate);

        /**
         * 取消期货订单
         */
        int DoFutureCancel(const char *symbol, const char *orderid, const char *contracttype);

        /**
         * 获取期货订单信息
         */
        int GetFutureOrderinfo(const char *symbol, const char *contracttype, const char *status,
                               const char *orderid, 
                               size_t currentpage, size_t pagelength);

        /**
         * 批量获取期货订单信息
         */
        int GetFutureOrdersinfo(const char *symbol, const char *contracttype, const char *orderid);

        /**
         * 获取逐仓期货账户信息
         */
        int GetFutureUserinfo4fix();

        /**
         * 逐仓用户持仓查询
         */
        int GetFuturePosition4fix(const char *symbol, const char *contracttype, const char *type);

        /**
         * 获取期货爆仓单
         */
        int GetFutureExplosive(const char *symbol, const char *contracttype, const char *status,
                               size_t currentpage, size_t pagelength);

        //-------------------- Future Trade History --------------------------

        /**
         * 获取OKCoin期货交易历史
         */
        int GetFutureTradeHistory(const char *symbol, const char *date, const char *range);

    };

} // namespace exapi
