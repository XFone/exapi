/*
 * $Id: $
 *
 * Main entry
 *
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */

#include "Base.h"
#include "Log.h"
#include "ConConf.h"
#include "Trace.h"

#include <cstdio>
#include <string>
#include <iostream>

#include "okcoinapi.h"

/** 
 * Read command line options
 */
int checkopt(int opt, char *parg)
{
    int result = 0;
    switch (opt) {
    case 'c':
        // read_config_file(parg, on_read_keyval);
        break;

    default:
        result = -1;
        break;
    }

    return result;
}

/*----------------------------- sync mode -----------------------------------*/

#include "okcoinapi.h"

void test_okex_quote_sync()
{
    OKCoinApiCom api("", "");

    string symbol = "eth_btc";
    string json = api.GetTicker(symbol);
}

/*---------------------------- async mode -----------------------------------*/

#include "MyQuoteOkexSpi.h"

void test_okex_quote_spi()
{
    DAQuoteOkexApi *api = DAQuoteOkexApi::CreateApi();
    MyQuoteOkexSpi spi;

    api->ConnServer(nullptr, &spi);
    api->Init();

    api->GetTicker("etc_btc");

    api->Join();
    api->Dispose();
}

/*----------------------------- Websocket -----------------------------------*/

#include "okcoinwsapi.h"

void test_okex_websocket()
{
    try {
        OKCoinWebSocketApiCom api("", "");      //国际站

        api.Run();                              // 启动连接服务器线程

        sleep(3);

        api.ok_spotusd_btc_ticker();            // 订阅行情

        sleep(20);

        api.remove_ok_spotusd_btc_ticker();     // 取消订阅
    } catch (exception &ex) {
        LOGFILE(LOG_ERROR, "exception caught - %s('%s')", typeid(ex).name(), ex.what()); 
    }
}

int main(int argc, char *argv[])
{
    int res;

    // this auto parsing -t <trace-level> and -v <log-level> arguments
    if ((res = parse_args(argc, argv, "c:", checkopt)) < 0 ||
        optind != argc) {
        print_usage(argv[0], "<name>",
            "  -c <Config File>           Specify config file\n"
        );
        return -1;
    }

    // test sync mode
    test_okex_quote_sync();

    // test async mode
    test_okex_quote_spi();

    // test websocket
    test_okex_websocket();
    
    return 0;
}
