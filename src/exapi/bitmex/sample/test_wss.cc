/*
 * $Id: $
 *
 * Main entry for websocket test
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
#include <system_error>

#include "JsonUtils.h"


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

/*---------------------------- Websocket -----------------------------------*/

#include "BitmexWsApi.h"
using namespace exapi;
using namespace std;

void on_message(const char *msg) {
    cout << "[websockect]: " << msg << endl;
}

void test_bitmex_websocket()
{
    BitmexWsApi api("", "");

    try {
        // BITMEX_WSS_URL, BITMEX_WSS_TESTNET
        // BITMEX_WSS_PATH, BITMEX_WSS_MUX_PATH
        api.SetUrl(BITMEX_WSS_TESTNET BITMEX_WSS_PATH); // using testnet

        api.set_message_callback(on_message);

        api.start();                            // start work thread

        sleep(3);

        const char *topics[] = {
            "orderBookL2_25",
            "quoteBin1m",
            "trade",
            NULL
        };

        api.Subscribe(topics);                 // subscribe to subjects/topics

        sleep(30);

        api.stop();

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

   // test websocket
    test_bitmex_websocket();

    return 0;
}
