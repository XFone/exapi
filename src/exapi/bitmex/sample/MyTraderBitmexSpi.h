/*
 * MyDATraderBitmexSpi implements DATraderBitmexSpi
 */

#pragma once

#include "trade/DATraderBitmexApi.h"

#include <iostream>

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


};