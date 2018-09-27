/*
 * $Id: $
 *
 * Main entry for trader test
 *
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */

#include "ReadConf.ipp"             // read apiKey from config file

#include "BitmexApi.h"              // for BITMEX_REST_TESTNET
#include "MyTraderBitmexSpi.h"

using namespace exapi;

/*---------------------------- async mode -----------------------------------*/

void test_bitmex_trader_spi()
{
    DATraderBitmexApi *api = DATraderBitmexApi::CreateApi(my_apikey.c_str(), my_secret.c_str());
    MyDATraderBitmexSpi spi;

    const char *slist[] = { BITMEX_REST_TESTNET, NULL };
    api->ConnServer(slist, &spi);
    api->Init();

    //------ ApiKey ------

    
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

    // test async mode
    test_bitmex_trader_spi();

    return 0;
}
