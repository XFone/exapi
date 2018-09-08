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

    virtual void OnQuoteUpdated(int quoteType, void *pQuoteData) {
        cout << "** OnQuoteUpdated - type: " << quoteType << ", json:\n"
             << (const char *)pQuoteData << "\n"
             << endl;
    }

    virtual void OnSymbolSubscribed(int quoteType, const char *pSymbol, unsigned status) {
        cout << "** OnSymbolSubscribed" << endl;
    }

    virtual void OnSymbolUnsubscribed(int quoteType, const char *pSymbol, unsigned status) {
        cout << "** OnSymbolUnsubscribed" << endl;
    }
};