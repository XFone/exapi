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

#include "BinanceApi.h"
#include "JsonUtils.h"
#include "trade/DATraderBinanceApi.h"
#include "detail/RestRequestImpl.h"
using namespace exapi;

/**
 * Internal class implemented DATraderOkexApi
 */
class _DATraderBinanceApiImpl : public DATraderBinanceApi {
private:
    friend class DATraderBinanceApi;
    const char         *m_domain;
    std::string         m_api_key;
    std::string         m_secret_key;
    DATraderBinanceSpi *m_spi;

    _DATraderBinanceApiImpl(const char *api_key, const char *secret_key)
      : m_domain(BINANCE_REST_URL), 
        m_api_key(api_key), m_secret_key(secret_key), m_spi(nullptr) {
        // TODO
    }

public:

    //----------------- Overrides DATraderBinanceApi ---------------------

    virtual void Dispose() override {
        DisConnServer(nullptr);
        delete this;
    }

    virtual void Init() override {
        // TODO
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    virtual int Join() override {
        // TODO
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        return 0;
    }

    //------------------- Overrides IQuoteApi -----------------------------

    virtual int ConnServer(const char *slist[], ITraderSpi *pEventHandler) override {
        assert(nullptr != pEventHandler);
        m_spi = dynamic_cast<DATraderBinanceSpi *>(pEventHandler);
        if (m_spi == nullptr) {
            assert(0);
        }
        if (nullptr != slist) {
            m_domain = slist[0];    // TODO: try fastest server
        }

        auto client = HttpRestClient::GetInstance(m_domain);

        const int wait_time = 5;
        int wait = 0, max_wait = 5000;

        while (!client->is_open() && wait < max_wait) {
            std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
            wait += wait_time;
        }

        if (client->is_open()) {
            LOGFILE(LOG_INFO, "'%s' connected in %d ms", m_domain, wait);
            return 0;
        }
        
        return -ETIMEDOUT;
    }

    virtual int DisConnServer(const char *addr) override {
        std::string server((addr == nullptr) ? m_domain : addr);
        HttpRestClient::DisposeInstance(server);
        return 0;
    }
    
    virtual int UserLogon(const char *userContext) override {
        // TODO
        return 0;
    }

};

//------------------------- DATraderBinanceSpi -----------------------------

int DATraderBinanceSpi::OnResponse(TraderApiType apiType, void *pRespData)
{
    int res = 0;
    int api = (int)apiType - TraderApiType::EX_TYPE_BINANCE;

    switch (api) {
    case BINANCE_TYPE_ACCOUNT_INFO:
        OnUserInfo(pRespData);
        break;

    default:
        LOGFILE(LOG_INFO, "DATraderBinanceSpi::OnResponse unknown command %d", 
                (int)apiType);
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
