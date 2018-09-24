/*
 * $Id: $
 *
 * WebSocketClient class implementation
 * 
 * Copyright (c) 2014-2018 Zerone.IO . All rights reserved.
 *
 * $Log: $
 *
 */

#include "Base.h"
#include "Log.h"
#include "Trace.h"

#include "detail/WebSocketClientImpl.ipp"
using namespace exapi;

/*---------------------------- WebSocketClient -----------------------------*/

WebSocketClient::WebSocketClient(const std::string url)
  : m_url(url), cb_open(nullptr), cb_close(nullptr), cb_message(nullptr),
    m_client(new WebSocketClientImpl())
{
    // m_url = "ws://localhost:1984/chat"; // for testing
    m_client->m_intf = this;
}

WebSocketClient::~WebSocketClient() {
    stop();
    // delete m_client;
};

void WebSocketClient::start()
{
    int ret;

    LOGFILE(LOG_DEBUG, "server '%s'...", m_url.c_str());

    if ((ret = m_client->connect(m_url)) != 0) {
        m_client->stop();
        throw std::system_error(-ret, std::generic_category());
    }

    LOGFILE(LOG_DEBUG, "ws[%s] starting...", m_client->get_key().c_str());
    m_client->start();

    for (int i = 0; i < 30; i++) {
        if (!m_client->m_ws->is_open()) {
           std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
 
    if (!m_client->m_ws->is_open()) {
        LOGFILE(LOG_ERROR, "websocket is closed");
        throw std::system_error(ECONNABORTED, std::generic_category());
    }

    LOGFILE(LOG_INFO, "ws[%s] started", m_client->get_key().c_str());
}

void WebSocketClient::stop()
{
    if (m_client->is_start) {
        LOGFILE(LOG_DEBUG, "ws[%s] stopping...", m_client->get_key().c_str());

        m_client->stop();

        LOGFILE(LOG_INFO, "ws[%s] stopped", m_client->get_key().c_str());
    }
}

void WebSocketClient::send(const std::string &cmd)
{
    if (!m_client->m_ws->is_open()) {
        throw std::system_error(ECONNABORTED, std::generic_category());
    }

    LOGFILE(LOG_DEBUG, "ws[%s] emit command %s", m_client->get_key().c_str(), cmd.c_str());

    m_client->write(cmd);
}

void WebSocketClient::subscribe(const std::string channel, std::string &parameter)
{
    // multiplexing 
    // TODO
}

void WebSocketClient::unsubscribe(std::string channel)
{
    // multiplexing 
    // TODO
}
