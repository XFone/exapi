/* $Id: $
 * 
 * Json utility class implementation
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */

#include <cstdlib>
#include <cassert>
#include <cerrno>
#include <memory>
#include <sstream>
#include <chrono>
#include <iomanip>

#define  _JSON_DECODER_
#include "JsonUtils.h"
#include "cJSON.h"

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
int JsonUtils::Json::get(const char *name, const char * &v)
{
    JsonImpl *impl = (JsonImpl *)(cJSON_GetObjectItem(m_pointer, name));

    if (cJSON_IsString(impl)) {
        v = impl->valuestring;
        return 0;
    }

    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(const char *name, char * &v)
{
    JsonImpl *impl = (JsonImpl *)(cJSON_GetObjectItem(m_pointer, name));

    if (cJSON_IsString(impl)) {
        v = impl->valuestring;
        return 0;
    }

    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(const char *name, const char ** &v)  // array of string [ "", ... ]
{
    JsonImpl *impl = (JsonImpl *)(cJSON_GetObjectItem(m_pointer, name));

    if (cJSON_IsArray(impl)) {
        cJSON *child = impl->child;
        while (child != NULL) {
            if (!cJSON_IsString(child)) {
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
int JsonUtils::Json::get(const char *name, void * &v)
{
    JsonImpl *impl = (JsonImpl *)(cJSON_GetObjectItem(m_pointer, name));

    if (cJSON_IsRaw(impl)) {
        v = impl->valuestring;
        return 0;
    }
    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(const char *name, bool &v)
{
    JsonImpl *impl = (JsonImpl *)(cJSON_GetObjectItem(m_pointer, name));

    if (cJSON_IsBool(impl)) {
        v = (impl->type == cJSON_True);
        return 0;
    }
    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(const char *name, double &v)
{
    JsonImpl *impl = (JsonImpl *)(cJSON_GetObjectItem(m_pointer, name));

    if (cJSON_IsNumber(impl)) {
        v = impl->valuedouble;
        return 0;
    }
    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(const char *name, double * &v)  // array of double [ 1.0, ... ]
{
    JsonImpl *impl = (JsonImpl *)(cJSON_GetObjectItem(m_pointer, name));

    if (cJSON_IsArray(impl)) {
        cJSON *child = impl->child;
        while (child != NULL) {
            if (!cJSON_IsNumber(child)) {
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
int JsonUtils::Json::get(const char *name, long &v)
{
    JsonImpl *impl = (JsonImpl *)(cJSON_GetObjectItem(m_pointer, name));

    if (cJSON_IsNumber(impl)) {
        v = (long)impl->valuedouble;
        return 0;
    }

    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(const char *name, long * &v)  // array of long [ 1, ... ]
{
    JsonImpl *impl = (JsonImpl *)(cJSON_GetObjectItem(m_pointer, name));

    if (cJSON_IsArray(impl)) {
        cJSON *child = impl->child;
        while (child != NULL) {
            if (!cJSON_IsNumber(child)) {
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
int JsonUtils::Json::get(const char *name, unsigned long &v)
{
    JsonImpl *impl = (JsonImpl *)(cJSON_GetObjectItem(m_pointer, name));

    if (cJSON_IsNumber(impl)) {
        v = (unsigned long)impl->valuedouble;
        return 0;
    }

    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(const char *name, unsigned long * &v)  // array of unsigned long [ 1, ... ]
{
    JsonImpl *impl = (JsonImpl *)(cJSON_GetObjectItem(m_pointer, name));

    if (cJSON_IsArray(impl)) {
        cJSON *child = impl->child;
        while (child != NULL) {
            if (!cJSON_IsNumber(child)) {
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
int JsonUtils::Json::get(const char *name, int &v)
{
    JsonImpl *impl = (JsonImpl *)(cJSON_GetObjectItem(m_pointer, name));

    if (cJSON_IsNumber(impl)) {
        v = (int)impl->valuedouble;
        return 0;
    }

    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(const char *name, long long &v)
{
    JsonImpl *impl = (JsonImpl *)(cJSON_GetObjectItem(m_pointer, name));

    if (cJSON_IsNumber(impl)) {
        v = (long long)impl->valuedouble;
        return 0;
    }

    return -EBADMSG;
}

template<>
int JsonUtils::Json::get(const char *name, json_t &v)
{
    // TODO
    JsonImpl *impl = (JsonImpl *)(cJSON_GetObjectItem(m_pointer, name));

    if (cJSON_IsString(impl)) {
        // v->string = impl->valuestring; // const char *
        return 0;
    }

    return -EBADMSG;
}

template<>
int JsonUtils::Json::get_datetime(const char *name, long &time)
{
    JsonImpl *impl = (JsonImpl *)(cJSON_GetObjectItem(m_pointer, name));

    if (cJSON_IsString(impl)) { // "2000-01-23T04:56:07Z"
        time = from_datetime(impl->valuestring); // time_t
        return 0;
    }

    return -EBADMSG;
}

template<>
int JsonUtils::Json::get_datetime(const char *name, long long &timestamp)
{
    JsonImpl *impl = (JsonImpl *)(cJSON_GetObjectItem(m_pointer, name));

    if (cJSON_IsString(impl)) { // "2000-01-23T04:56:07.126Z"
        timestamp = from_timestamp(impl->valuestring); // timestamp_t
        return 0;
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

time_t
JsonUtils::from_datetime(const std::string &dtstr)
{
    struct std::tm tm;
#if __GNUC__ < 5
    strptime(dtstr.c_str(), "%FT%T%Z", &tm);
#else
    std::istringstream ss(dtstr);
    ss >> std::get_time(&tm, "%FT%T%Z");
#endif
    return mktime(&tm);
}

int64_t
JsonUtils::from_timestamp(const std::string &tsstr)
{
    // NOT implemented
    return 0L;
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
    // ISO 8601 format (e.g. '2012-04-23T18:25:43Z')
    std::ostringstream ss;
#if __GNUC__ < 5
    char buf[sizeof "2012-04-23T18:25:43Z"];
    strftime(buf, sizeof(buf), "%FT%T%Z", gmtime(&datetime));
    ss << buf;
#else
    ss << std::put_time(gmtime(&datetime), "%FT%T%Z");
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
    snprintf(buf, sizeof(buf), "%02d%02d%02d.%03dZ", ptm->tm_hour, ptm->tm_min, 
             ptm->tm_sec, (int)(ms % 1000L));

    return std::string(buf);
}

template <>
std::string JsonUtils::to_json(const cJSON *json)
{
    std::string str(cJSON_Print(json));
    return str;
}

template <>
std::string JsonUtils::to_json(const json_t json)
{
    std::string str(json->string);
    return str;
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

//------------------------ for json_decoder ----------------------------------

void decode_json_tree(const decoder_map_t &jmap, JsonUtils::JsonImpl *root, void *object)
{
    cJSON *p = reinterpret_cast<cJSON *>(root)->child;
    const char *pch;

    while (p != NULL) {
        if ((pch = p->string) != NULL) {
            auto it = jmap.find(pch);
            if (it != jmap.end()) {
                (*it->second)((JsonUtils::JsonImpl *)p, object);
            }
        }
        p = p->next;
    }
}