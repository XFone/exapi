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

#include "ReadConf.ipp"             // read apiKey from config file
#include "JsonUtils.h"              // for JsonUtils::from_datetime

#include "BitmexApi.h"              // for BITMEX_REST_TESTNET
#include "MyQuoteBitmexSpi.h"

/*---------------------------- async mode -----------------------------------*/

void test_bitmex_quote_spi()
{
    DAQuoteBitmexApi *api = DAQuoteBitmexApi::CreateApi(my_apikey.c_str(), my_secret.c_str());
    MyDAQuoteBitmexSpi spi;

    try {
        const char *slist[] = {
            BITMEX_REST_TESTNET,    // https
            my_proxy.c_str(),       // socks
            nullptr                 // end of list
        };
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

    } catch (exception &ex) {
        LOGFILE(LOG_ERROR, "exception caught - %s('%s')", type_name(ex).data(), ex.what()); 
    }
}

int main(int argc, char *argv[])
{
    int res;

#if !(_WIN32 || _WIN64)
    // catch segfault and print backtrace to stderr
    catch_segfault_cpp(nullptr);
#endif

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
