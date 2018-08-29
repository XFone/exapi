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

#include "Base.h"
#include "Log.h"
#include "Trace.h"

#include "QuotationData.h"
#include "QuotationIntf.h"

#include <cctype>

using namespace AlgoApi;

inline int _checkLimit(int idx, size_t size)
{
    return ((idx >= (int)size) ? (size - 1) : idx);
}

timestamp_t QuotationData::_stamp(int idx) const {
    assert(idx < (int)m_hist.size());
    int n = _checkLimit(idx, m_hist.size());
    return static_cast<PStockData>(m_hist[n])->stamp();
}

price_t QuotationData::_checkAdjust(price_t price) const {
    if (price > m_upperlimit || price < m_lowerlimit) {
        LOGFILE(LOG_ERROR, "price %d out of range[%d, %d], use lastest %d",
                price, m_lowerlimit, m_upperlimit, m_latest);
        price = m_latest;
    }
    return price;
}


QuotationData::~QuotationData()
{
    TMUTEX_RW::scoped_lock(m_mutex, true); // write
    for (PMessage msg: m_hist) {
        if (NULL != msg) {
            QuotationIntf::DisposeQuotation(msg);
        }
    }
}

void QuotationData::Push(PMessage msg)
{
    assert(dynamic_cast<PStockData>(msg) != NULL);
    PStockData stock = static_cast<PStockData>(msg);

    if (NULL != stock) { // QT_STOCK
        TMUTEX_RW::scoped_lock(m_mutex, true); // write

        if (m_hist.size() >= m_max_hist) {
            PMessage old = m_hist.front();
            m_hist.pop_front();
            QuotationIntf::DisposeQuotation(old);
        }

        PMessage now = msg->New();
        now->CopyFrom(*msg);  // use unique_ptr ?
        m_hist.push_back(now);

        if (m_code.empty()) {
            m_code = stock->code();
            m_name = stock->name();
        }
        
        // --- instant values
        if (stock->new_price() > 0) {
            m_latest = stock->new_price();
        } else {
            LOGFILE(LOG_WARN, "quotation '%s' new_price is not set", m_code.c_str());
        }

        if (stock->new_volume() > 0) {
            m_volume = stock->new_volume();
        }
        if (stock->sum_volume() > 0) {
            m_volume_sum = stock->sum_volume();
        }
        if (stock->sum_amount() > 0) {
            m_amount_sum = stock->sum_amount();
        }

        // --- static values ---
        if (m_open <= 0 && stock->open() > 0) {
            m_open = stock->open();
        }
        if (m_preclose <= 0 && stock->preclose() > 0) {
            m_preclose = stock->preclose();
        }
        if (m_upperlimit <= 0 && stock->upperlimit_price() > 0) {
            m_upperlimit = stock->upperlimit_price();
            assert(m_upperlimit >= m_latest);
        }
        if (m_lowerlimit <= 0 && stock->lowerlimit_price() > 0) {
            m_lowerlimit = stock->lowerlimit_price();
            assert(m_lowerlimit <= m_latest);
        }

        m_timestamp = stock->stamp(); // trading_day()
    }
}


price_t QuotationData::Latest()
{
    TMUTEX_RW::scoped_lock(m_mutex, false);
    return m_latest;
}

price_t QuotationData::Open(int idx)
{
    if (idx >= 0) {
        TMUTEX_RW::scoped_lock(m_mutex, false);
        int n = _checkLimit(idx, m_hist.size());
        return static_cast<PStockData>(m_hist[n])->open();        
    }
    return NULL_PRICE;
}

price_t QuotationData::Close(int idx)
{
    if (idx == -1) return m_preclose;
    return NULL_PRICE;
}

price_t QuotationData::High(int idx)
{
    if (idx >= 0) {
        TMUTEX_RW::scoped_lock(m_mutex, false);
        int n = _checkLimit(idx, m_hist.size());
        return static_cast<PStockData>(m_hist[n])->high();        
    }
    return NULL_PRICE;
}

price_t QuotationData::Low(int idx)
{
    if (idx >= 0) {
        TMUTEX_RW::scoped_lock(m_mutex, false);
        int n = _checkLimit(idx, m_hist.size());
        return static_cast<PStockData>(m_hist[n])->low();        
    }
    return NULL_PRICE;
}

price_t QuotationData::Iopv(int idx)
{
    if (idx >= 0) {
        TMUTEX_RW::scoped_lock(m_mutex, false);
        int n = _checkLimit(idx, m_hist.size());
        return static_cast<PStockData>(m_hist[n])->iopv();        
    }
    return NULL_PRICE;
}

