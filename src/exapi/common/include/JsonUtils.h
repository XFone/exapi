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

        Json & at(const char *name) throw(std::runtime_error);

        template<typename T> T get() throw(std::runtime_error);
        // bool        get() throw(std::runtime_error);
        // double      get() throw(std::runtime_error);
        // std::string get() throw(std::runtime_error);

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