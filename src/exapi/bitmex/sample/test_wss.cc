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
#include "osdeps/osutil.h"

#include <cstdio>
#include <system_error>

#include "ReadConf.ipp"             // read apiKey from config file
#include "BitmexWsApi.h"

using namespace exapi;
using namespace std;

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

void test_bitmex_websocket_public()
{
    string real_secret = my_secret;
    // prompt for password
    
    BitmexWsApi api(my_apikey, real_secret);

    try {
        // using testnet
        api.SetUrl(BITMEX_WSS_TESTNET BITMEX_WSS_PATH);

        api.set_message_callback(on_message);

        api.start();                            // start work thread

        sleep(1);

        const char *public_topics[] = {
            "announcement",
            "publicNotifications",
            "orderBook10",
            "quote",
            "trade",
            // "connected", "chat", "instrument", "settlement",
            // "funding", "insurance", "liquidation", 
            // "orderBookL2", "orderBookL2_25", "orderBook10", 
            // "quote", "trade", 
            // "quoteBin1m", "quoteBin5m", "quoteBin1h", "quoteBin1d", 
            // "tradeBin1m", "tradeBin5m", "tradeBin1h", "tradeBin1d"
            NULL
        };

        api.Subscribe(public_topics);           // subscribe to subjects/topics

        sleep(30);

        api.stop();

    } catch (exception &ex) {
        LOGFILE(LOG_ERROR, "exception caught - %s('%s')", type_name(ex).data(), ex.what()); 
    }
}

void test_bitmex_websocket_private()
{
    string real_secret = my_secret;
    // prompt for password
    
    BitmexWsApi api(my_apikey, real_secret);

    try {
        // using testnet
        api.SetUrl(BITMEX_WSS_TESTNET BITMEX_WSS_PATH);

        api.set_message_callback(on_message);

        api.start();                            // start work thread

        sleep(1);

        api.Authentication();
        sleep(3);

        const char *private_topics[] = {
            "privateNotifications", 
            "account", 
            "wallet", 
            "affiliate", 
            "margin", 
            "position", 
            "transact", 
            "order", 
            "execution",
            NULL
        };

        api.Subscribe(private_topics); 

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
    test_bitmex_websocket_public();
    //test_bitmex_websocket_private();

    return 0;
}
