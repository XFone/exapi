/* Time-stamp: <[Exception.cc] modified by Guangxu Pan on 2014-04-24 08:43:17 星期四> */
#include <iostream>
#include <sstream>
#include <string>

#include "xredis/Exception.h"

using namespace std;

namespace x {

    Exception::Exception() throw() {}

    Exception::Exception(const string& msg) throw() {
        stringstream ss;
        ss << msg;
        what_.append(ss.str());
    }

    Exception::~Exception() throw() {}

    Exception::Exception(const x::Exception& other) {
        throw_file_ = other.throw_file_;
        throw_line_ = other.throw_line_;
        what_ = other.what_;
    }

    const char* Exception::what() const throw() {
        return what_.c_str();
    }
    
    Exception& Exception::throw_file(string file) {
        int pos = -1;
        while ((pos = file.find("\\")) >= 0) {
            file.replace(pos, 1, "/");
        }
        if ((pos = file.find("/src/")) >=0) {
            file.replace(0, pos+5, "");
        }
        throw_file_ = file;
        return *this;
    }

    Exception& Exception::throw_line(int line) {
        throw_line_ = line;
        stringstream ss;
        ss << "exception threw at " << throw_file_ << "(" << throw_line_ << "): ";
        what_.insert(0, ss.str());
        return *this;
    }


}
