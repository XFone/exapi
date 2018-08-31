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
# ifdef SHARELIBAPIEXPORT
#  define APIEXPORT declspec(dllexport)
# else
#  define APIEXPORT declspec(dllimport)
# endif
#else
# define  APIEXPORT 
#endif

#include "trade/ITraderApi.h"

namespace exapi {

    /**
     * DATraderOkexSpi
     * Okex trader interface for async callback
     */
    class DATraderOkexSpi {
    public:
        virtual ~DATraderOkexSpi() {}

    //现货交易 API
    //用于OKCoin快速进行现货交易

    };

    /**
     * DATraderOkexApi
     * Okex trade service
     */
    class APIEXPORT DATraderOkexApi {
    public:
        virtual ~DATraderOkexApi() {}

        /**
         * Create TraderApi
         * @param dpath the local directory to save quotation data, default is work path
         * @return the DATraderOkexApi instance
         */
        static DATraderOkexApi *CreateApi(const char *dpath = "");
        
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

        //----------------  Spot Trade ---------------------------------------

        /**
         * 获取用户信息
         */
        int GetUserinfo();

        /**
         * 下单交易
         */
        int DoTrade(const char *symbol, const char *type, price_t price, amount_t amount);

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
        int DoWithdraw(const char *symbol, price_t chargefee, const char *tradepwd,
                       const char *withdrawAddress, amount_t withdrawAmount);

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
        int DoBorrowMoney(const char *symbol, const char *days, amount_t amount, price_t rate);

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

        //---------------- Future Trade --------------------------------------

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
                          price_t price, amount_t amount,
                          const char *type, price_t matchprice, price_t leverrate);

        /**
         * 批量下单
         */
        int DoFutureBatchtrade(const char *symbol, const char *contracttype,
                               const char *ordersdata, price_t leverrate);

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