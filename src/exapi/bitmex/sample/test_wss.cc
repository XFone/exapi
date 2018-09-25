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
#include "osdeps/osutil.h"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
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
                string secret(vals[0]);
                if (secret.rfind(".key") != string::npos) { // load from keyfile
                    ifstream is;
                    is.open(secret, ios_base::in);
                    if (is.is_open()) {
                        is >> my_secret;   // read from keyfile
                        is.close();
                        cout << "Read Secret ('" << secret
                             << "') " << my_secret.substr(0, 2) << "..." << endl;
                    }
                } else {
                    my_secret = secret;
                    cout << "Read Secret " << my_secret.substr(0, 2) << "..." << endl;
                }
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

/*---------------------------- Signature -----------------------------------*/

void test_bitmex_signature()
{
    string API_KEY    = "CfwQ4SZ6gM_t6dIy1bCLJylX";
    string API_SECRET = "f9XOPLacPCZJ1dvPzN8B6Et7nMEaPGeomMSHk8Cr2zD4NfCY";
    const char *nonce = "1537896765";

    string signature = BitmexSignature(API_SECRET, "GET", "/realtime", nonce);

    string sig_valid("53c81d62ab81dda46f1b6cfae5464d85126aab66dbf57c275274bae1b8466f45");

    cout << "Signature: " << signature 
         << ((signature.compare(sig_valid) == 0) ? " matched" : " unmatched")
         << endl;

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

    #if 0
        const char *public_topics[] = {
            "publicNotifications",
            "orderBookL2_25",
            "quoteBin1m",
            "trade",
            NULL
        };

        api.Subscribe(public_topics);           // subscribe to subjects/topics
    #else
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
        LOGFILE(LOG_ERROR, "exception caught - %s('%s')", type_name(ex).data(), ex.what()); 
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

    // test websocket signaure
    // test_bitmex_signature();

    // test websocket
    test_bitmex_websocket();

    return 0;
}
