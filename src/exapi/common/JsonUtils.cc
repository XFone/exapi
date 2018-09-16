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
#include <cassert>
#include <cerrno>
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

unsigned JsonUtils::Json::type() const
{
    return (m_pointer != nullptr) ? m_pointer->type : cJSON_Invalid;
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

template<>
int JsonUtils::Json::get(const char * &v)
{
    if (m_pointer->type == cJSON_String) {
        v = m_pointer->valuestring;
        return 0;
    }
    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(const char ** &v)  // array of string [ "", ... ]
{
    if (m_pointer->type == cJSON_Array) {
        cJSON *child = m_pointer->child;
        while (child != NULL) {
            if (child->type != cJSON_String || child->type != cJSON_Raw) {
                // TODO: show warning
            }
            *v++  = child->valuestring;
            child = child->next;
        }
        return 0;
    }
    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(const void * &v)
{
    if (m_pointer->type == cJSON_String || m_pointer->type == cJSON_Raw) {
        v = m_pointer->valuestring;
        return 0;
    }
    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(bool &v)
{
    if (m_pointer->type == cJSON_False) {
        v = false;
        return 0;
    } else if (m_pointer->type == cJSON_True) {
        v = true;
        return 0;
    }
    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(double &v)
{
    if (m_pointer->type == cJSON_Number) {
        v = m_pointer->valuedouble;
        return 0;
    }
    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(double * &v)  // array of double [ 1.0, ... ]
{
    if (m_pointer->type == cJSON_Array) {
        cJSON *child = m_pointer->child;
        while (child != NULL) {
            if (child->type != cJSON_Number) {
                // TODO: show warning
            }
            *v++  = child->valuedouble;
            child = child->next;
        }
        return 0;
    }
    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(long &v)
{
    if (m_pointer->type == cJSON_Number) {
        v = (long)m_pointer->valuedouble;
        return 0;
    }
    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(long * &v)  // array of long [ 1, ... ]
{
    if (m_pointer->type == cJSON_Array) {
        cJSON *child = m_pointer->child;
        while (child != NULL) {
            if (child->type != cJSON_Number) {
                // TODO: show warning
            }
            *v++  = (long)child->valuedouble;
            child = child->next;
        }
        return 0;
    }
    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(unsigned long &v)
{
    if (m_pointer->type == cJSON_Number) {
        v = (unsigned long)m_pointer->valuedouble;
        return 0;
    }
    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(unsigned long * &v)  // array of unsigned long [ 1, ... ]
{
    if (m_pointer->type == cJSON_Array) {
        cJSON *child = m_pointer->child;
        while (child != NULL) {
            if (child->type != cJSON_Number) {
                // TODO: show warning
            }
            *v++  = (unsigned long)child->valuedouble;
            child = child->next;
        }
        return 0;
    }
    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(int &v)
{
    if (m_pointer->type == cJSON_Number) {
        v = (int32_t)m_pointer->valuedouble;
        return 0;
    }
    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(long long &v)
{
    if (m_pointer->type == cJSON_Number) {
        v = (int64_t)m_pointer->valuedouble;
        return 0;
    }
    return -EBADMSG;
}

template<>
int JsonUtils::Json::get_datetime(long &time)
{
    if (m_pointer->type == cJSON_String) {
        return time = m_pointer->valuedouble;
    }
    return -EBADMSG;
}

template<>
int JsonUtils::Json::get_datetime(long long &timestamp)
{
    if (m_pointer->type == cJSON_String) {
        return timestamp = m_pointer->valuedouble;
    }
    return -EBADMSG;
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

std::string JsonUtils::to_datetime(time_t datetime)
{
    // ISO 8601 format (e.g. '2012-04-23T18:25:43.511Z')
    std::ostringstream ss;
#if __GNUC__ < 5
    char buf[sizeof "2012-04-23T18:25:43Z"];
    strftime(buf, sizeof(buf), "%FT%TZ", gmtime(&datetime));
    ss << buf;
#else
    ss << std::put_time(gmtime(&datetime), "%FT%TZ");
#endif
    return ss.str();
}

std::string JsonUtils::to_datetime_ms(int64_t ms)
{
    // ISO 8601 format (e.g. '2012-04-23T18:25:43.511Z')
    char buf[sizeof "'2012-04-23T18:25:43.511Z'"];

    struct tm * ptm, stm = { 0 };
    time_t secs = ms / 1000L;
    ptm = localtime_r(&secs, &stm);
    snprintf(buf, sizeof(buf), "%02d%02d%02d.%03d", ptm->tm_hour, ptm->tm_min, 
             ptm->tm_sec, (int)(ms % 1000L));

    return std::string(buf);
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