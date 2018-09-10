/*
 * MyDAQuoteBinanceSpi implements DAQuoteBinanceSpi
 */

#pragma once

#include "quote/DAQuoteBinanceApi.h"

#include <iostream>

using namespace std;
using namespace exapi;

class MyDAQuoteBinanceSpi : public DAQuoteBinanceSpi {
public:
    MyDAQuoteBinanceSpi() {
        cout << "** MyDAQuoteBinanceSpi created" << endl;
    }

    virtual ~MyDAQuoteBinanceSpi() {
        cout << "** MyDAQuoteBinanceSpi deleted" << endl;
    }

    virtual void OnConnected(const char *addr) {
        cout << "** OnConnected" << endl;
    }

    virtual void OnDisconnected(const char *addr) {
        cout << "** OnDisconnected" << endl;
    }

    virtual void OnQuoteUpdated(QuoteApiType quoteType, void *pQuoteData) {
        cout << "** OnQuoteUpdated - type: " << (int)quoteType << ", json:\n"
             << (const char *)pQuoteData << "\n"
             << endl;
    }

    virtual void OnSymbolSubscribed(QuoteApiType quoteType, const char *pSymbol, unsigned status) {
        cout << "** OnSymbolSubscribed" << endl;
    }

    virtual void OnSymbolUnsubscribed(QuoteApiType quoteType, const char *pSymbol, unsigned status) {
        cout << "** OnSymbolUnsubscribed" << endl;
    }

    virtual void OnServerTime(time_t serverTime) {
        cout << "** OnServerTime - serverTime: " << serverTime << endl;
    }

    virtual void OnExchangeInfo(const char *exinfo) {
        cout << "** OnExchangeInfo\n" 
             << exinfo << endl;
    }
};