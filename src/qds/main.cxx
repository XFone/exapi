/*
 * $Id: $
 *
 * Main entry
 *
 * Copyright (c) 2014-2017 Zerone.IO (Shanghai). All rights reserved.
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

#include "worker/WorkerUtils.h"

#include "PcapHelper.h"

#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#if !(_WIN32 || _WIN64)
#include <signal.h>
#endif

//#include "GlobalDefine.h"
#define QDATA_VERSION            "$Version$"

#ifdef UNIT_TEST
#pragma message("UNIT_TEST macro is set") 
#endif

#ifdef STRESS_TEST
#pragma message("STRESS_TEST macro is set") 
#endif

using namespace std;

int g_is_exiting = FALSE;

/* referenced in deployment/AssemblyInfo.cxx */
vector<string> qds_address_cfgs; /**< Quotation server publish  address */
vector<string> qd_address_cfgs;  /**< Remote quotation subscribe address*/
vector<string> redis_addr_cfgs;  /**< Redis service address             */
string  qd_file_name;            /**< Quotation record file path        */
string  pcap_file_name;          /**< Pcap packets file path            */

/* Referenced by main.cxx::checkopt() */
#if !(_WIN32 || _WIN64)
void sigint_handler(int sig) {
    if (!g_is_exiting) {
        fprintf(stderr, "\n");
        LOGFILE(LOG_ALERT, "QDS was interrupted on signal #%d", sig);
        fprintf(stderr, "\n--- Gracefully stopping... (press Ctrl+C again to force) ---\n");
        g_is_exiting = TRUE;
    } else {
        fprintf(stderr, "\n----- Force Exiting! -----\n");
        exit(-EXIT_FAILURE);
    }
}

static void sigsegv_handler(int sig, siginfo_t *si, void *ctx)
{
    fprintf(stderr, "\n----- Exiting on Segfault -----\n");
    abort();
}
#endif /* ! (_WIN32||_WIN64) */

extern "C" void on_read_keyval(const char *key, const char *vals[])
{
    const int max_vals = 12;
    int max_key  = 16;

    // got a <key,value> pair
    TRACE_IF(7, {
        cout << "[" << key << "]";
        for (int i = 0; i < max_vals && vals[i]; i++) {
            cout << "\t(" << (i + 1) << ")" << vals[i];
        }
        cout << endl;
    });

    if (!strncmp("QD", key, max_key)) {
        qd_address_cfgs.push_back(vals[0]);
        // TODO vals[1] MQ_SUB/...
    }
    else if (!strncmp("QDS", key, max_key)) {
        qds_address_cfgs.push_back(vals[0]);
        // TODO vals[1] MQ_PUB/...
    }
    else if (!strncmp("DB", key, max_key)) {
        if (!strncasecmp("REDIS", vals[1], max_key)) {
            redis_addr_cfgs.push_back(vals[0]);
        }
    }
}


/** 
 * Read command line options
 */
int checkopt(int opt, char *parg)
{
    int result = 0;
    switch (opt) {
    case 'c':
        LOGFILE(LOG_INFO, "loadling config file '%s'", parg);
        read_config_file(parg, on_read_keyval);
        break;

    case 'd':
        fork_daemon();
        break;

    case 'p':
        // TODO: change default port (DEFAULT_PORT_PB)
        break;

    case 'l':
        pcap_file_name = parg;
        break;

    case 'q':
        qd_file_name = parg;
        break;

    case 'x':
        save_pcap_file(parg, (pcap_handler)handler_qdata);
        g_is_exiting = TRUE; // exit after converting
        break;

    case 'L':
        // TODO
        break;
        
    default:
        result = -1;
        break;
    }

    return result;
}

int main(int argc, char *argv[])
{
    int res, pidfile;

    version_str = QDATA_VERSION; // defined in ConConf.h

#if !(_WIN32 || _WIN64)
    // catch segfault and print backtrace to stderr
    catch_segfault(sigsegv_handler);
    // catch program interrupt (control-C)
    signal(SIGINT, sigint_handler); 
    // catch software termination signal (kill)
    signal(SIGTERM, sigint_handler);
#endif

#ifdef _DEBUG
    set_log_priority_name("LOG_DEBUG");
    set_trace_level(7, false); // do not print time
#endif

    if ((res = parse_args(argc, argv, "c:dl:p:q:x:", checkopt)) < 0 ||
        optind != argc) {
        print_usage(argv[0], "<name>",
            "  -c <Config File>           Specify config file\n"
            "  -d                         Run as a daemon in background\n"
            "  -p <Port>                  Listen port (default 8100)\n" // DEFAULT_PORT_QD
            "  -l <Pcap File>             Specify file to load packets (zmq payload)\n"
            "  -q <Quotation File>        Specify file to save quotation data\n"
            "  -x <Quotation File>        Specify file to be converted into .pcap format\n"
        );
        return -1;
    }

    LOGFILE(LOG_INFO,  "Quotation server starting...");
    pidfile = create_pidfile("/var/run/qds.pid");


    ATS::Worker::BeginSchedule();

    // loop and waiting signals
    while (!g_is_exiting) {
        ATS::Worker::ReportHeartbeat();
    }

    release_pidfile(pidfile);

    LOGFILE(LOG_ALERT, "Quotation server exiting...");
    ATS::Worker::EndSchedule();
    
    LOGFILE(LOG_INFO, "Quotaion exited.");
 
    return res;
}
