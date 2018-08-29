/* Time-stamp: <[lang.cc] modified by Guangxu Pan on 2014-11-05 14:41:02 星期三> */
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>

#include <boost/integer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

//#include <gmpxx.h>

#include "xredis/lang.h"
#include "xredis/str.h"

using namespace std;

namespace x {

    /*
      class __GMP_INIT {
      public:
      __GMP_INIT() {        
      mp_bitcnt_t old_prec = mpf_get_default_prec();
      mpf_set_default_prec(1024);
      mp_bitcnt_t new_prec = mpf_get_default_prec();
      cout << "[x::u] change mpf default precision from " << old_prec <<" to " << new_prec << endl;
      }
      };
      static __GMP_INIT a();
    */


    //int8_t int8_t(const char* data);


    //        std::string str(const int_t& t);
    //        std::string str(const float_t& t, int scale = 3);

    /* ######################### 基本数据类型转换 开始 ######################### */

    /*
      template<class T> std::string str(const T& t) {
      return boost::lexical_cast<std::string>(t);
      }
    */


    string str(const int8_t t) {
        return boost::lexical_cast<string>((int16_t)t);
    }
    string str(const uint8_t t) {
        return boost::lexical_cast<string>((uint16_t)t);
    }
    string str(const int16_t t) {
        return boost::lexical_cast<string>(t);
    }
    string str(const uint16_t t) {
        return boost::lexical_cast<string>(t);
    }
    string str(const int32_t t) {
        return boost::lexical_cast<string>(t);
    }
    string str(const uint32_t t) {
        return boost::lexical_cast<string>(t);
    }
    string str(const int64_t t) {
        return boost::lexical_cast<string>(t);
    }
    string str(const uint64_t t) {
        return boost::lexical_cast<string>(t);
    }
    string str(const float t) {
        stringstream ss;
        ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
        ss << t;
        return ss.str();
    }
    string str(const double t) {
        stringstream ss;
        ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
        ss << t;
        return ss.str();
    }

    string str(const google::protobuf::Message& m, const string& class_name) {
        string s = m.Utf8DebugString();
        s = x::trim(s);
        s = x::replace(s, "\n", ", ");
        s = x::replace(s, "\\{,\\s*", "{");
        s = x::replace(s, ", \\}", "}");
        s = x::replace(s, "\\s{2, }", " ");
        if (!class_name.empty()) {
            s = class_name + "{" + s + "}";
        }
        return s;
    }

    string str(const boost::shared_ptr< google::protobuf::Message >& m, const string& class_name) {
        if (m) {
            return str(*m.get(), class_name);
        } else {
            return "";
        }
    }

    bool boolean(const string str) {
        string s = str;
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        if (s == "1" || s == "true" || s == "yes" || s == "y") {
            return true;
        } else if (s == "0" || s == "false" || s == "no" || s == "n") {
            return false;
        }
        stringstream ss;
        ss << "illegal bool value: " << str;
        throw runtime_error(ss.str());
    }
    int8_t int8(const string str) {
        int16_t i16 = boost::lexical_cast<int16_t>(str);
        if (i16 > 127 || i16 < -128) {
            stringstream ss;
            ss << "convert string to int8 failed: " << str;
            throw runtime_error(ss.str());
        }
        return (int8_t)i16;
    }
    uint8_t uint8(const string str) {
        uint16_t u16 = boost::lexical_cast<uint16_t>(str);
        if (u16 > 255) {
            stringstream ss;
            ss << "convert string to uint8 failed: " << str;
            throw runtime_error(ss.str());
        }
        return (uint8_t)u16;
    }
    int16_t int16(const string str) {
        return boost::lexical_cast<int16_t>(str);
    }
    uint16_t uint16(const string str) {
        return boost::lexical_cast<uint16_t>(str);
    }
    int32_t int32(const string str) {
        return boost::lexical_cast<int32_t>(str);
    }
    uint32_t uint32(const string str) {
        return boost::lexical_cast<uint32_t>(str);
    }
    int64_t int64(const string str) {
        return boost::lexical_cast<int64_t>(str);
    }
    uint64_t uint64(const string str) {
        return boost::lexical_cast<uint64_t>(str);
    }


