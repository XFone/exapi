/* Time-stamp: <[Redis.cc] modified by Guangxu Pan on 2014-12-09 20:07:41 星期二> */
#include <iostream>
#include <sstream>
#include <string>

#include "xredis/Redis.h"

using namespace std;

namespace x {
    const char SYM_STAR = '*';
    const char SYM_DOLLAR = '$';
    const string SYM_CRLF = "\r\n";
    const char SYM_LF = '\n';

    Redis::Redis(string db_host, int db_port, int db_name):
        db_host_(db_host),
        db_port_(db_port),
        db_name_(db_name),
        io_service_(),
        sock_(io_service_),
        read_buf_size_(0) {
        connect();
    }

    Redis::~Redis(){}

    boost::shared_ptr<Redis> Redis::New(string db_host, int db_port, int db_name) {
        return boost::make_shared<Redis>(db_host, db_port, db_name);
    }

    void Redis::connect() {
        boost::system::error_code ec;
        tcp::resolver resolver(io_service_);
        tcp::resolver::query query(db_host_, x::str(db_port_));
        boost::asio::connect(sock_, resolver.resolve(query), ec);
        if (ec) {
            xthrow("connect to redis server[") << db_host_ << ":" << db_port_ << "/" << db_name_ << "] failed: " << ec.message();
        }
        send_command("SELECT", x::str(db_name_));
        read_ok();
    }

    void Redis::send_command(string cmd, const vector<string>& args) {
        // pack command
        stringstream ss;
        ss << SYM_STAR << (args.size()+1) << SYM_CRLF;
        ss << SYM_DOLLAR << cmd.size() << SYM_CRLF << cmd << SYM_CRLF;
        vector<string>::const_iterator itr = args.begin();
        while (itr != args.end()) {
            string s = *itr;
            ss << SYM_DOLLAR << s.size() << SYM_CRLF << s << SYM_CRLF;
            itr++;
        }
        string raw_cmd = ss.str();
        // send command
        boost::system::error_code ec;
        boost::asio::write(sock_, boost::asio::buffer(raw_cmd.data(), raw_cmd.size()), ec);
        if (ec) {
            xthrow("write data to redis server failed: ") << ec.message();
        }
    }

    void Redis::send_command(string cmd) {
        vector<string> args;
        send_command(cmd, args);
    }
    
    void Redis::send_command(string cmd, string arg1) {
        vector<string> args;
        args.push_back(arg1);
        send_command(cmd, args);
    }

    void Redis::send_command(string cmd, string arg1, string arg2) {
        vector<string> args;
        args.push_back(arg1);
        args.push_back(arg2);
        send_command(cmd, args);
    }

    void Redis::send_command(string cmd, string arg1, string arg2, string arg3) {
        vector<string> args;
        args.push_back(arg1);
        args.push_back(arg2);
        args.push_back(arg3);
        send_command(cmd, args);
    }

    
    string Redis::_read(const size_t& size) {
        string ret;
        while (true) {
            if (read_buf_size_ <= 0) {
                boost::system::error_code ec;
                size_t len = boost::asio::read(sock_, read_buf_, boost::asio::transfer_at_least(1), ec);
                if (!ec) {
                    read_buf_size_ += len;
                } else {
                    xthrow() << "read data from redis server failed: " << ec.message();
                }
            }
            read_buf_.sgetc();
            char c = read_buf_.sbumpc();
            read_buf_size_ -= 1;
            ret.append(1, c);
            if (ret.size() == size) {
                break;
            }
        }
        return ret;
    }
    
    string Redis::_read_until(const char& delimiter) {
        string ret;
        while (true) {
            if (read_buf_size_ <= 0) {
                boost::system::error_code ec;
                size_t len = boost::asio::read(sock_, read_buf_, boost::asio::transfer_at_least(1), ec);
                if (!ec) {
                    read_buf_size_ += len;
                } else {
                    xthrow() << "read data from redis server failed: " << ec.message();
                }
            }
            read_buf_.sgetc();
            char c = read_buf_.sbumpc();
            read_buf_size_ -= 1;
            ret.append(1, c);
            if (c == delimiter) {
                break;
            }
        }
        return ret;
    }

    string Redis::read(const int64_t& size) {
        string ret;
        if (size == -1) {
            ret = _read_until(SYM_LF);
        } else {
            ret = _read(size+2); // read with the line ending
        }
        ret.erase(ret.size() - 2, 2); // remove the line ending
        return ret;
    }

