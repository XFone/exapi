/*
 * $Id: $
 * 
 * UrlUtils helper class for URL processing
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file UrlUtils.h Url helper class
 */

#include "Base.h"
#include "Trace.h"

#include <string>
#include <boost/regex.hpp>

namespace exapi {

    /**
     * Implement UrlUtils
     */
    class UrlUtils {
    public:
        static void 
        parse_uri(const std::string &url, 
                  std::string &host, std::string &port, std::string &path)
        {
            boost::smatch match;
            static const boost::regex pattern("^(.*:)//([A-Za-z0-9\\-\\.]+)(:[0-9]+)?(.*)$");
            if (regex_search(url, match, pattern)) {
                std::string proto = match[1];    // we always use https or wss
                std::string _port = match[3];
                host = match[2];
                if (_port.empty()) {
                    bool is_ssl = (!proto.compare("https:") || !proto.compare("wss:"));
                    port = is_ssl ? "443" : "80";
                } else {
                    port = _port.substr(1);
                }
                if (match.size() >= 5) {
                    path = match[4];
                }
                TRACE(7, "host: '%s' port: '%s' path: '%s'", 
                      host.c_str(), port.c_str(), path.c_str());
            } else {
                assert(0);
            }
        }

        static void 
        parse_uri(const std::string &url, 
                  std::string &host, std::string &port)
        {
            std::string path;
            parse_uri(url, host, port, path);
        }

    };   // UrlUtils

} // namespace exapi