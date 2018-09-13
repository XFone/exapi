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

#include <cstdlib>
#include <memory>
#include <sstream>
#include <chrono>
#include <iomanip>

//------------------------ JsonUtils::JsonImpl ----------------------------

class JsonUtils::JsonImpl : public cJSON {
public:
    static void *g_cjson_mpool;

    JsonImpl() {
        if (nullptr == g_cjson_mpool) {
            g_cjson_mpool = &g_cjson_mpool; // TODO: using TBB memory pool
            cJSON_Hooks hooks = {
                .malloc_fn = JsonUtils::FastAlloc,
                .free_fn   = JsonUtils::FastFree
            };
            cJSON_InitHooks(&hooks);
        }
    }

    ~JsonImpl() {}
};

void *JsonUtils::JsonImpl::g_cjson_mpool = nullptr;

//------------------------- JsonUtils::Json -------------------------------

JsonUtils::Json &JsonUtils::Json::parse(const std::string &jsonstr)
{
    cJSON *pjson = cJSON_Parse(jsonstr.c_str());
    if (nullptr != this->m_ctx_root) {
        cJSON_Delete(m_ctx_root);
    }
    m_ctx_root = reinterpret_cast<JsonImpl *>(pjson);
    m_pointer  = m_ctx_root;
    return *this;
}

JsonUtils::Json::~Json()
{
    if (nullptr != m_ctx_root) {
        cJSON_Delete(m_ctx_root);
    }
}

JsonUtils::Json &JsonUtils::Json::at(const char *name) throw(std::runtime_error)
{
    if (m_pointer == nullptr)
        m_pointer = m_ctx_root;
    assert(m_pointer != nullptr);
    cJSON *obj = cJSON_GetObjectItem(m_pointer, name);
    m_pointer = reinterpret_cast<JsonImpl *>(obj);

    if (m_pointer == nullptr) {
        throw std::runtime_error(
            std::string("json item ") + name + "not found"
        );
    }
    return *this;
}

template <>
bool JsonUtils::Json::get() throw(std::runtime_error)
{
    if (m_pointer->type == cJSON_False) {
        return false;
    } else if (m_pointer->type == cJSON_True) {
        return true;
    }
    throw std::runtime_error(
        std::string("json item ") + m_pointer->string + "is not boolean"
    );
}

template <>
double JsonUtils::Json::get() throw(std::runtime_error)
{
    if (m_pointer->type == cJSON_Number) {
        return m_pointer->valuedouble;
    }
    throw std::runtime_error(
        std::string("json item ") + m_pointer->string + "is not double"
    );
}

template <>
std::string JsonUtils::Json::get() throw(std::runtime_error)
{
    if (m_pointer->type == cJSON_String || m_pointer->type == cJSON_Raw) {
        return m_pointer->valuestring;
    }
    throw std::runtime_error(
        std::string("json item") + m_pointer->string + "is not string"
    );
}

//---------------------------- JsonUtils ----------------------------------

void *JsonUtils::FastAlloc(size_t size)
{
    // TODO: using TBB memory pool
    return std::malloc(size);
}

void JsonUtils::FastFree(void *ptr)
{
    // TODO: using TBB memory pool
    std::free(ptr);
}

double
JsonUtils::GetItemDouble(const std::string &jsonstr, const char *name) {
    double result = 0.0;
    if (!jsonstr.empty()) {
        Json json;
        result = json.parse(jsonstr).at(name).get<double>();
    }
    return result;
}

std::string
JsonUtils::GetItemString(const std::string &jsonstr, const char *name) {
    std::string result;
    if (!jsonstr.empty()) {
        Json json;
        result = json.parse(jsonstr).at(name).get<std::string>();
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

template <>
std::string JsonUtils::to_json(time_t datetime)
{
    // ISO 8601 format (e.g. '2012-04-23T18:25:43.511Z')
    std::ostringstream ss;
    ss << std::put_time(gmtime(&datetime), "%FT%TZ");
    return ss.str();
}

template <>
std::string JsonUtils::to_json(const char *values[])
{
    std::ostringstream ss;
    ss << "[ ";
    if (nullptr != values) {
        const char **pv = &values[0];
        const char *value;
        while ((value = *pv++) != nullptr) {
            ss << "\"" << value << "\",";
        }
    }
    ss.seekp(-1, std::ios_base::end);
    ss.put(']');

    return ss.str();
}

template <>
std::string JsonUtils::to_json(const void *object)
{
    // TODO: need serializing object
    const char *str = reinterpret_cast<const char *>(object);
    return std::string(str);
}

template <>
std::string JsonUtils::to_json(std::vector<std::string> &values)
{
    std::ostringstream ss;
    ss << "[ ";
    for (auto v : values) {
        ss << "\"" << v << "\",";
    }
    ss.seekp(-1, std::ios_base::end);
    ss.put(']');

    return ss.str();
}