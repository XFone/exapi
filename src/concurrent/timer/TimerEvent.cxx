/*
 * $Id: $
 *
 * TimerEvent is a event timer for state-machine (implementation)
 *  
 * ----------------------------------------------------------------------------
 *
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */

#include <time.h>
#include <limits.h>

#include "Base.h"
#include "Trace.h"
#include "Log.h"
#include "TimeType.h"

#include "timer/TimerEvent.h"
#include "timer/TimerReal.h"
#include "pool/MemoryPool.h"

#include <event2/event.h>
#include <event2/event_compat.h>
#include <event2/event_struct.h>

using namespace ATS;

#define USE_TIMER_REAL   1
#define USE_SIG_ALARM    0

static const struct timeval zero_time = { 0, 0 };
// static const struct timeval max_time  = { LONG_MAX, LONG_MAX };

// thread-local
static _THREAD_LOCAL_ TimerEvent *_instance = NULL;

typedef TimerEvent::TimeoutCallback PTIMER_FUNC;
typedef TimerEvent::POwner          PTIMER_USER;

/** Pointer to Timer Item
 */
typedef struct _timer_ctx *PTIMER_ITEM;

/** Timer Item
 */
typedef struct _timer_ctx {
    PTIMER_ITEM    _next;       /**< pointer to next timer                */

    PTIMER_FUNC    _func;       /**< on_timer callback function           */
    PTIMER_USER    _user;       /**< timer context defined by user        */
    int             tag;        /**< timer identifier                     */
    
    struct timeval  tvdef;      /**< timer default value                  */
    struct timeval  tvval;      /**< timer absolute expiration time       */

#if USE_TIMER_REAL
    struct event    evt;        /**< use libevent's event instance        */
#endif

} TimerItem;

MemoryPool<TimerItem> l_timer_pool(1024, sizeof(TimerItem), 1024); 

TimerEvent& TimerEvent::Instance()
{
    if (NULL == _instance) {
        _instance = new TimerEvent();
    }
    return *_instance;
}

static PTIMER_ITEM _alloc_timer(PTIMER_ITEM list_head, int tag, long secs, long usecs)
{
    PTIMER_ITEM ptm = new (l_timer_pool.allocate(1)) TimerItem(); // TODO: memory leak

    assert(NULL != ptm);

    ptm->tag   = tag;
    ptm->tvdef.tv_sec  = secs;
    ptm->tvdef.tv_usec = usecs;
    ptm->tvval = zero_time;

    ptm->_next = list_head;
    list_head  = ptm;

    return ptm;
}

#if 0
static void _free_timer(PTIMER_ITEM list_head, PTIMER_ITEM ptm)
{
    PTIMER_ITEM pcur, prev = NULL;
    for (pcur = list_head; pcur != NULL; pcur = pcur->_next) {
        if (pcur == ptm)
            break;
        prev = pcur;
    }
    if (pcur == ptm) {
        if (prev != NULL) {
            prev->_next = pcur->_next;
        } else {
            list_head = pcur->_next;
        }
        l_timer_pool.deallocate(pcur);
        //delete pcur;
    } else {
        LOGFILE(LOG_WARN, "timer not in list");
        assert(0);
    }    
}
#endif


#if USE_TIMER_REAL
static void event_timer_handler(const int fd, const short which, void *arg) {
    PTIMER_ITEM ptm = static_cast<PTIMER_ITEM>(arg);
    assert(NULL != ptm);
    assert(NULL != ptm->_func);
    //TRACE_THREAD(7, "event_timer_handler(%d, %d, %x): cb %p", fd, which, arg, pctx->_func);
    if (NULL != ptm->_user) {
        (*ptm->_func)(ptm->tag, ptm->_user); // callback in TimerReal thread   
    } else {
        TRACE_THREAD(7, "event_timer_handler: NULL argument");
        assert(0);
    }
}
#endif /* USE_TIMER_REAL */

void TimerEvent::InitTimer(PTimer &pt, int tag, TimeoutCallback cb, long usec)
{
    PTIMER_ITEM ptm = _alloc_timer(static_cast<PTIMER_ITEM>(_timer_list), 
                                   tag, 
                                   usec / USECS_PER_SEC, 
                                   usec % USECS_PER_SEC);
    ptm->_func = cb;
 
#if USE_TIMER_REAL
    evtimer_set(&ptm->evt, event_timer_handler, ptm);
    TimerReal::AddTimer(&ptm->evt);
#else
    // timer will processed in timer_list
#endif

    pt = (PTimer)ptm;
}

bool TimerEvent::StartTimer(PTimer pt, POwner po)
{
    PTIMER_ITEM ptm = static_cast<PTIMER_ITEM>(pt);
    ptm->_user = po;

#if USE_TIMER_REAL
    return 0 == evtimer_add(&ptm->evt, &ptm->tvdef);
#else
    return true;
#endif
}

bool TimerEvent::RestartTimer(PTimer pt, POwner po)
{
#if USE_TIMER_REAL
    StopTimer(pt);
    return StartTimer(pt, po);
#else
    return true;    
#endif
}

bool TimerEvent::StopTimer(PTimer pt)
{
    PTIMER_ITEM ptm = static_cast<PTIMER_ITEM>(pt);
#if USE_TIMER_REAL
    return 0 == evtimer_del(&ptm->evt);
#else
    return true;
#endif
}

