/*
 * $Id: $
 *
 * Realtime send queue (implementation)
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */

#include "Base.h"
#include "Log.h"

#include <assert.h>

#include "GlobalDefine.h"
#include "TimeSendQueue.h"
#include "algoapi/zmq/ZmqHelper.h"

using namespace ATS;
using namespace AlgoApi::Zmq;

IQueue *TimeClockSendQueue::g_egress = NULL;

int  TimeClockSendQueue::ProcessEgress()
{ 
    // TODO: send-queue with throttle and accurite clock-time
    int n = 0;
    zmq_msg_t msg;

    if (NULL != g_egress && 0 == (n = g_egress->pop_front<zmq_msg_t>(&msg))) {
        TRACE_THREAD(6, "<< SF << %08x << (%d bytes)", zmq_msg_data(&msg), zmq_msg_size(&msg));
        if (zmq_sendmsg(m_zmq, &msg, 0) < 0) {
            LOGFILE(LOG_ERROR, "TimeClockSendQueue: zmq send fail: %s", zmq_strerror(errno));
        }
        zmq_msg_close(&msg);
        n++;    
    }

    return n; 
}

void 
TimeClockSendQueue::SendAt(TTimeInterval tm, FuncId funcId, PMessage msg)
{ 
    // TODO
}

void 
TimeClockSendQueue::SendAt(TTimeInterval tm, const char *data, size_t dlen)
{
    // TODO
    assert(0);
}
