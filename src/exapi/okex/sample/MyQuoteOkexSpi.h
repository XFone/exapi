/*
 * MyQuoteOkexSpi implements DAQuoteOkexSpi
 */

#pragma once

#include "quote/DAQuoteOkexApi.h"

#include <iostream>

using namespace std;

class MyQuoteOkexSpi : public DAQuoteOkexSpi {
public:
    MyQuoteOkexSpi() {
        cout << "** MyQuoteOkexSpi created" << endl;
    }

    virtual ~MyQuoteOkexSpi() {
        cout << "** MyQuoteOkexSpi deleted" << endl;
    }

    virtual void OnConnected(const char *addr) {
        cout << "** OnConnected" << endl;
    }

    virtual void OnDisconnected(const char *addr) {
        cout << "** OnDisconnected" << endl;
    }

    virtual void OnQuoteUpdated(QuoteApiType quoteType, void *pQuoteData) {
        int type = (int)quoteType - QuoteApiType::EX_TYPE_OKEX; // see enum HTTP_PROTOCL_TYPE
        cout << "** OnQuoteUpdated - type: " << type << ", json:\n"
             << (const char *)pQuoteData << "\n"
             << endl;
    }

    virtual void OnSymbolSubscribed(QuoteApiType quoteType, const char *pSymbol, unsigned status) {
        cout << "** OnSymbolSubscribed" << endl;
    }

    virtual void OnSymbolUnsubscribed(QuoteApiType quoteType, const char *pSymbol, unsigned status) {
        cout << "** OnSymbolUnsubscribed" << endl;
    }
};