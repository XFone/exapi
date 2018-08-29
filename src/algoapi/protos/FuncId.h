/*
 * $Id: $
 *
 * FuncId (Swordfish) definitions for AlgoApi
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#ifndef _ALGOAPI_FUNC_ID_H_
#define _ALGOAPI_FUNC_ID_H_

#define INVALID_FUNCID                -255

#define RETCODE_SUCCESS               0
#define RETCODE_FAIL                  -1 

#ifdef __cplusplus
namespace AlgoApi {
    namespace Protos {
#endif

    /**
     * All FuncIds (message id) in AutoTrade name domain
     * <refer src='algoapi/protos/FuncId.cs'/>
     * For global FuncId definitions, please refer to ../../doc/funid.py
     */
    typedef enum _FuncId
    {
        // -------------------------------------------------------------------
        // Used by ATS internally
        // -------------------------------------------------------------------
        ID_ATS_INTERNAL_TIMER       = -200,
        ID_ATS_INTERNAL_TIMER_1     = -199,
        ID_ATS_INTERNAL_TIMER_2     = -198,
        ID_ATS_INTERNAL_TIMER_3     = -197,
        ID_ATS_INTERNAL_TIMER_4     = -196,
        ID_ATS_INTERNAL_TIMER_5     = -195,
        ID_ATS_INTERNAL_TIMER_6     = -194,
        ID_ATS_INTERNAL_TIMER_7     = -193,
        ID_ATS_INTERNAL_TIMER_8     = -192,
        ID_ATS_INTERNAL_TIMER_9     = -191,
        ID_ATS_INTERNAL_TIMER_A     = -190,
        ID_ATS_INTERNAL_TIMER_B     = -189,
        ID_ATS_INTERNAL_TIMER_C     = -188,
        ID_ATS_INTERNAL_TIMER_D     = -187,
        ID_ATS_INTERNAL_TIMER_E     = -186,
        ID_ATS_INTERNAL_TIMER_F     = -185,
        ID_ATS_INTERNAL             = -100,
 
        // -------------------------------------------------------------------
        // QErrorProtos.cs (refer to ./autotrade/qerr_protocol.proto)
        // -------------------------------------------------------------------
        ID_REQUEST_OK               =  1,
        ID_REQUEST_ERR              = -1,

        // -------------------------------------------------------------------
        // QStockProtos.cs (refer to ./autotrade/qstock_protocol.proto), 
        // -------------------------------------------------------------------
        // 登录
        ID_S_ACCOUNT_LOGIN_REQT     = 11000001,
        ID_S_ACCOUNT_LOGIN_RESP     = 11100001,
        ID_S_ACCOUNT_LOGOUT_REQT    = 11000002,
        ID_S_ACCOUNT_LOGOUT_RESP    = 11100002,
        ID_S_ACCOUNT_Q_LOGIN_REQT   = 11000003,
        ID_S_ACCOUNT_Q_LOGIN_RESP   = 11100003,
        ID_S_ACCOUNT_RELOGIN_REQT   = 11000004,
        ID_S_ACCOUNT_RELOGIN_RESP   = 11100004,
        // 单笔委托
        ID_S_ORDER_REQT             = 11001001,
        ID_S_ORDER_RESP             = 11101001,
        // 批量委托
        ID_S_ORDERS_REQT            = 11001002,
        ID_S_ORDERS_RESP            = 11101002,
        ID_S_ORDERS_RECEIVED        = 11101010,   // 表明系统接收指令
        ID_S_ORDERS_SUCCESSED       = 11101011,   // 报单报到柜台
        ID_S_ORDERS_FINISHED        = 11101012,   // 报单完成成交（或者撤单）
        ID_S_ORDERS_FAILED          = 11101013,   // 报单报到柜台
        ID_S_ORDERS_CREATE_RESP     = 11101014,   // 报到StockSvr并返回正确or错误信息
        // 申购赎回
        ID_S_CREATE_REDEEM_REQT     = 11001003,
        ID_S_CREATE_REDEEM_RESP     = 11101003,
        // 单笔撤单
        ID_S_DELETE_REQT            = 11001004,
        ID_S_DELETE_RESP            = 11101004,
        // 批量撤单
        ID_S_DELETES_REQT           = 11001005,
        ID_S_DELETES_RESP           = 11101005,
        // 单笔委托查询
        ID_S_Q_ORDER_REQT           = 11002002,
        ID_S_Q_ORDER_RESP           = 11102002,
        // 批量委托查询
        ID_S_Q_ORDERS_REQT          = 11002003,
        ID_S_Q_ORDERS_RESP          = 11102003,
        // ETF
        ID_S_Q_ETF_BASIC_INFO_REQT  = 11002008,
        ID_S_Q_ETF_BASIC_INFO_RESP  = 11102008,
        ID_S_Q_ETF_BASKET_LIST_REQT = 11002009,
        ID_S_Q_ETF_BASKET_LIST_RESP = 11102009,
        // 资金查询
        ID_S_Q_CAPITAL_REQT         = 11002010,
        ID_S_Q_CAPITAL_RESP         = 11102010,
        // 持仓查询
        ID_S_Q_POSITION_REQT        = 11002006,
        ID_S_Q_POSITION_RESP        = 11102006,
        // 成交明细查询
        ID_S_Q_DEAL_REQT            = 11002004,
        ID_S_Q_DEAL_RESP            = 11102004,
        // 清算流水查询
        ID_S_Q_CLEARING_FLOW_REQT   = 11002005,
        ID_S_Q_CLEARING_FLOW_RESP   = 11102005,
        // ETF动态信息查询-可申购数量
        ID_S_Q_ETF_BASKET_DYNAMIC_REQT = 11002011,
        ID_S_Q_ETF_BASKET_DYNAMIC_RESP = 11102011,
        // 成交回报
        ID_S_K_RAW_STOCK_KNOCK      = 11003001,   // 原始
        ID_S_K_ONE_STOCK_KNOCK      = 11003002,   // 单条
        ID_S_K_SUM_STOCK_KNOCK      = 11003003,   // 汇总
        // 历史委托查询
        ID_S_Q_HISTORY_ORDER_REQT   = 11002091,
        ID_S_Q_HISTORY_ORDER_RESP   = 11102091,
        // 历史成交查询
        ID_S_Q_HISTORY_DEAL_REQT    = 11002092,
        ID_S_Q_HISTORY_DEAL_RESP    = 11102092,

        // -------------------------------------------------------------------
        // QPolicyProtos.cs (refer to ./autotrade/qpolicy_protocol.proto), 
        // -------------------------------------------------------------------
        ID_ALGOPOLICY_CREATE_REQT   = 91000001,
        ID_ALGOPOLICY_CREATE_RESP   = 91100001,
        ID_ALGOPOLICY_UPDATE_REQT   = 91000002,
        ID_ALGOPOLICY_UPDATE_RESP   = 91100002,
        ID_ALGOPOLICY_DELETE_REQT   = 91000003,
        ID_ALGOPOLICY_DELETE_RESP   = 91100003,
        ID_ALGOPOLICY_QUERY_REQT    = 91000004,
        ID_ALGOPOLICY_QUERY_RESP    = 91100004,

        // -------------------------------------------------------------------
        // QuotationProtos.cs (refer to autotrade/quotation.proto)
        // -------------------------------------------------------------------
        // 周期行情数据
        ID_CIRCLE_QUOTATION         = 10000004
        
    } FuncId;

    /**
     * All OpCodes (operation code) in AutoTrade name domain
     * see algoapi/protos/qpolicy_protocol.proto: AlgoPolicyUpdateReqt.op_code
     */
    typedef enum _OpCode
    {
        OPC_CREATE = 0,
        OPC_STOP,
        OPC_SUSPEND,
        OPC_PARAMS,
        OPC_RESUME
    } OpCode;

#ifdef __cplusplus
    } // AlgoApi::Protos
} // AlgoApi
#endif

#endif /* _ALGOAPI_FUNCID_H_ */
