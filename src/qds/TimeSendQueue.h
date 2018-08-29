/*
 * $Id: $
 *
 * Realtime send queue
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file TimeSendQueue.h Realtime send queue */
#include "Base.h"
#include "TimeType.h"

#include "algoapi/protos/FuncId.h"
#include "algoapi/protos/Enum2Message.h"
#include "fifo/IQueue.h"
#include "timer/TimerReal.h"

namespace ATS {

    using namespace AlgoApi::Protos;

    /**
     * Send message acurately on hardware clock time like an oscillator
     * This queue also has a throttle to prevent send too much messages in 
     * an given time interval
     */
    class TimeClockSendQueue
    {
    protected:
        static IQueue *g_egress;
        void  *m_zmq;
        int    m_sock; // TODO: use sock directly or call DispatchIO ?
        // DispatchIO *m_io; 

    protected:
        /**
         * Hidden contructor
         */
        TimeClockSendQueue(IQueue *egress, void *zmq, int fd)
            : m_zmq(zmq), m_sock(fd) {
            g_egress = egress;
        }

        /**
         * Call by worker (threads)
         * [thread]
         */
        int  ProcessEgress();
 
    public:
        static IQueue &GetQueue() {
            return *g_egress;
        }

        /**
         * Send message at the given clock-time
         * @tm date time
         * @funcId function id
         * @msg  message
         */
        void SendAt(TTimeInterval tm, FuncId funcId, PMessage msg);

        /**
         * Send data directly via socket / device fd
         * @tm date time
         * @data data bytes
         * @dlen data length 
         */
        void SendAt(TTimeInterval tm, const char *data, size_t dlen);
    };

}
