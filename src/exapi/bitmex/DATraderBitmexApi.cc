/*
 * $Id: $
 *
 * DATraderBitmexApi implementation
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */

#include "Base.h"
#include "Log.h"

#include "RestRequest.h"
#include "BitmexApi.h"
#include "trade/DATraderBitmexApi.h"
using namespace exapi;

/**
 * Internal class implemented DATraderOkexApi
 */
class _DATraderBitmexApiImpl : public DATraderBitmexApi {
private:
    friend class DATraderBitmexApi;
    const char         *m_domain;
    std::string         m_api_key;
    std::string         m_secret_key;
    DATraderBitmexSpi  *m_spi;

    _DATraderBitmexApiImpl(const char *api_key, const char *secret_key)
      : m_domain(BITMEX_REST_URL), 
        m_api_key(api_key), m_secret_key(secret_key), m_spi(nullptr) {
        // TODO
    }

public:

    //----------------- Overrides DATraderBitmexApi ---------------------

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
        m_spi = dynamic_cast<DATraderBitmexSpi *>(pEventHandler);
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

//------------------------- DATraderBitmexSpi -----------------------------

int DATraderBitmexSpi::OnResponse(TraderApiType apiType, void *pRespData)
{
    int res = 0;
    int api = (int)apiType - TraderApiType::EX_TYPE_BITMEX;

    switch (api) {

    default:
        LOGFILE(LOG_INFO, "DATraderBitmexSpi::OnResponse unknown command %d", 
                (int)apiType);
        res = -1; // CONTINUE
        break;
    }

    // release allocated resource of pRespData
    return res;
}

//------------------------- DATraderBitmexApi -----------------------------

DATraderBitmexApi *DATraderBitmexApi::CreateApi(const char *api_key, const char *secret_key, const char *dpath)
{
    return new _DATraderBitmexApiImpl(api_key, secret_key);
}


//-------- Trade ---------
        

//----- Trade History ----


//------- Future ---------


//------- Wallet ---------
