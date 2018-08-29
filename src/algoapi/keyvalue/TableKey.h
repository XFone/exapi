#ifndef _ALGOAPI_TABLE_KEY_H_
#define _ALGOAPI_TABLE_KEY_H_

#ifdef __cplusplus
namespace AlgoApi {
    namespace KeyValue {
#endif

    /// <summary>
    /// SFDB key-prefix for caching in Redis
    /// <refer src='algoapi/keyvalue/TableKey.cs'/>
    /// Key format: "SFDB:{date}:{fund_id}:{table_key}"
    /// </summary>
    typedef enum _TableKey
    {
        // 现货
        ID_SFDB_SPOT_ASSET          = 90010001,
        ID_SFDB_SPOT_POSITION       = 90010002,
        ID_SFDB_SPOT_ORDER          = 90010003,
        ID_SFDB_SPOT_KNOCK          = 90010004,
        ID_SFDB_SPOT_INNER_POSITION = 90010005,
        ID_SFDB_SPOT_ORDER_QUEUE    = 90010013,
        ID_SFDB_SPOT_KNOCK_QUEUE    = 90010014,
        // 期货
        ID_SFDB_FUTURE_ASSET        = 90020001,
        ID_SFDB_FUTURE_POSITION     = 90020002,
        ID_SFDB_FUTURE_ORDER        = 90020003,
        ID_SFDB_FUTURE_KNOCK        = 90020004,
        // 期权
        ID_SFDB_OPTION_ASSET        = 90030001,
        ID_SFDB_OPTION_POSITION     = 90030002,
        ID_SFDB_OPTION_ORDER        = 90030003,
        ID_SFDB_OPTION_KNOCK        = 90030004,
        ID_SFDB_OPTION_ORDER_QUEUE  = 90030013,
        ID_SFDB_OPTION_KNOCK_QUEUE  = 90030014
    } TableKey;

#ifdef __cplusplus
    } // AlgoApi::KeyValue
} // AlgoApi
#endif

#endif /* _ALGOAPI_TABLE_KEY_H_ */
