
/*
 * $Id: $
 * 
 * WebSocket class declaration
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file wsshelp.h Helper classes for websocket handling.
 */

#include <iostream>

namespace exapi {
    
    /** Inherited from restbed::WebSocket */
    class WebSocketClient;

    typedef void (*ws_callback_open)();
    typedef void (*ws_callback_close)();
    typedef void (*ws_callback_message)(const char *pmsg);

    /* TO REMOVE */
    enum CONNECTION_STATE {
        CONNECTION_STATE_UNKONWN,
        CONNECTION_STATE_CONNECTING,
        CONNECTION_STATE_DISCONNECT,
    };

} // namespace exapi