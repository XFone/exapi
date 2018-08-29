/*
 * $Id: $
 *
 * DataFeed Interface
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file IDataFeedConsumer.h DataFeed Interface */
#include "Base.h"
#include "Common.h"

using namespace AlgoApi::DataSource;

_INTERFACE_ IDataFeedConsumer {
    /// <summary>
    /// Get a token to identify this instance for the local dispatcher
    /// </summary>
    /// <returns>a hash value</returns>
    UINT32 GetToken();

    /// <summary>
    /// Global UUID
    /// </summary>
    /// <returns>UUID (or ipv6 address) in 64 bits</returns>
    UINT64 GetGobalToken();

    /// <summary>
    /// Receive a dataset from data-feed broadcast server
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="dataset"></param>
    /// <returns></returns>
    TDispResult OnDataDispatch(Object sender, Object dataset);
};