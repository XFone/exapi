/*
 * $Id: $
 *
 * Common types for AlgoApi::DataSource
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */

#ifndef _ALGOAPI_DATASOURCE_COMMON_H_
#define _ALGOAPI_DATASOURCE_COMMON_H_

#ifdef __cplusplus
namespace AlgoApi {
    namespace DataSource {
#endif

    /**
     * Dispatch result for processing by dispatcher
     */
    typedef enum _DispResult {
        DR_SUCCESS = 0, /// process success    - resources should be released (by dispatcher)
        DR_FAILED,      /// process failed     - resources should be released (by dispatcher)
        DR_IGNORED,     /// message is ignored - resources should be released (by dispatcher)
        DR_DROPPED,     /// message is dropped
        DR_CONTINUE,    /// message is shared to next consumer
        DR_MAX_INVALID
    } TDispResult;

    /**
     * Wrapper Object in CSharp
     */
    typedef void *Object;

    /**
     * Shared object use smart_pointer
     */
    typedef void *SharedObject;

#ifdef __cplusplus
    } // AlgoApi::DataSource
} // AlgoApi
#endif

#endif /* _ALGOAPI_DATASOURCE_COMMON_H_ */

