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
#include "ConConf.h"
#include "Log.h"
#include "Trace.h"
#include "osdeps/PidFile.h"
#include "osdeps/osutil.h"

#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>
#include <streambuf>

#include "JsonUtils.h"

using namespace std;
string my_api_key;
string my_secret_key;

/** 
 * Read command line options
 */
int checkopt(int opt, char *parg)
{
    int result = 0;
    ifstream infile;

    switch (opt) {
    case 'c':
        infile.open(parg);
        if (infile.is_open()) {
            stringstream buffer;
            buffer << infile.rdbuf();
            string jsonstr = buffer.str();
            infile.close();
            my_api_key    = JsonUtils::GetItem(jsonstr, "apiKey");
            my_secret_key = JsonUtils::GetItem(jsonstr, "secretKey");
        }
        // read_config_file(parg, on_read_keyval);
        break;

    case 'd':
        fork_daemon();
        break;

    case 'p':
        // TODO: change default port (DEFAULT_PORT_PB)
        break;

    case 'q':
        // qd_file_name = parg;
        break;

    default:
        result = -1;
        break;
    }

    return result;
}

#include "okcoinwsapi.h"

void test_okex_websocket()
{
    OKCoinWebSocketApiCom api(my_secret_key, my_secret_key);   //国际站

    try {
        std:string input;
        api.Run();                              // 启动连接服务器线程

        sleep(3);

        LOGFILE(LOG_DEBUG, "subscribe usd_btc ticker...");
        api.ok_spotusd_btc_ticker();            // 订阅行情

        std::cout << "Press enter to exit..." << endl;
        std::getline(std::cin, input);

        LOGFILE(LOG_DEBUG, "unsubscribe usd_btc ticker...");
        api.remove_ok_spotusd_btc_ticker();     // 取消订阅
    } catch (const std::exception &e) {
        LOGFILE(LOG_ERROR, "exception %s", e.what());
    } catch (...) {
        LOGFILE(LOG_ALERT, "unknown exception");
    }
}

#if !(_WIN32 || _WIN64)
#include <signal.h>
static void sigsegv_handler(int sig, siginfo_t *si, void *ctx)
{
    fprintf(stderr, "\n----- Exiting on Segfault -----\n");
    abort();
}
#endif

int main(int argc, char *argv[])
{
    int res;

#if !(_WIN32 || _WIN64)
    // catch segfault and print backtrace to stderr
    catch_segfault(sigsegv_handler);
#endif

#ifdef _DEBUG
    set_log_priority_name("LOG_DEBUG");
    set_trace_level(7, false); // do not print time
#endif

    if ((res = parse_args(argc, argv, "c:dp:q:", checkopt)) < 0 ||
        optind != argc) {
        print_usage(argv[0], "<name>",
            "  -c <Config File>           Specify config file\n"
            "  -d                         Run as a daemon in background\n"
            "  -p <Port>                  Listen port (default 8100)\n" // DEFAULT_PORT_QD
            "  -q <Quotation File>        Specify file to save quotation data\n"
        );
        return -1;
    }

    cout << "Api-key: " << my_api_key << endl;

    test_okex_websocket();

    return 0;
}