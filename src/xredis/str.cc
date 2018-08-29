/* Time-stamp: <[str.cc] modified by Guangxu Pan on 2014-12-15 14:47:57 星期一> */
#include <iostream>
#include <sstream>
#include <string>

#include <boost/regex.hpp>
#include <boost/algorithm/string_regex.hpp>
//#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>


#include "xredis/Exception.h"
#include "xredis/str.h"

using namespace std;

namespace x {

    string uuid() {
        boost::uuids::random_generator gen;
        std::string str = boost::uuids::to_string(gen());
        boost::algorithm::erase_all(str, "-");
        return str;
    }

    string upper(const string str) {
        return boost::to_upper_copy(str);
    }

    string lower(const string str) {
        return boost::to_lower_copy(str);
    }

    string replace(const string& str, const string& pattern, const string& replace_str) {
        boost::regex p(pattern);
        string replaced = boost::regex_replace(str, p, replace_str);
        return replaced;
    }

    void search(vector< vector<string> >& list, const string& str, const string& regex) {
        string::const_iterator begin = str.begin();
        string::const_iterator end = str.end();
        boost::regex expression(regex);
        boost::match_results<std::string::const_iterator> what;
        boost::match_flag_type flags = boost::match_default;

        while(regex_search(begin, end, what, expression, flags)) {
            size_t size = what.size();
            vector<string> group(size);
            for (size_t i = 0; i < size; i++) {
                group[i] = what[i].str();
            }
            list.push_back(group);
            begin = what[0].second;
        }
    }

    void search(vector<string>& list, const string& str, const string& regex) {
        boost::regex expression(regex);
        boost::algorithm::find_all_regex(list, str, expression);
    }

    void split(vector<string>& list, const string& str, const string& delimiters) {
        if (str.empty()) {
            return;
        }
        boost::algorithm::split(list, str, boost::algorithm::is_any_of(delimiters), boost::algorithm::token_compress_off);
        vector<string>::iterator itr = list.begin();
        while (itr != list.end()) {
            boost::trim(*itr);
            itr++;
        }
    }

    string trim(const string& str) {
        return boost::trim_copy(str);
    }

    string iconv(const string& fromcode, const string& tocode, const string& str, const size_t& buf_size) {
        // @TODO 转换海量数据时有问题，比如一个完整的电子书
        iconv_t cd = iconv_open(tocode.c_str(), fromcode.c_str()); // create conversion descriptor
        if (cd == (iconv_t)-1) {
            xthrow() << "invoke iconv_open failed";
        }
        size_t dest_size = buf_size <= 0 ? str.size() * 5 : buf_size;
        string dest(dest_size, '\0'); // GBK: 2字节，UTF8: 1~3字节
        char* inbuf = (char*)str.c_str();
        size_t inbytesleft = str.size();
        char* outbuf = (char*)dest.c_str();
        size_t outbytesleft = dest.size();
#ifdef _WIN32
        size_t size = ::iconv(cd, (const char* *)&inbuf, (size_t *)&inbytesleft, (char* *)&outbuf, (size_t *)&outbytesleft);
#else
        size_t size = ::iconv(cd, (char* *)&inbuf, (size_t *)&inbytesleft, (char* *)&outbuf, (size_t *)&outbytesleft);
#endif
        if (size == (size_t)(-1)) {
            iconv_close(cd);
            xthrow() << "invoke iconv failed";
        }
        if (iconv_close(cd) != 0) {
            xthrow() << "invoke iconv_close failed";
        }
        string ret(dest.c_str());
        return ret;
    }

    string iconv8(const string& fromcode, const string& tocode, const string& str, const size_t& buf_size) {
        // @TODO 转换海量数据时有问题，比如一个完整的电子书
        iconv_t cd = iconv_open(tocode.c_str(), fromcode.c_str()); // create conversion descriptor
        if (cd == (iconv_t)-1) {
            xthrow() << "invoke iconv_open failed";
        }
        size_t dest_size = buf_size <= 0 ? str.size() * 5 : buf_size;
        string dest(dest_size, '\0'); // GBK: 2字节，UTF8: 1~3字节
        char* inbuf = (char*)str.c_str();
        size_t inbytesleft = str.size();
        char* outbuf = (char*)dest.c_str();
        size_t outbytesleft = dest.size();
#ifdef _WIN32
        size_t size = ::iconv(cd, (const char* *)&inbuf, (size_t *)&inbytesleft, (char* *)&outbuf, (size_t *)&outbytesleft);
#else
        size_t size = ::iconv(cd, (char* *)&inbuf, (size_t *)&inbytesleft, (char* *)&outbuf, (size_t *)&outbytesleft);
#endif
        if (size == (size_t)(-1)) {
            iconv_close(cd);
            xthrow() << "invoke iconv failed";
        }
        if (iconv_close(cd) != 0) {
            xthrow() << "invoke iconv_close failed";
        }
        string ret(dest.c_str());
        return ret;
    }


    string gbk_to_utf8(const string& str) {
        return iconv("GBK", "UTF-8", str, str.size()*2);
    }

    string utf8_to_gbk(const string& str) {
        return iconv("UTF-8", "GBK", str, str.size()*2);
    }


#ifdef _WIN32
    string wchar2str(wchar_t *wbuf, size_t size) {
        if (size == 0) {
            return "";
        }
        if (size == (size_t)-1) {
            size = wcslen(wbuf);
        }
        string str;
        str.append(2*size+1, (char)0);
        char *buf = (char *)str.data();
        memset(buf, 0, str.length());
        WideCharToMultiByte(CP_OEMCP, NULL, wbuf, -1, buf, str.length(), NULL, FALSE);
        string s(buf, strlen(buf));
        s = x::trim(s);
        return s;
    }

    string str2wstr(const string& str) {
        if (str.empty()) {
            return "";
        }
        string wstr;
        UINT code_page = CP_ACP; // CP_ACP, CP_UTF8， 使用CP_ACP转换str为GBK编码的字符串成功
        int wlen = MultiByteToWideChar(code_page, MB_PRECOMPOSED, str.data(), str.size(), NULL, 0);
        if (wlen > 0) {
            wstr.append(wlen * sizeof(wchar_t) + 1, (char)0);
            int rc = MultiByteToWideChar(code_page, MB_PRECOMPOSED, str.data(), str.size(), (LPWSTR)wstr.data(), wlen);
            if (rc == 0) {
                xthrow() << "invoke MultiByteToWideChar failed";
            }
        }
        return wstr;
    }

#endif


} // namespace x