price_t QuotationData::BuyAdjust()
{
    TMUTEX_RW::scoped_lock(m_mutex, false);
    return static_cast<PStockData>(_last())->bp1_ajust();
}

price_t QuotationData::SellAdjust()
{
    TMUTEX_RW::scoped_lock(m_mutex, false);
    return static_cast<PStockData>(_last())->sp1_ajust();
}

price_t QuotationData::BuyLevel(int lev)
{
    TMUTEX_RW::scoped_lock(m_mutex, false);
    PStockData stock = static_cast<PStockData>(_last());
    return _checkAdjust(GetStockBuyLevelPrice(lev, stock));
}

price_t QuotationData::SellLevel(int lev)
{
    TMUTEX_RW::scoped_lock(m_mutex, false);
    PStockData stock = static_cast<PStockData>(_last());
    return _checkAdjust(GetStockSellLevelPrice(lev, stock));
}

volume_t QuotationData::LatestVolume()
{
    TMUTEX_RW::scoped_lock(m_mutex, false);
    return m_volume;
}

volume_t QuotationData::TotalVolume()
{
    TMUTEX_RW::scoped_lock(m_mutex, false);
    return m_volume_sum;
}

volume_t QuotationData::BoughtVolume(int idx)
{
    if (idx >= 0) {
        TMUTEX_RW::scoped_lock(m_mutex, false);
        int n = _checkLimit(idx, m_hist.size());
        return static_cast<PStockData>(m_hist[n])->bought_volume();        
    }
    return NULL_PRICE;
}

volume_t QuotationData::SoldVolume(int idx)
{
    if (idx >= 0) {
        TMUTEX_RW::scoped_lock(m_mutex, false);
        int n = _checkLimit(idx, m_hist.size());
        return static_cast<PStockData>(m_hist[n])->sold_volume();        
    }
    return NULL_PRICE;
}

amount_t QuotationData::TotalAmount()
{
    TMUTEX_RW::scoped_lock(m_mutex, false);
    return m_amount_sum;    
}

int QuotationData::GetMarket(const string &code)
{
    // 0-未知 1-上海 2-深圳 3-上海B 4-深圳B 5-中金所 6-上期所 7-大商所 8-郑商所 9-特转A 10-特转B, 11-三板
    int market = 0; // unknown
    char ch0 = code[0];

    if (std::isdigit(ch0)) {
        long l = std::stol(code);
        market = (l < 400000L) ? 2 : 1;
    } else if (ch0 == 'I') {
        // IF, IC, IH
        char ch1 = code[1];
        if (ch1 == 'F' || ch1 == 'C' || ch1 == 'H') {
            market = 5; // 5-中金所
        } else {
            LOGFILE(LOG_ERROR, "Unknown market for code %s", code.c_str());
        }
    }
    return market;
}

price_t QuotationData::GetStockBuyLevelPrice(int level, const PStockData stock)
{
    price_t price = NULL_PRICE;

    // TODO: check stock->bpxx() is provided
    switch (level) {
        case 1:     price = stock->bp1();     break;
        case 2:     price = stock->bp2();     break;
        case 3:     price = stock->bp3();     break;
        case 4:     price = stock->bp4();     break;
        case 5:     price = stock->bp5();     break;
        case 6:     price = stock->bp6();     break;
        case 7:     price = stock->bp7();     break;
        case 8:     price = stock->bp8();     break;
        case 9:     price = stock->bp9();     break;
        case 10:    price = stock->bp10();    break;

        default:
            LOGFILE(LOG_ERROR, "invalid buy price level: order_price_level = %d",
                    level);
            break;
    } // switch (leve)

    return price;
}

price_t QuotationData::GetStockSellLevelPrice(int level, const PStockData stock)
{
    price_t price = NULL_PRICE;

    // TODO: check stock->spxx() is provided
    switch (level) {
        case 1:     price = stock->sp1();     break;
        case 2:     price = stock->sp2();     break;
        case 3:     price = stock->sp3();     break;
        case 4:     price = stock->sp4();     break;
        case 5:     price = stock->sp5();     break;
        case 6:     price = stock->sp6();     break;
        case 7:     price = stock->sp7();     break;
        case 8:     price = stock->sp8();     break;
        case 9:     price = stock->sp9();     break;
        case 10:    price = stock->sp10();    break;

        default:
            LOGFILE(LOG_ERROR, "invalid sell price level: order_price_level = %d",
                    level);
            break;
    } // switch (leve)

    return price;
}
