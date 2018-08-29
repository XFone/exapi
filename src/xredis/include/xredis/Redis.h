/* Time-stamp: <[Redis.h] modified by Guangxu Pan on 2014-12-09 20:00:37 星期二> */
#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

#include "lang.h"
#include "str.h"
#include "Exception.h"

using namespace std;
using boost::asio::ip::tcp;

namespace x {

    /**
     * @TODO 增加断线自动重连的功能。
     * @author Guangxu Pan, bajizhh@gmail.com
     * @since 2013-12-03 22:19:02
     * @version 2013-12-03 22:19:02
     * @seealso http://redis.io/commands
     */
    class Redis {
    private:
        string db_host_;
        int db_port_;
        int db_name_;

        boost::asio::io_service io_service_;
        tcp::socket sock_;
        boost::asio::streambuf read_buf_;
        size_t read_buf_size_;
        boost::mutex mutex_;

        string _read(const size_t& size);
        string _read_until(const char& delimiter);

        Redis(const Redis& obj); // disable object copy
        Redis& operator=(const Redis& obj);

    protected:
        void connect();
        void send_command(string cmd, const vector<string>& args);
        void send_command(string cmd);
        void send_command(string cmd, string arg1);
        void send_command(string cmd, string arg1, string arg2);
        void send_command(string cmd, string arg1, string arg2, string arg3);
        string read(const int64_t& size = -1);
        
        int64_t read_int();
        string read_str();
        size_t read_list(vector<string>& v);
        bool read_ok();

    public:
        explicit Redis(string db_host, int db_port, int db_name);
        ~Redis();

        static boost::shared_ptr<Redis> New(string db_host, int db_port, int db_name);

        // +------------------------- SERVER INFORMATION -------------------------+
        // Echo the given string
        string echo(const string& str);
        // Return the number of keys in the selected database
        int64_t dbsize();
        // Remove all keys from the current database
        void flushdb();
        // Remove all keys from all databases
        void flushall();
        // Find all keys matching the given pattern
        size_t keys(vector<string>& ret, const string& pattern);

        // +------------------------- BASIC KEY COMMANDS -------------------------+
        // Delete a key
        void del(const string& key);
        // Delete keys
        void del(const vector<string>& keys);
        // Get the value of a key
        string get(const string& key);
        // Get the values of all the given keys
        void mget(vector<string>& ret, const vector<string>& keys);
        // Set the value at key ``key`` to ``value``
        void set(const string& key, const string& value);
        // Sets each key in the ``mapping`` dict to its corresponding value
        void mset(const map<string, string>& data);
        // Set the string value of a key and return its old value
        string getset(const string& key, const string& value);
        
        // +------------------------- LIST COMMANDS -------------------------+
        // Return the item from list ``name`` at position ``index``
        // Negative indexes are supported and will return an item at the end of the list
        string lindex(const string& key, const int64_t& index);
        // Return the length of the list ``name``
        size_t llen(const string& key);
        // Remove and get the first element in a list
        string lpop(const string& key);
        // Remove and get the last element in a list
        string rpop(const string& key);
        // Prepend one or multiple values to a list
        // @return: the length of the list after the push operations.
        size_t lpush(const string& key, const string& value);
        size_t lpush(const string& key, const vector<string>& values);
        // Append one or multiple values to a list
        // @return: the length of the list after the push operations.
        size_t rpush(const string& key, const string& value);
        size_t rpush(const string& key, const vector<string>& values);
        // Return a slice of the list ``key`` between position ``start`` and ``end``
        // These offsets can also be negative numbers indicating offsets starting at the end of the list. 
        void lrange(vector<string>& ret, const string& key, const int64_t& start, const int64_t& end);
        // Set the value of an element in a list by its index
        void lset(const string& key, const int64_t& index, const string& value);

        // +------------------------- SET COMMANDS -------------------------+
        // Add one or more members to a set
        void sadd(const string& key, const string& value);
        void sadd(const string& key, const vector<string>& values);
        // Get the number of members in a set
        size_t scard(const string& key);
        // Determine if a given value is a member of a set
        bool sismember(const string& key, const string& value);
        // Get all the members in a set
        size_t smembers(vector<string>& ret, const string& key);
        // Remove one or more members from a set
        void srem(const string& key, const string& value);
        void srem(const string& key, const vector<string>& value);

        // +------------------------- HASH COMMANDS -------------------------+
        // Delete one or more hash fields
        void hdel(const string& key, const string& hkey);
        void hdel(const string& key, const vector<string>& hkeys);
        // Get the value of a hash field
        string hget(const string& key, const string& hkey);
        // Get all the fields and values in a hash
        size_t hgetall(map<string, string>& ret, const string& key);
        // Get all the fields in a hash
        size_t hkeys(vector<string>& ret, const string& key);
        // Get the number of fields in a hash
        size_t hlen(const string& key);
        // Set the string value of a hash field
        void hset(const string& key, const string& hkey, const string& value);
        // Set multiple hash fields to multiple values
        void hmset(const string& key, const map<string, string>& values);
        // Get the values of all the given hash fields
        void hmget(vector<string>& ret, const string& key, const vector<string>& hkeys);
        // Get all the values in a hash
        size_t hvals(vector<string>& ret, const string& key);
    };

    typedef boost::shared_ptr<Redis> redis_ptr;

}
