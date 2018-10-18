/*
 * $Id: $
 * 
 * Socks5 Client Library
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file Socks5.h Socks5 Proxy Client Library
 */
#include "Base.h"
#include "Trace.h"

#include <string>
#include <system_error>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>

using error_code    = boost::system::error_code;
using tcp           = boost::asio::ip::tcp;
using address_v4    = boost::asio::ip::address_v4;

namespace exapi {

    /**
     * Socks5
     * Wrap class of restbed::WebSocket to handling wss messages:
     */
    class Socks5 {
    protected: 
        /**
         * handshake request
         */
        class Socks5Request {
        private:
            unsigned char           m_version;
            unsigned char           m_command;
            unsigned char           m_null_byte;
            unsigned char           m_addrtype; // 0x1: ipv4; 0x3: hostname
            unsigned char           m_addrlen;
            std::string             m_address;
            unsigned char           m_port_high;
            unsigned char           m_port_low;

        public:

            Socks5Request(const std::string &host, const std::string &port) 
                : m_version(0x05), m_command(0x01), m_null_byte(0)
            {

                m_addrtype = 0x03; // hostname
                // m_addrtype = 0x01; // IPv4

                // Convert port number to network byte order.
                unsigned short nport = std::stoi(port, nullptr, 10);
                m_port_high = (nport >> 8) & 0xff;
                m_port_low = nport & 0xff;

                // Save IP address in network byte order.
                m_addrlen = host.length();
                m_address = host;
            }

            void send_0(tcp::socket &socket) {
                boost::array<boost::asio::const_buffer, 3> bufs = {
                    {
                        boost::asio::buffer(&m_version, 1),
                        boost::asio::buffer(&m_command, 1),
                        boost::asio::buffer(&m_null_byte, 1)
                    }
                };
                boost::asio::write(socket, bufs);
            }

            void send_1(tcp::socket &socket) {
                size_t len = 7 + m_addrlen;
                boost::array<boost::asio::const_buffer, 128> bufs = {
                    {
                        boost::asio::buffer(&m_version, 1),
                        boost::asio::buffer(&m_command, 1),
                        boost::asio::buffer(&m_null_byte, 1),
                        boost::asio::buffer(&m_addrtype, 1),
                        boost::asio::buffer(&m_addrlen, 1),
                        boost::asio::buffer(m_address.c_str(), m_addrlen),
                        boost::asio::buffer(&m_port_high, 1),
                        boost::asio::buffer(&m_port_low, 1)
                    }
                };
                boost::asio::write(socket, bufs);
            }

        };

        /**
         * handshake response
         */
        class Socks5Response {
        private:
            unsigned char           m_version;
            unsigned char           m_status;
            unsigned char           m_null_byte;
            unsigned char           m_addrtype;
            address_v4::bytes_type  m_address;
            unsigned char           m_port_high;
            unsigned char           m_port_low;
        public:

            Socks5Response() : m_version(0), m_status(0) {}

            
            void recv_0(tcp::socket &socket) {
                boost::array<boost::asio::mutable_buffer, 2> bufs = {
                    {
                        boost::asio::buffer(&m_version, 1),
                        boost::asio::buffer(&m_status, 1)
                    }
                };
                boost::asio::read(socket, bufs);
            }

            void recv_1(tcp::socket &socket) {
                boost::array<boost::asio::mutable_buffer, 7> bufs = {
                    {
                        boost::asio::buffer(&m_version, 1),
                        boost::asio::buffer(&m_status, 1),
                        boost::asio::buffer(&m_null_byte, 1),
                        boost::asio::buffer(&m_addrtype, 1),
                        boost::asio::buffer(m_address),
                        boost::asio::buffer(&m_port_high, 1),
                        boost::asio::buffer(&m_port_low, 1)
                    }
                };
                boost::asio::read(socket, bufs);
            }

            bool is_success() const {
                return m_status == 0;
            }

            unsigned char status() const {
                return m_status;
            }

            tcp::endpoint endpoint() const {
                unsigned short port = m_port_high;
                port = (port << 8) & 0xff00;
                port = port | m_port_low;

                address_v4 address(m_address);
                return tcp::endpoint(address, port);
            }
        };

        // Socks5() = delete;

    public:
        Socks5(const std::string &proxy) {
            // TODO: default constructor
        }

        static void handshake(tcp::socket &socket, const std::string &host, 
                              const std::string &port, error_code &ec) {
            Socks5Request request(host, port);

            // Send first command
            request.send_0(socket);

            // Receive a response from the SOCKS server.
            Socks5Response response;
            response.recv_0(socket);

            TRACE(7, "socks5 handshake 1");

            if (!response.is_success()) {
                ec = boost::asio::error::address_family_not_supported;
                return;
            }

            // Send second commnad
            request.send_1(socket);

            // Receive a response from the SOCKS server.
            response.recv_1(socket);

            TRACE(7, "socks5 handshake 2");

            if (!response.is_success()) {
                ec = boost::asio::error::address_family_not_supported;
                return;
            }
        }

    };

} // namespace exapi