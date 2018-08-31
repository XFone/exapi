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
    
    return 0;
}


wss_test() {
	std::string com_apiKey		= "";									//请到www.okcoin.com申请。
	std::string com_secretKey	= "";									//请到www.okcoin.com申请。
	comapi = new OKCoinWebSocketApiCom(com_apiKey,com_secretKey);		//国际站
	comapi->SetCallBackOpen(com_callbak_open);
	comapi->SetCallBackClose(com_callbak_close);
	comapi->SetCallBackMessage(com_callbak_message);
	comapi->Run();//启动连接服务器线程

	cout << "输入1订阅比特币当周合约行情，输入2订阅比特币期货指数，输入3取消订阅，输入4关闭连接，请输入：";
	string i;
	cin >> i;
	if(i == "1")
	{
		comapi->ok_futureusd_btc_ticker_this_week();
	}
	
	if(i == "2")
	{
		comapi->ok_futureusd_btc_ticker_this_week();
	}

	//对于已经注册的数据在不需要时注销掉，不再接收，
	//注册太多非必要数据，会使您的程序整体性能下降，请开发者注意处理。
	if(i == "3")
	{
		if(comapi != 0)
		{
			comapi->remove_ok_spotusd_btc_ticker();
		}
		system("pause");
	}

	//关闭连接
	//cnapi->Close();	
	comapi->Close();

	system("pause");

	//释放API实例
	//delete cnapi;
	delete comapi;
	cout << "释放API实例完成。";

}
