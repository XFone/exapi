/*
 * $Id: $
 *
 * System I/O dispatching implementation
 *
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#include "Base.h"
#include "Log.h"
#include "Trace.h"

#include "GlobalDefine.h"
#include "DispatchIO.h"
#include "algoapi/zmq/ZmqHelper.h"

#include <string>

using namespace std;
using namespace ATS;

#define USE_ZMQ_POLL        1
#define ZMQ_POLL_TIMEOUT    10      // 10ms

DispatchIO::DispatchIO(void) : n_mq(0)
{
    m_zmqctx = zmq_ctx_new();
    zmq_ctx_set(m_zmqctx, ZMQ_MAX_SOCKETS, 128);
}

DispatchIO::~DispatchIO(void)
{
    for (int i = 0; i < n_mq; i++) {
        delete a_mqs[i]; 
    }

    zmq_ctx_term(m_zmqctx);
    // zmq_ctx_destroy(m_zmqctx); // deprecated
}

BaseZmqClient *DispatchIO::AssignSfHandler(SwordFishIntf *psf, BaseMsgDispatcher *dsp) 
{
    if (n_mq < (int)MAX_MQ_COUNT) {
        psf->SetContext(m_zmqctx);
        psf->SetUserParseFunc(dsp->GetMessageParser());
        a_mqs[n_mq++] = new _MqDesc(psf, dsp, dsp);
    }
    return (BaseZmqClient *)psf;
}

BaseZmqClient *DispatchIO::AssignMqHandler(BaseZmqClient *pmq, IDispatcher *dsp)
{
    if (n_mq < (int)MAX_MQ_COUNT) {
        pmq->SetContext(m_zmqctx);
        a_mqs[n_mq++] = new _MqDesc(pmq, dsp, NULL);
    }
    return pmq;
}

int DispatchIO::DispatchInfinitely(void *ctx, bool do_egress)
{
    int i;

#if USE_ZMQ_POLL
    int n_items = 0;
    zmq_pollitem_t poll_items[MAX_MQ_COUNT];
    
    for (i = 0; i < n_mq && i < (int)MAX_MQ_COUNT; i++) {
        zmq_pollitem_t *pitem = &poll_items[i];
        pitem->socket  = a_mqs[i]->GetSocket();
        pitem->fd      = a_mqs[i]->GetFd();
        pitem->events  = ZMQ_POLLIN | ZMQ_POLLERR;
        pitem->revents = 0;
        n_items++;

        TRACE_THREAD(7, "poll_item { socket: 0x%x, fd: %d }", 
                     pitem->socket, pitem->fd); 
    }

    LOGFILE(LOG_DEBUG, "ZMQ polling on %d items", n_items);

#endif /* USE_ZMQ_POLL */
 
    do {

#if USE_ZMQ_POLL
        int rc = zmq_poll(poll_items, n_items, ZMQ_POLL_TIMEOUT);
        
        if (rc > 0) {          // has events
            i = 0;
            TRACE_THREAD(8, "Got %d events", rc);
            while (i < n_items && rc > 0) {
                int revts = poll_items[i].revents;
                if (revts) { // this item got an event
                    rc--;

                    TRACE_THREAD(8, "ZMQ#%d polled!", i);
                    
                    if (revts & ZMQ_POLLIN) {
                        onDispatch(a_mqs[i]);
                    }

                    if (revts & ZMQ_POLLERR) {
                        LOGFILE(LOG_WARN, "socket %d has error", i);
                        // TODO
                    }
                }

                i++;

            } // while (i && rc)

        } else if (0 == rc) {  // timeout
            // TODO ---
            // TRACE_THREAD(7, "timeout!");

        } else {               // -1 check errors
            int err = zmq_errno();
            switch (err) {
            case EBADF:  // fd or socket closed
                // TODO: reopen or re-initialize MQ
                LOGFILE(LOG_WARN, "zmq_poll got EBADF: %s", zmq_strerror(err));
                break;

            case EINTR:
                // got SIGCHLD or SIGALRM
                // assert(0);
                LOGFILE(LOG_WARN, "got SIGCHLD or SIGALRM");
                break;

            default:
                // what's up?
                LOGFILE(LOG_WARN, "zmq_poll got error: %s", zmq_strerror(err));
                // assert(0);
                break;
            }

        } // if (rc)

#else /* !USE_ZMQ_POLL */
        
        for (i = 0; i < n_mq; i++) {
            onDispatch(&a_mqs[i]);
        }

#endif /* USE_ZMQ_POLL */

        if (do_egress) {
            ProcessEgress();
        } 

    } while (!g_is_exiting);

    return 0;
}

TDispResult DispatchIO::onDispatch(Object desc) 
{
    _MqDesc    *mqd  = (_MqDesc *)desc;
    TDispResult ret  = DR_DROPPED;
    Object      obj;

    if (NULL != (obj = mqd->doRecv())) {
        ret = mqd->doDispatch(obj);
    } else {
        TRACE_THREAD(7, "DispatchIO::onDispatch: unknown message");        
    }

    if (DR_SUCCESS != ret) {
        // TODO: to check status
    }

    if (DR_CONTINUE == ret) {
        // TODO: broadcast this message to all
    }

    return ret;
}
