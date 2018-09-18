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
#include <vector>
#include <stdexcept>

/**
 * Json string
 */
struct _json_t {
    char string[];
};

typedef _json_t *json_t;

/**
 * JsonUtils has methods for processing json string
 */
class JsonUtils {
public:
    class JsonImpl;

    /** 
     * Json is a wrapper and usage class of JsonImpl
     * <pre>
     * Json json;
     * double result = json.parse(jsonstr)
     *                     .at(name)
     *                     .at(subname)
     *                     .get<double>();
     * </pre>
     */
    class Json {
    private: 
        JsonImpl      *m_ctx_root;      ///< the root of json tree
        JsonImpl      *m_pointer;       ///< floating pointer to json leaf

        Json(JsonImpl *impl) : m_ctx_root(impl), m_pointer(nullptr) {}

    public:

        Json() : m_ctx_root(nullptr), m_pointer(nullptr) {}

        ~Json();

        Json & parse(const std::string &jsonstr);

        Json & root() { 
            m_pointer = m_ctx_root;
            return *this;
        }

        /**
         * Json item type: 0: invalid
         */
        unsigned type() const;

        Json & at(const char *name) throw(std::runtime_error);

        /**
         * Get value
         * support typename: bool, double, std::string
         */
        template<typename T> T get() throw(std::runtime_error);

        /**
         * Get value
         * support typename: all base type, and C struct defined in json2c
         * @return 0 for success, elsewise return -errno
         */
        template<typename T> int get(const char *name, T &v);

        /**
         * Get datetime value
         * support typename: time_t timestamp_t
         * @return 0 for success, elsewise return -errno
         */
        template<typename T> int get_datetime(const char *name, T &v);
    }; // Json

public:

    static void *FastAlloc(size_t size);

    static void FastFree(void *ptr);

public:

    //-------------------------- Json Decode ------------------------------

    static std::string GetItem(const std::string &jsonstr, const char *name) {
        return GetItemString(jsonstr, name);
    }

    static double GetItemDouble(const std::string &jsonstr, const char *name);

    static int64_t GetItemInt64(const std::string &jsonstr, const char *name) {
        return (int64_t)GetItemDouble(jsonstr, name);
    }

    static uint64_t GetItemUint64(const std::string &jsonstr, const char *name) {
        return (uint64_t)GetItemDouble(jsonstr, name);
    }

    static std::string GetItemString(const std::string &jsonstr, const char *name);

    //-------------------------- Json Encode ------------------------------

    /**
     * binary buffer to hexstring
     * @out output character buffer
     * @buf input binary buffer
     * @size size if input buffer
     * @return sizeof of output buffer in chars
     */
    static int to_hexstring(char *out, const char *buf, size_t size);

    /**
     * @param time seconds 
     * @return string in ISO 8601 format (e.g. '2012-04-23T18:25:43Z')
     */
    static std::string to_datetime(time_t time);

    /**
     * @param ms milli-seconds 
     * @return string in ISO 8601 format (e.g. '2012-04-23T18:25:43.511Z')
     */
    static std::string to_datetime_ms(int64_t ms);

    /**
     * Serialize data to json string
     */
    template <typename T>
    static std::string to_json(T value);

    template <typename T>
    static std::string to_json(std::vector< T > values);
};

#if defined(_JSON_DECODER_)

#include <unordered_map>
using std::unordered_map;
using std::string;

typedef void (* JsonDecoderRt)(JsonUtils::JsonImpl *jnode, void *object);

typedef unordered_map<string, JsonDecoderRt> decoder_map_t;

void decode_json_tree(const decoder_map_t &jmap, JsonUtils::JsonImpl *root, void *object);

#endif /* _JSON_DECODER_ */