/* $Id: $
 * 
 * Http utility class implementation
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */

#include <cstdlib>
#include <cassert>
#include <cerrno>
#include <string>
#include <sstream>

#include "HttpRestClient.h"
using namespace exapi;

//------------------------ JsonUtils::JsonImpl ----------------------------

template<>
std::string HttpRestClient::to_string(HTTP_METHOD &method)
{
    const char *str = "";

    switch (method) {
    case METHOD_GET:
        str = "GET";
        break;
    case METHOD_POST:
        str = "POST";
        break;
    case METHOD_PUT:
        str = "PUT";
        break;
    case METHOD_DELETE:
        str = "DELETE";
        break;
    default:
        // LOGFILE(LOG_ALERT, "Unsupported HTTP_METHOD %d", method);
        assert(0);
        break;
    }

    return std::string(str);
}