void TimerEvent::ProcessTimers()
{
#if USE_TIMER_REAL
    TimerReal::ProcessThread();
#endif    
    // TRACE_THREAD(7, "TimerEvent::ProcessTimers");
}

#if !USE_TIMER_REAL

static int proc_timers(void *);

/** Re-order timer items and set the recent timeout one
 */
static long _reschedule_timers(TPTimeInterval now)
{
    TTimeInterval next;
    PTIMER_ITEM  ptm;

    // calulate the upcoming next timeout
    next = max_time;
    for (ptm = l_timer_head; ptm != NULL; ptm = ptm->_next) {
        TPTimeInterval ttm = &ptm->tmval;
        if (ttm->tv_sec && ttm->tv_usec) {
            if (compare_time(ttm, now) < 0) {
                LOGFILE(LOG_WARN, "unhandled time #%d", ptm->tag);
                ptm->tmval = zero_time;
            } else if (compare_time(ttm, &next) < 0) {
                next = *ttm;
            }
        } // if (tm)
    } // for (i)

    dec_time(&next, now);
    if (next.tv_sec > 0 || (next.tv_sec == 0 && next.tv_usec > 0)) {
#if USE_SIG_ALARM
        struct itimerval ival, oval;
        ival.it_value    = next;
        ival.it_interval = zero_time;
        (void)setitimer(ITIMER_REAL, &ival, &oval);
#else /*! USE_SIG_ALARM */
        TimerReal::Instance().StartOnce(next, (TimerReal::TimeoutCallback)proc_timers, _instance);
#endif /* USE_SIG_ALARM */
    }
    
    return (next.tv_sec * 1000) + (next.tv_usec / 1000);
}

static long start_timer(PTIMER_ITEM ptm)
{
    assert(NULL != ptm);
    if (compare_time(&ptm->tmdef, (TPTimeInterval)&zero_time) > 0) {
        TTimeInterval now;

#ifdef USE_SIG_ALARM
        struct itimerval ival, oval;
        // clear pending timers
        ival.it_value    = zero_time;
        ival.it_interval = zero_time;
        (void)setitimer(ITIMER_REAL, &ival, &oval);
#else /* !USE_SIG_ALARM */
        TimerReal::Instance().StopTimer(0L);
#endif /* USE_SIG_ALARM */

        // update next timeout value
        get_current_time(&now);
        
#ifndef NO_TRACE
        if (compare_time(&ptm->tmval, &now) > 0) {
            struct tm *pltm = localtime(&now.tv_sec);
            TRACE(6, "timer #%d restarted at %02d:%02d:%02d.%03d",
                  ptm->tag, pltm->tm_hour, pltm->tm_min, pltm->tm_sec,
                  now.tv_usec / 1000);
        } else {
            struct tm *pltm = localtime(&now.tv_sec);
            TRACE(6, "timer #%d started at %02d:%02d:%02d.%03d",
                  ptm->tag, pltm->tm_hour, pltm->tm_min, pltm->tm_sec,
                  now.tv_usec / 1000);
        }
#endif // NO_TRACE

        ptm->tmval = ptm->tmdef;
        (void)inc_time(&ptm->tmval, &now);
        return _reschedule_timers(&now);
    }    
    
    return 0;
}

static long restart_timer(PTIMER_ITEM ptm)
{
    assert(NULL != ptm);
    if (compare_time(&ptm->tmval, (TPTimeInterval)&zero_time) > 0) {
        TRACE(6, "timer #%d restarted", ptm->tag);
        return start_timer(ptm);
    }
    return 0;
}

static void stop_timer(PTIMER_ITEM ptm)
{
    if (compare_time(&ptm->tmval, (TPTimeInterval)&zero_time) > 0) {
        TRACE(6, "timer #%d stopped", ptm->tag);
        ptm->tmval = zero_time;
    }    
    return;
}

static int proc_timers(void *)
{
    PTIMER_ITEM   tmset[USER_TIMER_MAX];
    PTIMER_ITEM   ptm;
    TTimeInterval now;
    size_t i, tmcnt = 0;
    int result = 0;

    get_current_time(&now);

    for (ptm = l_timer_head; ptm != NULL; ptm = ptm->_next) {        
        if (compare_time(&ptm->tmval, (TPTimeInterval)&zero_time) > 0 &&
            compare_time(&ptm->tmval, &now) <= 0)
        {
            // timer is set and overdue
            tmset[tmcnt++] = ptm;
            ptm->tmval = zero_time;
        }
    } // for (ptm)
    
    for (i = 0; i < tmcnt; i++) {
        ptm = tmset[i];
#ifndef NO_TRACE
        struct tm *pltm = localtime(&now.tv_sec);
        TRACE(6, "timer #%d timeout at %02d:%02d:%02d.%03d",
              ptm->tag,
              pltm->tm_hour, pltm->tm_min, pltm->tm_sec, now.tv_usec / 1000);
#endif
        if (NULL != ptm->_func) {
            (*ptm->_func)(ptm->tag, ptm->user);
            result++;
        }
    } // for (i < tmcnt)

    (void)_reschedule_timers(&now);

    return result;
}

#endif /* USE_TIMER_REAL */


