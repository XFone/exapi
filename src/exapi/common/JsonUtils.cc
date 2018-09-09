/* $Id: $
 * 
 * Json utility class implementation
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */

#include "JsonUtils.h"
#include "cJSON.h"

std::string 
JsonUtils::GetItem(const std::string &jsonstr, const char *name)
{
    std::string result;
    if (!jsonstr.empty()) {
        cJSON *pjson = cJSON_Parse(jsonstr.c_str());
        if (nullptr != pjson) {
            auto obj = cJSON_GetObjectItem(pjson, name);
            if (nullptr != obj) result = obj->valuestring;
            cJSON_Delete(pjson);
        }
    }
    return result;
}

int 
JsonUtils::to_hexstring(char *out, const char *buf, size_t size)
{
    /* register */ int i;
    unsigned char val;
    /* register */ char *p = out;
    /* register */ const char *p_buf;
    const char *buf_end = buf + size;
    
    for (i = 0, p_buf = buf; p_buf < buf_end; i++, p_buf++) {
        val = 0x0F & (*p_buf >> 4);
        *p++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        val = 0x0F & *p_buf;
        *p++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
    }
    return (p - out);
}