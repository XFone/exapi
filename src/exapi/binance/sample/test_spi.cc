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
#include <cstring>
#include <iostream>

/** 
 * Read command line options
 */
int checkopt(int opt, char *parg)
{
    int result = 0;
    switch (opt) {
    case 'c':
        LOGFILE(LOG_INFO, "loadling config file '%s'", parg);
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

    std::string symbol = "eth_btc";
    std::string json = api.GetTicker(symbol);
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
    std::string com_apiKey      = "";                       //请到www.okcoin.com申请
    std::string com_secretKey   = "";                       //请到www.okcoin.com申请
    OKCoinWebSocketApiCom api(com_apiKey, com_secretKey);   //国际站

    api.Run();                              //启动连接服务器线程

    api.ok_spotusd_btc_ticker();            // 订阅行情

    sleep(20);

    api.remove_ok_spotusd_btc_ticker();     // 取消订阅
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
