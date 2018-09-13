/*
 * $Id: $
 * 
 * Interfaces for trader service
 * 
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file ITraderApi.h Interfaces for trader service.
 */

// use __declspec(novtable) in MSVC
#ifndef _INTERFACE_
# define _INTERFACE_         struct
#endif

#include <cstddef>
#include <cstdint>

namespace exapi {

#ifndef DEF_TRADE_TYPES
#define DEF_TRADE_TYPES
    typedef int32_t quantity_t;
    typedef int32_t price_t;                /**< d_price_t  * PRICE_BASE */
    typedef int64_t amount_t;               /**< d_amount_t * PRICE_BASE */
    typedef double  d_quantity_t;           /**< quantity in units of the underlying instrument */
    typedef double  d_price_t;
    typedef double  d_amount_t;
    typedef int64_t volume_t;
    typedef int32_t timestamp_t;
#endif // DEF_TRADE_TYPES

    /**
     * QuoteApiType
     */
    class TraderApiType {
    private:
        int m_apitype;

    public:
      #if (__cplusplus >= 199711L)
        TraderApiType() = delete;
        enum ExType : int { // C++11 required
      #else
        enum ExType {
      #endif
            EX_TYPE_BINANCE     = 0x010000,
            EX_TYPE_BITMEX      = 0x020000,
            EX_TYPE_OKEX        = 0x030000,
            EX_TYPE_HUOBI       = 0x040000
        };

        TraderApiType(const TraderApiType &r) { m_apitype = r.m_apitype; }

        TraderApiType(int apiType) : m_apitype(apiType) {}

        TraderApiType(int type, ExType ex) : m_apitype(ex + type) {}

        operator int() const { return m_apitype; }

    };

    /**
     * ITraderSpi
     * Common trader interface for async callback
     */
    _INTERFACE_ ITraderSpi {
    public:
      // C++0x or C++11
      #if (__cplusplus >= 199711L)
        virtual ~ITraderSpi() = default;
      #endif

        /**
         * On server connected (not used in HTTP/1.1)
         * @param addr server address
         */
        virtual void OnConnected(const char *addr) = 0;

        /**
         * On server disconnected (not used in HTTP/1.1)
         * @param addr server address
         */
        virtual void OnDisconnected(const char *addr) = 0;

        /**
         * Response to UserLogon
         * @param pLogon userContext
         */
        virtual void OnUserLogon(void *pLogon) = 0;

        /**
         * Response is received for a pending command
         * @param apiType api command type
         * @param pRespData response data
         * @return process status (0 for success)
         */
        virtual int OnResponse(TraderApiType apiType, void *pRespData) = 0;
    };

    /**
     * ITraderApi
     * Common trader service
     */
    _INTERFACE_ ITraderApi {
    public:
      // C++0x or C++11
      #if (__cplusplus >= 199711L)
        virtual ~ITraderApi() = default;
      #endif

        /**
         * Connect to quotation or data feed server(s)
         * @param slist server list, last element should be '\0'
         * @param pEventHandler instance of IQuoteSpi for callback
         * @return 0 for sucess, or errono
         */
        virtual int ConnServer(const char *slist[], ITraderSpi *pEventHandler) = 0;

        /**
         * Disconnect from data feed servers
         * @param addr single server adress, or all servers if no given
         * @return 0 for sucess, or errono
         */
        virtual int DisConnServer(const char *addr = nullptr) = 0;

        /**
         * @param userContext user context in json string
         * @return 0 for sucess, or errono
         */
        virtual int UserLogon(const char *userContext) = 0;

        //----------------  More trade commands ------------------------------
    };

} // namespace exapi