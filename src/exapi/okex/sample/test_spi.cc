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

#include "okcoinapi.h"

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

    test_okex_quote_spi();
    
    return 0;
}
