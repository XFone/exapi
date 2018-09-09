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

    static int to_hexstring(char *out, const char *buf, size_t size);
};