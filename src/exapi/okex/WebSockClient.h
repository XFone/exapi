/*
 * $Id: $
 * 
 * WebSocketClient class declaration
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file WebSockClient.h Wrap class of restbed::Request.
 */

#include <restbed>

#include "wsshelp.h"

namespace exapi {

    /**
     * WebSocketClient
     * Wrap class of restbed::WebSocket to handling wss messages:
     */
    class WebSocketClient : public restbed::WebSocket {
    protected:
        std::string          m_address;
        std::string          m_key;
        wss_callback_open    cb_open;
        wss_callback_close   cb_close;
        wss_callback_message cb_message;

    public:
        WebSocketClient(const char *server, const char *key)
          : restbed::WebSocket(), m_address(server), m_key(key), 
            cb_open(nullptr), cb_close(nullptr), cb_message(nullptr) {
            // NOTHING
        }

        virtual ~WebSocketClient() {
            stop();
        };

        void start();

        void stop();

        /**
         * subscribe to emit/event channel
         * @param channel name of channel
         */
        void emit(const std::string channel);

        /**
         * subscribe to emit/event channel
         * @param channel name of channel
         * @param parameter parameters in json (e.g., '{}' is empty)
         */
        void emit(const std::string channel, std::string &parameter);

        /**
         * Unsubscribe to emit/event channel
         * @param channel name of channel
         */
        void remove(const std::string channel);

    };

    /**
     * OKex websocket emit parameter builder
     */
    class ParameterBuilder {
    protected:
        std::vector< std::pair<std::string, std::string> > m_params;

    public:
        ParameterBuilder() : m_params() {}

        ParameterBuilder &AddParam(const char *name, const std::string &val) {
            std::pair<std::string, std::string> param(name, val);
            m_params.push_back(param);
            return *this;
        }

        std::string build() {
            std::string result("{");
            int n = 0;
            for (auto param : m_params) {
                result += ((n++ != 0) ? ",'" : "'");
                result += param.first;
                result += "':'";
                result += param.second;
                result += "'";
            }
            result += "}";
            return result;
        }

        std::string buildSign(const std::string &secret) {
            std::string result("{");
            std::string signature;
            int n = 0;
            for (auto param : m_params) {
                result += ((n++ != 0) ? ",'" : "'");
                result += param.first;
                result += "':'";
                result += param.second;
                result += "'";
                // calculate signature
            }
            result += "'sign':'";
            result += signature;
            result += "'}";
            return result;
        }
    };
} // namespace exapi

/*  TO REMOVE: 

class WebSocket
{
private:
    client m_endpoint;
    websocketpp::connection_hdl m_hdl;
    std::string m_uri;
    CONNECTION_STATE m_con_state;
public:


    bool m_manual_close;
    typedef WebSocket type;

    WebSocket() :  
    m_manual_close(false),
    m_con_state(CONNECTION_STATE_UNKONWN),
    callbak_open(0),
    callbak_close(0),
    callbak_message(0)
    {
        
        m_endpoint.set_access_channels(websocketpp::log::alevel::all);
        m_endpoint.set_error_channels(websocketpp::log::elevel::all);

        // Initialize ASIO
        m_endpoint.init_asio();

        //Register our handlers
        //m_endpoint.set_socket_init_handler(bind(&type::on_socket_init,this,::_1));
        m_endpoint.set_tls_init_handler(bind(&type::on_tls_init,this,::_1));
        m_endpoint.set_message_handler(bind(&type::on_message,this,::_1,::_2));
        m_endpoint.set_open_handler(bind(&type::on_open,this,::_1));
        m_endpoint.set_close_handler(bind(&type::on_close,this,::_1));
        m_endpoint.set_fail_handler(bind(&type::on_fail,this,::_1));
        
    }

    void start()
    {
        websocketpp::lib::error_code ec;
        client::connection_ptr con = m_endpoint.get_connection(m_uri, ec);

        if (ec) {
            m_endpoint.get_alog().write(websocketpp::log::alevel::app,ec.message());
        }

        m_endpoint.set_open_handshake_timeout(3000);
        m_endpoint.set_close_handshake_timeout(3000);

        //con->set_proxy("http://humupdates.uchicago.edu:8443");

        m_endpoint.connect(con);
    
        // Start the ASIO io_service run loop
        m_endpoint.run();
        if(callbak_close != 0)callbak_close();
    }

    context_ptr on_tls_init(websocketpp::connection_hdl)
    {
        context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv1);

        try {
            ctx->set_options(boost::asio::ssl::context::default_workarounds |
                             boost::asio::ssl::context::no_sslv2 |
                             boost::asio::ssl::context::no_sslv3 |
                             boost::asio::ssl::context::single_dh_use);
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
        return ctx;
    }

    void on_fail(websocketpp::connection_hdl hdl)
    {
        
        client::connection_ptr con = m_endpoint.get_con_from_hdl(hdl);
        
        std::cout << "Fail handler" << std::endl;
        std::cout << con->get_state() << std::endl;
        std::cout << con->get_local_close_code() << std::endl;
        std::cout << con->get_local_close_reason() << std::endl;
        std::cout << con->get_remote_close_code() << std::endl;
        std::cout << con->get_remote_close_reason() << std::endl;
        std::cout << con->get_ec() << " - " << con->get_ec().message() << std::endl;
        
    }

    void on_close_handshake_timeout(websocketpp::connection_hdl hdl)
    {
    }

    void run(std::string &uri)
    {
        try {
            m_uri = uri;
            start();
        } catch (const std::exception & e) {
            std::cout << e.what() << std::endl;
        } catch (websocketpp::lib::error_code e) {
            std::cout << e.message() << std::endl;
        } catch (...) {
            std::cout << "other exception" << std::endl;
        }
    }
};

*/