    /*
      string str(const mpz_class& t) {
      return t.get_str();
      }
    */
    /**
     * 将GMP mpf_class对象转换为字符串形式
     * @param f mpf_class对象
     * @param scale 小数位数，默认为3
     */
    /*
      string str(const mpf_class& f, int scale) {
      if ( f == 0 ) {
      return "0";
      }
    
      if ( scale <= 0 ) {
      mpz_class z(f);
      return z.get_str();
      }
    
      int precision = 10; // 四舍五入精度，默认为1/10^10

      string zoom_str = "1";
      zoom_str.insert(1, scale+precision, '0');
      mpz_class zoom(zoom_str);

      string round_half_away_from_zero_str = "5";
      round_half_away_from_zero_str.insert(1, precision-1, '0');
      mpz_class round_half_away_from_zero(round_half_away_from_zero_str);

      mpz_class z(f*zoom);
      if ( z < 0 ) {
      z = z - round_half_away_from_zero - 1;
      } else {
      z = z + round_half_away_from_zero + 1;
      }

      string s = z.get_str();
      s.erase(s.length()-precision, precision);

      if ( z < 0 ) {
      if ( s.length() -1  == 0 ) {
      return "0";
      } else if ( s.length() - 1 < scale ) {
      s.insert(1, scale - s.length() + 1, '0');
      s.insert(1, "0.");
      } else if ( s.length() - 1 == scale ) {
      s.insert(1, "0.");
      } else {
      s.insert(s.length()-scale, ".");
      }
      } else {
      if ( s.length() == 0 ) {
      return "0";
      } else if ( s.length() < scale ) {
      s.insert(0, scale - s.length(), '0');
      s.insert(0, "0.");
      } else if ( s.length() == scale ) {
      s.insert(0, "0.");
      } else {
      s.insert(s.length()-scale, ".");
      }
      }

      int pos_zero = s.find_last_not_of('0');
      int pos_point = s.find_last_of('.');
      if ( pos_zero == pos_point ) {
      s.erase(pos_zero, s.length() - pos_zero);
      } else if ( pos_zero + 1 < s.length() ) { // pos_zero > pos_point
      s.erase(pos_zero + 1, s.length() - pos_zero - 1);
      }

      return s;
      }
    */
    /* ######################### 基本数据类型转换 结束 ######################### */


    bool is_bigendian() {
        int16_t n = 0x0100;
        unsigned char* b = (unsigned char*)&n;
        return b[0] == 0x01;
    }

    void hton(char* data, size_t length) {
        if (is_bigendian()) {
            return;
        }
        for (size_t i = 0;  i < length/2; i++) {
            *(data+i) ^= *(data+length-i-1);
            *(data+length-i-1) ^= *(data+i);
            *(data+i) ^= *(data+length-i-1);
        }
    }

    void hton(string& data) {
        if (is_bigendian()) {
            return;
        }
        reverse(data.begin(), data.end());
    }


    void ntoh(char* data, size_t length) {
        if (is_bigendian()) {
            return;
        }
        for (size_t i = 0;  i < length/2; i++) {
            *(data+i) ^= *(data+length-i-1);
            *(data+length-i-1) ^= *(data+i);
            *(data+i) ^= *(data+length-i-1);
        }
    }

    void ntoh(string& data) {
        if (is_bigendian()) {
            return;
        }
        reverse(data.begin(), data.end());
    }

