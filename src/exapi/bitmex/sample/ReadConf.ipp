/*
 * $Id: $
 *
 * Read config file with api-key and secrect key
 *
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */

#include "Base.h"
#include "Log.h"
#include "ConConf.h"
#include "osdeps/osutil.h"

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

string my_apikey;
string my_secret;
string my_proxy;        // socks5 proxy

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
                cout << "Read ApiKey: " << my_apikey << endl;
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
                        cout << "Read Secret: ('" << secret
                             << "') " << my_secret.substr(0, 2) << "..." << endl;
                    }
                } else {
                    my_secret = secret;
                    cout << "Read Secret: " << my_secret.substr(0, 2) << "..." << endl;
                }
                return;
            }
            if (!strcmp(key, "Proxy")) {
                my_proxy = vals[0];
                cout << "Read Proxy:  " << my_proxy << endl;
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