    int64_t Redis::read_int() {
        string s = read();
        if (s.at(0) == '-') { // -ERR ....
            xthrow(s);
        }
        s.erase(0, 1);
        return int64(s);
    }

    string Redis::read_str() {
        string s = read();
        if (s.at(0) == '-') { // -ERR ....
            xthrow(s);
        }
        s.erase(0, 1);
        int64_t size = x::int64(s);
        if (size == -1) { // # $-1\r\n
            return "";
        } else { // "$3\r\nABC\r\n"
            return read(size);
        }
    }

    size_t Redis::read_list(vector<string>& v) {
        string s = read();
        if (s.at(0) == '-') { // -ERR ....
            xthrow(s);
        }
        s.erase(0, 1);
        int64_t size = x::int64(s);
        for (int64_t i = 0; i < size; ++i) {
            string data = read_str();
            v.push_back(data);
        }
        return size;
    }

    bool Redis::read_ok() {
        string s = read();
        if (s.find("+OK") == 0) {
            return true;
        } else {
            s.erase(0, 1);
            xthrow() << s;
        }
    }

    // +------------------------- SERVER INFORMATION -------------------------+
    string Redis::echo(const string& str) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("ECHO", str);
        return read_str();
    }
    int64_t Redis::dbsize() {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("DBSIZE");
        return read_int();
    }
    void Redis::flushdb() {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("FLUSHDB");
        read_ok();
    }
    void Redis::flushall() {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("FLUSHALL");
        read_ok();
    }

    size_t Redis::keys(vector<string>& ret, const string& pattern) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("KEYS", pattern);
        size_t size = read_list(ret);
        return size;
    }
    
    // +------------------------- BASIC KEY COMMANDS -------------------------+
    void Redis::del(const string& key) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("DEL", key);
        read_int();
    }
    void Redis::del(const vector<string>& keys) {
        boost::mutex::scoped_lock lock(mutex_);
        if (keys.empty()) {
            return;
        }
        send_command("DEL", keys);
        read_int();
    }
    string Redis::get(const string& key) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("GET", key);
        return read_str();
    }
    void Redis::mget(vector<string>& ret, const vector<string>& keys) {
        if (keys.empty()) {
            return;
        }
        boost::mutex::scoped_lock lock(mutex_);
        send_command("MGET", keys);
        size_t size = read_list(ret);
        if (size != keys.size()) {
            xthrow("MGET size don't match: key_size=") << keys.size() << ", value_size=" << size;
        }
    }
    void Redis::set(const string& key, const string& value) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("SET", key, value);
        read_ok();
    }
    void Redis::mset(const map<string, string>& data) {
        if (data.empty()) {
            return;
        }
        boost::mutex::scoped_lock lock(mutex_);
        vector<string> v;
        map<string, string>::const_iterator itr = data.begin();
        while (itr != data.end()) {
            v.push_back(itr->first);
            v.push_back(itr->second);
            itr++;
        }
        send_command("MSET", v);
        read_ok();
    }
    string Redis::getset(const string& key, const string& value) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("GETSET", key, value);
        return read_str();
    }
    
    // +------------------------- LIST COMMANDS -------------------------+
    string Redis::lindex(const string& key, const int64_t& index) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("LINDEX", key, x::str(index));
        return read_str();
    }
    size_t Redis::llen(const string& key) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("LLEN", key);
        return (size_t)read_int();
    }
    string Redis::lpop(const string& key) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("LPOP", key);
        return read_str();
    }
    string Redis::rpop(const string& key) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("RPOP", key);
        return read_str();
    }
    size_t Redis::lpush(const string& key, const string& value) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("LPUSH", key, value);
        return (size_t)read_int();
    }
    size_t Redis::lpush(const string& key, const vector<string>& values) {
        boost::mutex::scoped_lock lock(mutex_);
        vector<string> v;
        v.push_back(key);
        v.insert(v.end(), values.begin(), values.end());
        send_command("LPUSH", v);
        return (size_t)read_int();
    }
    size_t Redis::rpush(const string& key, const string& value) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("RPUSH", key, value);
        return (size_t)read_int();
    }
    size_t Redis::rpush(const string& key, const vector<string>& values) {
        boost::mutex::scoped_lock lock(mutex_);
        vector<string> v;
        v.push_back(key);
        v.insert(v.end(), values.begin(), values.end());
        send_command("RPUSH", v);
        return (size_t)read_int();
    }
    void Redis::lrange(vector<string>& ret, const string& key, const int64_t& start, const int64_t& end) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("LRANGE", key, x::str(start), x::str(end));
        read_list(ret);
    }
    void Redis::lset(const string& key, const int64_t& index, const string& value) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("LSET", key, x::str(index), value);
        read_ok();
    }
    // +------------------------- SET COMMANDS -------------------------+
    // Add one or more members to a set
    void Redis::sadd(const string& key, const string& value) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("SADD", key, value);
        read_int(); // 添加：返回1，已存在：返回0
    }
    void Redis::sadd(const string& key, const vector<string>& values) {
        boost::mutex::scoped_lock lock(mutex_);
        vector<string> v;
        v.push_back(key);
        v.insert(v.end(), values.begin(), values.end());
        send_command("SADD", v);
        read_int();
    }
    size_t Redis::scard(const string& key) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("SCARD", key);
        return (size_t)read_int();
    }
    bool Redis::sismember(const string& key, const string& value) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("SISMEMBER", key, value);
        return read_int() == 1;
    }
    size_t Redis::smembers(vector<string>& ret, const string& key) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("SMEMBERS", key);
        return read_list(ret);
    }
    void Redis::srem(const string& key, const string& value) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("SREM", key, value);
        read_int(); // 删除成功：返回1，不存在：返回0
    }
    void Redis::srem(const string& key, const vector<string>& values) {
        boost::mutex::scoped_lock lock(mutex_);
        vector<string> v;
        v.push_back(key);
        v.insert(v.end(), values.begin(), values.end());
        send_command("SREM", v);
        read_int();
    }

    // +------------------------- HASH COMMANDS -------------------------+
    void Redis::hdel(const string& key, const string& hkey) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("HDEL", key, hkey);
        read_int();
    }
    void Redis::hdel(const string& key, const vector<string>& hkeys) {
        boost::mutex::scoped_lock lock(mutex_);
        vector<string> v;
        v.push_back(key);
        v.insert(v.end(), hkeys.begin(), hkeys.end());
        send_command("HDEL", v);
        read_int();
    }
    string Redis::hget(const string& key, const string& hkey) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("HGET", key, hkey);
        return read_str();
    }
    size_t Redis::hgetall(map<string, string>& ret, const string& key) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("HGETALL", key);
        vector<string> v;
        size_t size = read_list(v);
        if (size % 2 != 0) {
            xthrow("HGETALL failed, illegal result set size: ") << size;
        }
        vector<string>::iterator itr = v.begin();
        while (itr != v.end()) {
            string i_key = *itr;
            itr++;
            string i_value = *itr;
            itr++;
            ret[i_key] = i_value;
        }
        return size;
    }
    size_t Redis::hkeys(vector<string>& ret, const string& key) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("HKEYS", key);
        return read_list(ret);
    }
    size_t Redis::hlen(const string& key) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("HLEN", key);
        return (size_t)read_int();
    }
    void Redis::hset(const string& key, const string& hkey, const string& value) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("HSET", key, hkey, value);
        read_int();
    }
    void Redis::hmset(const string& key, const map<string, string>& values) {
        boost::mutex::scoped_lock lock(mutex_);
        vector<string> v;
        v.push_back(key);
        map<string, string>::const_iterator itr = values.begin();
        while (itr != values.end()) {
            v.push_back(itr->first);
            v.push_back(itr->second);
            itr ++;
        }
        send_command("HMSET", v);
        read_ok();
    }
    void Redis::hmget(vector<string>& ret, const string& key, const vector<string>& hkeys) {
        boost::mutex::scoped_lock lock(mutex_);
        vector<string> v;
        v.push_back(key);
        v.insert(v.end(), hkeys.begin(), hkeys.end());
        send_command("HMGET", v);
        size_t size = read_list(ret);
        if (size != hkeys.size()) {
            xthrow("HMGET failed, size don't match, keys_size=") << hkeys.size() << ", value_size=" << size;
        }
    }
    size_t Redis::hvals(vector<string>& ret, const string& key) {
        boost::mutex::scoped_lock lock(mutex_);
        send_command("HVALS", key);
        return (size_t)read_list(ret);
    }

    

}
