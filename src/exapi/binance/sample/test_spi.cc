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
#include "Trace.h"
#include "osdeps/osutil.h"
#include "ReadConf.ipp"             // read apiKey from config file

#include <cstring>
#include <system_error>
#include <iostream>

#include "MyQuoteBinanceSpi.h"

/*---------------------------- async mode -----------------------------------*/

void test_binance_quote_spi()
{
    DAQuoteBinanceApi *api = DAQuoteBinanceApi::CreateApi(my_apikey.c_str(), my_secret.c_str());
    MyDAQuoteBinanceSpi spi;

    try {
        const char *slist[] = { "https://www.binance.co", "\0" };
        api->ConnServer(slist, &spi);
        api->Init();

        api->GetServerTime();
        // sleep(3);

        api->GetExchangeInfo();
        // sleep(3);

        api->Join();
        api->Dispose();
    } catch (exception &ex) {
        LOGFILE(LOG_ERROR, "exception caught - %s('%s')", type_name(ex).data(), ex.what()); 
    }
}

/*----------------------------- Websocket -----------------------------------*/

// TODO

int main(int argc, char *argv[])
{
    int res;

#if !(_WIN32 || _WIN64)
    // catch segfault and print backtrace to stderr
    catch_segfault_cpp(nullptr);
#endif

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
