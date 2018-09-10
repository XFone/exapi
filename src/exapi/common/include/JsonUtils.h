/*
 * $Id: $
 * 
 * Json utility class declaration
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file JsonUtils.h implements tools for processing json.
 */

#include <string>

/**
 * JsonUtils has methods for processing json string
 */
class JsonUtils {
public:

    static std::string GetItem(const std::string &jsonstr, const char *name);

    static double GetItemDouble(const std::string &jsonstr, const char *name);

    static int64_t GetItemInt64(const std::string &jsonstr, const char *name) {
        return (int64_t)GetItemDouble(jsonstr, name);
    }

    static uint64_t GetItemUint64(const std::string &jsonstr, const char *name) {
        return (uint64_t)GetItemDouble(jsonstr, name);
    }

    static std::string GetItemString(const std::string &jsonstr, const char *name) {
        return GetItem(jsonstr, name);
    }

    static int to_hexstring(char *out, const char *buf, size_t size);
};