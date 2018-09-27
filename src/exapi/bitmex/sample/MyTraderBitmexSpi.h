/*
 * MyDATraderBitmexSpi implements DATraderBitmexSpi
 */

#pragma once

#include <iostream>

#include "trade/DATraderBitmexApi.h"

#include "JsonUtils.h"
#include "BitmexApiDef.h"

using namespace std;
using namespace exapi;

class MyDATraderBitmexSpi : public DATraderBitmexSpi {
public:
    MyDATraderBitmexSpi() {
        cout << "** MyDATraderBitmexSpi created" << endl;
    }

    virtual ~MyDATraderBitmexSpi() {
        cout << "** MyDATraderBitmexSpi deleted" << endl;
    }

    virtual void OnConnected(const char *addr) {
        cout << "** OnConnected" << endl;
    }

    virtual void OnDisconnected(const char *addr) {
        cout << "** OnDisconnected" << endl;
    }

    virtual void OnUserLogon(void *pLogon) {
        cout << "** OnUserLogon" << endl;
    }

    virtual int OnResponse(TraderApiType apiType, void *pRespData) {
        cout << "** OnResponse" << endl;
        return 0;
    }

    //--------------------- Callbacks ------------------------

    virtual void OnApiKey(const void *pRespData) {
        cout << "** OnApiKey" << endl;
    }

    //------------------ Chat callbacks ----------------------

    virtual void OnChatMessages(const void *msgs) {
        cout << "** OnChatMessages" << endl;
    }

    virtual void OnChatChannels(const void *channels) {
        cout << "** OnChatChannels" << endl;
    }

    virtual void OnConnectedUsers(const void *users) {
        cout << "** OnConnectedUsers" << endl;
    }

    virtual void OnSentMessage(const void *chat) {
        cout << "** OnSentMessage" << endl;
    }

    //------------------ Account callbacks ------------------

    virtual void OnUserInfo(const void *pUserInfo) {
        cout << "** OnUserInfo" << endl;
    }

    virtual void OnLogout(const void *nothing) {
        cout << "** OnLogout" << endl;
    }

    virtual void OnLogoutAll(const char *value) {
        cout << "** OnLogoutAll" << endl;
    }
    
    virtual void OnEmailConfirmed(const void *accessToken) {
        cout << "** OnEmailConfirmed" << endl;
    }

    virtual void OnTFAComfirmed(const char *token, bool enabled) {
        cout << "** OnTFAComfirmed" << endl;
    }

    virtual void OnTFAEnabled(bool enabled) {
        cout << "** OnTFAEnabled" << endl;
    }

    virtual void OnTFADisabled(const char *token, bool disabled) {
        cout << "** OnTFADisabled" << endl;
    }

    virtual void OnQueryUser(const void *user) {
        cout << "** OnQueryUser" << endl;
    }

    virtual void OnUserUpdated(const void *user) {
        cout << "** OnUserUpdated" << endl;
    }

    virtual void OnUserAffiliateStatus(const void *affiliate) {
        cout << "** OnUserAffiliateStatus" << endl;
    }

    virtual void OnUserCommissionStatus(const void *commissions) {
        cout << "** OnUserCommissionStatus" << endl;
    }

    virtual void OnUserMarginStatus(const void *margin) {
        cout << "** OnUserMarginStatus" << endl;
    }

    virtual void OnUserPreferences(const void *user) {
        cout << "** OnUserPreferences" << endl;
    }

    virtual void OnNotification(const void *notifies) {
        cout << "** OnNotification" << endl;
    }

    //--------------------- Trade callbacks ------------------

    virtual void OnOrderPlaced(const void *order_json) {
        struct Order order { NULL };
        JsonUtils::from_json((const char *)order_json, order);
        // using order ...
    }

    virtual void OnOrdersPlaced(const void *orders) {
        cout << "** OnOrdersPlaced" << endl;
    }

    virtual void OnOrdersCancelled(const void *orders_json) {
        cout << "** OnOrdersCancelled" << endl;
    }

    virtual void OnOrderUpdated(const void *order) {
        cout << "** OnOrderUpdated" << endl;
    }

    virtual void OnOrdersUpdated(const void *orders) {
        cout << "** OnOrdersUpdated" << endl;
    }

    virtual void OnExecutions(const void *executions) {
        cout << "** OnExecutions" << endl;
    }

    //------------------ Instrument callbacks ----------------

    virtual void OnInstruments(const void *instruments) {
        cout << "** OnInstruments" << endl;
    }

    virtual void OnPriceIndices(const void *instruments) {
        cout << "** OnPriceIndices" << endl;
    }

    virtual void OnActiveIntervals(const void *instruments) {
        cout << "** OnActiveIntervals" << endl;
    }

    virtual void OnCompositeIndex(const void *instruments) {
        cout << "** OnCompositeIndex" << endl;
    }

    //----------------- Trade history callbacks --------------

    virtual void OnQueryOrders(const void *orders_json) {
        cout << "** OnQueryOrders" << endl;
        std::vector<Order> orders;
        // JsonUtils::Json json((const char *)orders_json);
        // TODO parse array...
        // struct Order order { NULL };
        // (void)from_json(json, orders);
    }

    virtual void OnTradeHistory(const void *executions) {
        cout << "** OnTradeHistory" << endl;
    }

    //----------------- Future callbacks ---------------------

    virtual void OnQueryPositions(const void *positions) {
        cout << "** OnQueryPositions" << endl;
    }

    virtual void OnPositionIsolateMargin(const void *position) {
        cout << "** OnPositionIsolateMargin" << endl;
    }

    virtual void OnPositionTransferMargin(const void *position) {
        cout << "** OnPositionTransferMargin" << endl;
    }

    virtual void OnPositionLeverage(const void *position) {
        cout << "** OnPositionLeverage" << endl;
    }

    virtual void OnPositionRiskLimit(const void *position) {
        cout << "** OnPositionRiskLimit" << endl;
    }

    virtual void OnQuerySettlement(const void *settlements) {
        cout << "** OnQuerySettlement" << endl;
    }

    //-------------- Account wallet callbacks ----------------

    virtual void OnResponseWithdraw(const void *transaction) {
        cout << "** OnResponseWithdraw" << endl;
    }

    virtual void OnWithdrawCancelled(const void *transaction) {
        cout << "** OnWithdrawCancelled" << endl;
    }

    virtual void OnWithdrawConfirmed(const void *transaction) {
        cout << "** OnWithdrawConfirmed" << endl;
    }

    virtual void OnDepositAddress(const char *address) {
        cout << "** OnDepositAddress" << endl;
    }

    virtual void OnQueryWallet(const void *wallet) {
        cout << "** OnQueryWallet" << endl;
    }

    virtual void OnWalletHistory(const void *transactions) {
        cout << "** OnWalletHistory" << endl;
    }

    virtual void OnWalletSummary(const void *transactions) {
        cout << "** OnWalletSummary" << endl;
    }

    virtual void OnMinWithdrawalFee(const void *object) {
        cout << "** OnMinWithdrawalFee" << endl;
    }

    //----------------- Top user callbacks -------------------

    virtual void OnLeadboard(const void *leaderboards) {
        cout << "** OnLeadboard" << endl;
    }

    virtual void OnLeadboardName(const char *name) {
        cout << "** OnLeadboardName" << endl;
    }

};