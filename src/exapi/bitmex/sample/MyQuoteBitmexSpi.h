/*
 * MyDAQuoteBitmexSpi implements DAQuoteBitmexSpi
 */

#pragma once

#include "quote/DAQuoteBitmexApi.h"

#include <iostream>

using namespace std;
using namespace exapi;

class MyDAQuoteBitmexSpi : public DAQuoteBitmexSpi {
public:
    MyDAQuoteBitmexSpi() {
        cout << "** MyDAQuoteBitmexSpi created" << endl;
    }

    virtual ~MyDAQuoteBitmexSpi() {
        cout << "** MyDAQuoteBitmexSpi deleted" << endl;
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

};