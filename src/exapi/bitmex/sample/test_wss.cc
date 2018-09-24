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
#include "BitmexWsApi.h"

using namespace exapi;
using namespace std;

string my_apikey("");
string my_secret("");

/**
 * Read command line options
 */
int checkopt(int opt, char *parg)
{
    int result = 0;
    switch (opt) {
    case 'c':
        LOGFILE(LOG_INFO, "loadling config file '%s'", parg);
        read_config_file(parg, [](const char *key, const char *vals[]) {
            if (!strcmp(key, "ApiKey")) {
                my_apikey = vals[0];
                cout << "Read ApiKey " << my_apikey << endl;
                return;
            } 
            if (!strcmp(key, "Secret")) {
                my_secret = vals[0];
                cout << "Read Secret " << my_secret.substr(0, 2) << "..." << endl;
                return;
            }
        });
        break;

    default:
        result = -1;
        break;
    }

    return result;
}

/*---------------------------- Websocket -----------------------------------*/

void on_message(const char *msg) {
    cout << "[websockect]: " << msg << endl;
}

void test_bitmex_websocket()
{
    string real_secret = my_secret;
    // prompt for password
    
    BitmexWsApi api(my_apikey, real_secret);

    try {
        // BITMEX_WSS_URL, BITMEX_WSS_TESTNET
        // BITMEX_WSS_PATH, BITMEX_WSS_MUX_PATH
        api.SetUrl(BITMEX_WSS_TESTNET BITMEX_WSS_PATH); // using testnet

        api.set_message_callback(on_message);

        api.start();                            // start work thread

        sleep(1);

        const char *public_topics[] = {
            "publicNotifications",
            "orderBookL2_25",
            "quoteBin1m",
            "trade",
            NULL
        };

        api.Subscribe(public_topics);           // subscribe to subjects/topics

        sleep(10);
    #if 0
        api.Authentication();
        sleep(3);

        const char *private_topics[] = {
            "position"
            // ...
        };

        api.Subscribe(private_topics); 
    #endif 

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
