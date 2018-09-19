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

    //-------------------------- Quote Data ------------------------------

    virtual void OnOrderBook(const void *l2data) {
        cout << "** OnOrderBook\n" 
             << (const char *)l2data << endl;
    }

    virtual void OnQuotes(const void *quotes) {
        cout << "** OnQuotes\n" 
             << (const char *)quotes << endl;
    }

    virtual void OnQuotesBucketed(const void *quotes) {
        cout << "** OnQuotesBucketed\n" 
             << (const char *)quotes << endl;
    }

    //------------------------- Funding callbacks ------------------------

    virtual void OnFundingHistory(const void *fundings) {
        cout << "** OnFundingHistory\n" 
             << (const char *)fundings << endl;
    }

    virtual void OnInsuranceHistory(const void *insurances) {
        cout << "** OnInsuranceHistory\n" 
             << (const char *)insurances << endl;
    }

    //-------------------------- Trades Data -----------------------------

    virtual void OnTrades(const void *trades) {
        cout << "** OnTrades\n" 
             << (const char *)trades << endl;
    }

    virtual void OnTradesBucketd(const void *tradebin) {
        cout << "** OnTradesBucketd\n" 
             << (const char *)tradebin << endl;
    }

    //-------------------------- Extra Data ------------------------------

    virtual void OnAnnouncement(const void *data) {
        cout << "** OnAnnouncement\n" 
             << (const char *)data << endl;
    }

    virtual void OnAnnouncementUrgent(const void *data) {
        cout << "** OnAnnouncementUrgent\n"
             << (const char *)data << endl;
    }

    virtual void OnSchema(const void *schema) {
        cout << "** OnSchema\n"
             << (const char *)schema << endl;
    }

    virtual void OnSchemaWebsocket(const void *schema) {
        cout << "** OnSchemaWebsocket\n"
             << (const char *)schema << endl;
    }

    virtual void OnStats(const void *stats) {
        cout << "** OnStats\n"
             << (const char *)stats << endl;
    }

    virtual void OnStatsHistory(const void *stats) {
        cout << "** OnStatsHistory\n"
             << (const char *)stats << endl;
    }

    virtual void OnStatsHistoryUsd(const void *stats) {
        cout << "** OnStatsHistoryUsd\n"
             << (const char *)stats << endl;
    }
};