    int8_t hton(int8_t value) {
        hton((char*)&value, sizeof(int8_t));
        return value;
    }
    uint8_t hton(uint8_t value) {
        hton((char*)&value, sizeof(uint8_t));
        return value;
    }
    int16_t hton(int16_t value) {
        hton((char*)&value, sizeof(int16_t));
        return value;
    }
    uint16_t hton(uint16_t value) {
        hton((char*)&value, sizeof(uint16_t));
        return value;
    }
    int32_t hton(int32_t value) {
        hton((char*)&value, sizeof(int32_t));
        return value;
    }
    uint32_t hton(uint32_t value) {
        hton((char*)&value, sizeof(uint32_t));
        return value;
    }
    int64_t hton(int64_t value) {
        hton((char*)&value, sizeof(int64_t));
        return value;
    }
    uint64_t hton(uint64_t value) {
        hton((char*)&value, sizeof(uint64_t));
        return value;
    }


    int8_t ntoh(int8_t value) {
        ntoh((char*)&value, sizeof(int8_t));
        return value;
    }
    uint8_t ntoh(uint8_t value) {
        ntoh((char*)&value, sizeof(uint8_t));
        return value;
    }
    int16_t ntoh(int16_t value) {
        ntoh((char*)&value, sizeof(int16_t));
        return value;
    }
    uint16_t ntoh(uint16_t value) {
        ntoh((char*)&value, sizeof(uint16_t));
        return value;
    }
    int32_t ntoh(int32_t value) {
        ntoh((char*)&value, sizeof(int32_t));
        return value;
    }
    uint32_t ntoh(uint32_t value) {
        ntoh((char*)&value, sizeof(uint32_t));
        return value;
    }
    int64_t ntoh(int64_t value) {
        ntoh((char*)&value, sizeof(int64_t));
        return value;
    }
    uint64_t ntoh(uint64_t value) {
        ntoh((char*)&value, sizeof(uint64_t));
        return value;
    }

    string hex(const char* data, size_t length) {
        stringstream ss;
        char map[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
        for (size_t i = 0;  i < length; i++) {
            unsigned char c = (unsigned char)data[i];
            if (i == 0) {
                ss << "0x";
            } else {
                ss << " ";
            }
            ss << map[(c >> 4) & 0xF];
            ss << map[c & 0xF];
        }
        return ss.str();
    }
    string hex(const string value) {
        return hex(value.data(), value.length());
    }
    string hex(int8_t value) {
        return hex((const char*)&value, sizeof(int8_t));
    }
    string hex(uint8_t value) {
        return hex((const char*)&value, sizeof(uint8_t));
    }
    string hex(int16_t value) {
        return hex((const char*)&value, sizeof(int16_t));
    }
    string hex(uint16_t value) {
        return hex((const char*)&value, sizeof(int16_t));
    }
    string hex(int32_t value) {
        return hex((const char*)&value, sizeof(int32_t));
    }
    string hex(uint32_t value) {
        return hex((const char*)&value, sizeof(int32_t));
    }
    string hex(int64_t value) {
        return hex((const char*)&value, sizeof(int64_t));
    }
    string hex(uint64_t value) {
        return hex((const char*)&value, sizeof(int64_t));
    }

    string hex2str(const string hex) {
        string value;
        string str = trim(hex);
        str = replace(str, "^0x", "");
        str = replace(str, "\\s+", "");
        size_t size = str.length();
        if (size % 2 != 0) {
            throw runtime_error("illegal hex string: size % 2 != 0");
        }
        const char *buf = str.data();
        unsigned char byte = 0;
        for (size_t i = 0;  i < size; i++) {
            unsigned char c = (unsigned char)buf[i];
            if (c >= '0' && c <= '9') {
                c = c - '0';
            } else if (c >= 'A' && c <= 'F') {
                c = c - 'A' + 10;
            } else if (c >= 'a' && c <= 'f') {
                c = c - 'a' + 10;
            } else {
                stringstream ss;
                ss << "illegal hex char found: " << c;
                throw runtime_error(ss.str());
            }
            if (i % 2 == 1) {
                byte = (byte << 4) | c;
                value.append(1, (char)byte);
            } else {
                byte = c;
            }
        }
        return value;
    }



}
