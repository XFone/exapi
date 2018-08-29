/* Time-stamp: <[str.h] modified by Guangxu Pan on 2014-12-15 14:48:03 星期一> */
#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
#include <iconv.h>

using namespace std;


namespace x {

    /**
     * UUID(Universally Unique Identifier) Version 4(random)
     */
    std::string uuid();

    string upper(const string str);
    string lower(const string str);
    
    string replace(const string& str, const string& regex, const string& replace_str = "");
    void split(vector<string>& ret, const string& str, const string& delimiters = ",");
    void search(vector< vector<string> >& list, const string& str, const string& regex);
    void search(vector<string>& list, const string& str, const string& regex);

    string trim(const string& str);

    /**
     * 字符串编码转换
     * @param fromcode 源编码
     * @param tocode 目标编码
     * @param str 以fromcode编码的字符串
     * @param size_t 缓冲区大小
     * @return 转换后的字符串
     * @throws 如果转换失败则抛出异常
     */
    string iconv(const string& fromcode, const string& tocode, const string& str, const size_t& buf_size = 0);
    string iconv8(const string& fromcode, const string& tocode, const string& str, const size_t& buf_size = 0);

    string gbk_to_utf8(const string& str);
    string utf8_to_gbk(const string& str);


#ifdef _WIN32
    string wchar2str(wchar_t *wbuf, size_t size = -1);
    string str2wstr(const string& str);
#endif

    
} // namespace x

