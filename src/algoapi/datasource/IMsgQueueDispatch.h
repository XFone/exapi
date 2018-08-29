/*
 * $Id: $
 *
 * Abstract MQ (rabbitMQ or 0MQ) dispatcher Interface
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file IMsgQueueDispatch.h MQ dispatcher Interface */
#include "Base.h"
#include "Common.h"
#include <string>

using namespace AlgoApi::DataSource;

_INTERFACE_ IMsgQueueDispatch {
    /// <summary>
    /// Get a token to identify this instance for the local dispatcher
    /// </summary>
    /// <returns>a hash value</returns>
    UINT32 GetToken();

    /// <summary>
    /// Get MQ's customer-id which is assigned by message queue server
    /// </summary>
    /// <returns></returns>
    std::string GetCustomerId();

    /// <summary>
    /// Receive a new message from mq
    /// </summary>
    /// <param name="sender">the mq server or proxy</param>
    /// <param name="message">message object</param>
    /// <returns>Dispatch result</returns>
    TDispResult OnMessageDispatch(Object sender, Object message);

    /// <summary>
    /// Receive a command response from mq
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="response"></param>
    /// <returns>Dispatch result, only DR_SUCCESS, DR_FAILED, DR_DROPPED is allowed <see cref="TDispResult"/>
    /// </returns>
    TDispResult OnCommandResponse(Object sender, Object response);
};
