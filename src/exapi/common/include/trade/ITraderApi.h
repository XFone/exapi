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
    typedef int32_t price_t;
    typedef int64_t volume_t;
    typedef int64_t amount_t;
    typedef int32_t timestamp_t;
#endif // DEF_TRADE_TYPES

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
         * @param cmdType command type
         * @param pRespData response data
         * @return process status (0 for success)
         */
        virtual int OnResponse(int cmdType, void *pRespData) = 0;
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