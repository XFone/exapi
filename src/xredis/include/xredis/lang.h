/* Time-stamp: <[lang.h] modified by Guangxu Pan on 2014-11-05 14:16:35 星期三> */
#pragma once

#include <string>
#include <boost/smart_ptr.hpp>
#include <boost/integer.hpp>
#include <google/protobuf/message.h>

using namespace std;

namespace x {

    //      typedef mpz_class int_t;
    //        typedef mpf_class float_t;

    string str(const int8_t t);
    string str(const uint8_t t);
    string str(const int16_t t);
    string str(const uint16_t t);
    string str(const int32_t t);
    string str(const uint32_t t);
    string str(const int64_t t);
    string str(const uint64_t t);
    string str(const float t);
    string str(const double t);
    string str(const google::protobuf::Message& m, const string& class_name = "");
    string str(const boost::shared_ptr< google::protobuf::Message >& m, const string& class_name = "");

    //        std::string str(const int_t& t);
    //        std::string str(const float_t& t, int scale = 3);

    bool boolean(const string str);
    int8_t int8(const string str);
    uint8_t uint8(const string str);
    int16_t int16(const string str);
    uint16_t uint16(const string str);
    int32_t int32(const string str);
    uint32_t uint32(const string str);
    int64_t int64(const string str);
    uint64_t uint64(const string str);
    //int_t bigint(const string str);
    //float_t bigfloat(const string str);

    bool is_bigendian();

    void hton(char* data, size_t length);
    void hton(string& data);
    void ntoh(char* data, size_t length);
    void ntoh(string& data);

    int8_t hton(int8_t value);
    uint8_t hton(uint8_t value);
    int16_t hton(int16_t value);
    uint16_t hton(uint16_t value);
    int32_t hton(int32_t value);
    uint32_t hton(uint32_t value);
    int64_t hton(int64_t value);
    uint64_t hton(uint64_t value);

    int8_t ntoh(int8_t value);
    uint8_t ntoh(uint8_t value);
    int16_t ntoh(int16_t value);
    uint16_t ntoh(uint16_t value);
    int32_t ntoh(int32_t value);
    uint32_t ntoh(uint32_t value);
    int64_t ntoh(int64_t value);
    uint64_t ntoh(uint64_t value);

    string hex(const char* data, const size_t length);
    string hex(const string value);
    string hex(const int8_t value);
    string hex(const uint8_t value);
    string hex(const int16_t value);
    string hex(const uint16_t value);
    string hex(const int32_t value);
    string hex(const uint32_t value);
    string hex(const int64_t value);
    string hex(const uint64_t value);

    string hex2str(const string hex);


}

