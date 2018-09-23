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

#include "JsonUtils.h"

#ifdef CHECK_REGEX_VERSION
# include <regex>
# if __cplusplus >= 201103L &&                            \
    (!defined(__GLIBCXX__) || (__cplusplus >= 201402L) || \
        (defined(_GLIBCXX_REGEX_DFS_QUANTIFIERS_LIMIT) || \
         defined(_GLIBCXX_REGEX_STATE_LIMIT)           || \
             (defined(_GLIBCXX_RELEASE)                && \
             _GLIBCXX_RELEASE > 4)))
#  define HAVE_WORKING_REGEX 1
# else
#  define HAVE_WORKING_REGEX 0
#  warning "No working std::regex! please upgrade to gcc-4.9+ or use boost::regex"
# endif
#endif

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
    MyDAQuoteBitmexSpi spi;

    const char *slist[] = { "https://www.bitmex.com", "\0" };
    api->ConnServer(slist, &spi);
    api->Init();

    //------ Quote Data ------
    //api->QueryOrderBookLevel2("XBT");

    QueryFilterParams params = {
        .symbol     = "", //"ETCBTC",
        .filter     = nullptr,
        .columns    = nullptr, 
        .count      = 100,
        .start      = 0,
        .reverse    = false,
        .startTime  = JsonUtils::from_datetime("2018-08-01T12:00:00"), // BUG in gcc, strptime not handle %Z
        .endTime    = JsonUtils::from_datetime("2018-08-01T12:30:00")  // BUG in gcc, strptime not handle %Z
    };

    //api->QueryQuotes(params);                      // got 403 Forbidden
    //api->QueryQuotesBucketed("1m", false, params); // got 403 Forbidden

    //------- Funding --------
    api->QueryFundingHistory(params);
    //api->QueryInsuranceHistory(params);

    //------- Trades Data ----
    //api->QueryTrades(params);
    //api->QueryTradesBucketed("1m", false, params);

    //------ Extra Data ------
    //api->GetAnnouncement();
    //api->GetAnnouncementUrgent();
    //api->GetSchema();
    //api->GetSchemaWebsocket();
    //api->GetStats();
    //api->GetStatsHistory();
    //api->GetStatsHistoryUsd();
    
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

    printf("GCC is %d.%d\n", __GNUC__, __GNUC_MINOR__);

    // test async mode
    test_bitmex_quote_spi();

    // test websocket
    // test_bitmex_websocket();
    
    return 0;
}
