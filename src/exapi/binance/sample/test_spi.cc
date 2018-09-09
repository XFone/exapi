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

/*---------------------------- async mode -----------------------------------*/

#include "MyQuoteBinanceSpi.h"

void test_binance_quote_spi()
{
    DAQuoteBinanceApi *api = DAQuoteBinanceApi::CreateApi("", "");
    MyDAQuoteBinanceSpi spi;

    api->ConnServer(nullptr, &spi);
    api->Init();

    api->GetServerTime();
    sleep(3);

    api->GetExchangeInfo();

    api->Join();
    api->Dispose();
}

/*----------------------------- Websocket -----------------------------------*/

// TODO

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

    // test async mode
    test_binance_quote_spi();

    // test websocket
    // test_binance_websocket();
    
    return 0;
}
