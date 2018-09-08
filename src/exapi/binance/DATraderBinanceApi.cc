/*
 * $Id: $
 *
 * DATraderBinanceApi implementation
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */

#include "Base.h"
#include "Log.h"

#include "RestRequest.h"
#include "trade/DATraderBinanceApi.h"
using namespace exapi;

/**
 * Internal class implemented DATraderOkexApi
 */
class _DATraderBinanceApiImpl : public DATraderBinanceApi {
private:
    friend class DATraderBinanceApi;
    const char      *m_domain;
    DATraderOkexSpi *m_spi;
    std::string      m_api_key;
    std::string      m_secret_key;

    _DATraderBinanceApiImpl(const char *api_key, const char *secret_key)
      : m_domain(URL_DOMAIN_COM), m_spi(nullptr), 
        m_api_key(api_key), m_secret_key(secret_key) {
        // TODO
    }

public:

    //----------------- Overrides DATraderBinanceApi ---------------------

    virtual void Dispose() override {
        delete this;
    }

    virtual void Init() override {
    }

    virtual int Join() override {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return 0;
    }

    //------------------- Overrides IQuoteApi -----------------------------

    virtual int ConnServer(const char *slist[], ITraderSpi *pEventHandler) override {
        assert(nullptr != pEventHandler);
        m_spi = dynamic_cast<DATraderBinanceSpi *>(pEventHandler);
        if (m_spi == nullptr) {
            assert(0);
        }
        return 0;
    }

    virtual int DisConnServer(const char *addr) override {
        return 0;
    }
    
    virtual int UserLogon(const char *userContext) override {
        // TODO
        return 0;
    }

};

//------------------------- DATraderBinanceSpi -----------------------------

int DATraderBinanceSpi::OnResponse(int cmdType, void *pRespData)
{
    int res = 0;
    switch (cmdType) {
    case HTTP_API_TYPE_USERINFO:
        OnUserInfo(pRespData);
        break;

    default:
        LOGFILE(LOG_INFO, "DATraderBinanceSpi::OnResponse unknown command %d", cmdType);
        res = -1; // CONTINUE
        break;
    }

    // release allocated resource of pRespData
    return res;
}

//------------------------- DATraderBinanceApi -----------------------------

DATraderBinanceApi *DATraderBinanceApi::CreateApi(const char *api_key, const char *secret_key, const char *dpath)
{
    return new _DATraderBinanceApiImpl(api_key, secret_key);
}


//-------- Spot ---------
        

//-------- Spot Trade History ---------


//------- Future --------


//------- Future Trade History ---------
