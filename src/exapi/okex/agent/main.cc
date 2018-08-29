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

int main(int argc, char *argv[])
{
    int res;

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

    OKCoinApiCom api("", "");

    std::string symbol = "eth_btc";
    std::string json = api.GetTicker(symbol);
    
    return 0;
}
