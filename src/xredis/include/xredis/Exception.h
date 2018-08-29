/* Time-stamp: <[Exception.h] modified by Guangxu Pan on 2014-04-24 08:42:27 星期四> */
#pragma once

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace x {

/**
 * 
 * @author Guangxu Pan, bajizhh@gmail.com
 * @since 2013-12-24 13:55:53
 * @version 2013-12-24 13:55:53
 */
    class Exception: virtual public std::exception {
    private:
        string throw_file_;
        int throw_line_;
        string what_;
    
    public:
        explicit Exception() throw();
        explicit Exception(const string& s) throw();
        Exception(const x::Exception& e);
        virtual ~Exception() throw();

        const char* what() const throw();

        string throw_file() {
            return throw_file_;
        }

        int throw_line() {
            return throw_line_;
        }

        Exception& throw_file(string file);
        Exception& throw_line(int line);

        template <typename T>
        inline Exception& operator<<(T const & t) {
            stringstream ss;
            ss << t;
            what_.append(ss.str());
            return *this;
        }
};

#define xthrow(e) throw x::Exception(e).throw_file(__FILE__).throw_line(__LINE__)

}
