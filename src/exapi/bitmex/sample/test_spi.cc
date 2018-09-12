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

#include "MyQuoteBitmexSpi.h"

void test_bitmex_quote_spi()
{
    DAQuoteBitmexApi *api = DAQuoteBitmexApi::CreateApi("", "");
    DAQuoteBitmexSpi spi;

    const char *slist[] = { "https://www.bitmex.com", "\0" };
    api->ConnServer(slist, &spi);
    api->Init();

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
    test_bitmex_quote_spi();

    // test websocket
    // test_bitmex_websocket();
    
    return 0;
}
