/*
 * $Id: $
 *
 * Quotation utility class for auto-trade strategy and algorithm system
 *  
 * Copyright (c) 2014-2017 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file QuotationData.h Quotation and Data Feed Interface */
#include "datasource/Common.h"
#include "protos/quotation.pb.h"
#include "protos/Enum2Message.h"

#ifdef USE_TBB
#include <tbb/queuing_rw_mutex.h>
#endif

#include <string>
#include <deque>
#include <memory>

namespace AlgoApi {

    using namespace std;
    using namespace DataSource;
    using namespace Protos;
 
#ifndef DEF_TRADE_TYPES
#define DEF_TRADE_TYPES
    typedef int32_t quantity_t;
    typedef int32_t price_t;                /**< d_price_t  * PRICE_BASE */
    typedef int64_t amount_t;               /**< d_amount_t * PRICE_BASE */
    typedef double  d_price_t;
    typedef double  d_amount_t;
    typedef int64_t volume_t;
    typedef int32_t timestamp_t;
#endif // DEF_TRADE_TYPES

    typedef quotation::Stock *PStockData;
    //typedef SharedObject QuotationData;

#ifdef USE_TBB
    typedef tbb::queuing_rw_mutex   TMUTEX_RW;
#endif

    const size_t     INIT_HIST_SIZE = 3;    /**< save latest three history */
    const price_t    NULL_PRICE  = -1;
    const quantity_t NULL_QTY    = 0;
    const price_t    PRICE_BASE  = 10000L;  /**< all price should divide this */

    enum QuotationType {
        QT_IOPV       = 'C',    // 分级基金IOPV
        QT_INDEX      = 'I',    // 指数行情
        QT_STOCK      = 'S',    // 股票行情
        QT_FUTURE     = 'F',    // 期货行情
        QT_OPTION     = 'O',    // 期权行情
        QT_VOLATILITY = 'P',    // 期权波动率
        QT_VINDEX     = 'V',    // ETF预估净值
        QI_HANDICAP   = 'H',    // 全息盘口

        QT_ORDERDATA  = 'D',    // Level2行情 逐笔委托
        QT_ORDERQUEUE = 'Q',    // Level2行情 委托队列
        QT_ORDERTRANS = 'T',    // Level2行情 逐笔成交

        QT_CURRENTBASIS = 'B',  // 当前盘口位的基差
        
        QT_MAX_INVALID = '~'    // 126 
    };

    /**
     * QuotationData
     * Quotation and data feed interface for policy strategy
     */
    class QuotationData {
    private:
        string              m_code;
        string              m_name;
        price_t             m_latest;
        price_t             m_open;
        price_t             m_preclose;
        price_t             m_upperlimit;   /**< 涨停价 */
        price_t             m_lowerlimit;   /**< 跌停价 */
        volume_t            m_volume;       /**< 成交数量      */
        volume_t            m_volume_sum;   /**< 市场总成交数量 */
        amount_t            m_amount_sum;   /**< 市场总成交金额 */
        timestamp_t         m_timestamp;

        size_t              m_max_hist;
        deque<PMessage>     m_hist;
        TMUTEX_RW           m_mutex;

        /**
         * NOT thread-safe
         */
        PMessage _last() const {
            return m_hist.back();
        }

        timestamp_t _stamp(int idx) const;

        price_t _checkAdjust(price_t price) const;

    public:
        QuotationData() : m_latest(NULL_PRICE), m_open(NULL_PRICE), 
            m_preclose(NULL_PRICE), m_upperlimit(NULL_PRICE), m_lowerlimit(NULL_PRICE),
            m_volume(0), m_volume_sum(0), m_amount_sum(0L), m_timestamp(0), 
            m_max_hist(INIT_HIST_SIZE), m_hist() {
            // NOTHING
        }

        ~QuotationData();

        void Push(PMessage msg);

        const QuotationType Type() const {
            return QT_STOCK; // TODO: QT_FUTURE, QT_OPTION
        }

        const string &Code() const {
            return m_code;
        }

        const string &Name() const {
            return m_name;
        }

        timestamp_t TimeStamp() const {
            return m_timestamp;
        }

        price_t PreClose() const {
            return m_preclose;
        }

        price_t UpperLimit() const {
            return m_upperlimit;
        }

        price_t LowerLimit() const {
            return m_lowerlimit;
        }

        /**
         * Get new_price
         */
        price_t Latest();

        price_t Open(int idx = 0);   
        price_t Close(int idx = 0);
        price_t High(int idx = 0);
        price_t Low(int idx = 0);
        price_t Iopv(int idx = 0);
        
        price_t BuyAdjust();
        price_t SellAdjust();

        price_t BuyLevel(int lev);
        price_t SellLevel(int lev);

        volume_t LatestVolume();
        volume_t TotalVolume();

        volume_t BoughtVolume(int idx);
        volume_t SoldVolume(int idx);

        amount_t TotalAmount();
 
        // ----------------------- Market Routines --------------------------

        /** Get market code - defined in qstock_proto.proto and qpolicy_proto.proto
         * (中国)市场代码
         * @param code security product code
         * @return 0-未知 1-上海 2-深圳 3-上海B 4-深圳B 5-中金所 6-上期所 7-大商所 8-郑商所 9-特转A 10-特转B, 11-三板
         */
        static int GetMarket(const string &code);

        // ----------------------- Stock Routines --------------------------

        static price_t GetStockBuyLevelPrice(int level, const PStockData stock);

        static price_t GetStockSellLevelPrice(int level, const PStockData stock);

        // ----------------------- Future Routines -------------------------
        // TODO

        // ----------------------- Option Routines -------------------------
        // TODO

        // ----------------------- Trigger Routines ------------------------
        //void set_trigger(price_t high, price_t low, callback);
    };

}
