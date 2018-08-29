/*
 * $Id: $
 *
 * Realtime timer implementation
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */

#include "Base.h"
#include "Trace.h"
#include "Log.h"

#include "TimerReal.h"

#include <event2/event.h>
#include <event2/event_compat.h>
#include <event2/event_struct.h>

#if _WIN32 || _WIN64
//#include "machine/windows_api.h"
#elif __linux__
#include <ctime>
#else /* generic Unix */
#include <sys/time.h>
#endif /* (choice of OS) */

//#define TIMER_INTERVAL

// thread_local variable event_base is thread-unsafe
static _THREAD_LOCAL_ struct event_base *_tl_ebase = NULL;

extern int g_is_exiting;

using namespace ATS;

usecs_t TimerReal::Ticks2USecs(ticks_t ticks)
{
    // TODO
    return 0L;
}

ticks_t TimerReal::Usecs2Ticks(usecs_t usecs)
{
    ticks_t ticks;
    // TODO
    return ticks;
}

void TimerReal::GetClock(LARGE_INTEGER &qpcnt)
{
    // ticks_t now = tbb::now();
#if _WIN32 || _WIN64
    int rval = QueryPerformanceCounter(&qpcnt);
#elif __linux__
    struct timespec ts;
    (void)clock_gettime(CLOCK_REALTIME, &ts);
    qpcnt.QuadPart = static_cast<int64_t>(1000000) * static_cast<int64_t>(ts.tv_sec) + static_cast<int64_t>(ts.tv_nsec) / 1000;
#else /* generic Unix */
    struct timeval tv;
    (void)gettimeofday(&tv, NULL);
    qpcnt.QuadPart = static_cast<int64_t>(1000000) * static_cast<int64_t>(tv.tv_sec) + static_cast<int64_t>(tv.tv_usec);
#endif /*(choice of OS) */
}


void TimerReal::GetDateTime(LARGE_INTEGER &qpcnt)
{
#if _WIN32 || _WIN64
    SYSTEMTIME tm;
    GetLocalTime(&tm);
    // date in format of '20141120'
    qpcnt.HighPart = tm.wYear * 10000L + tm.wMonth * 100 + tm.wDay;
    // time in format of '142345231'
    qpcnt.LowPart  = tm.wHour * 10000000L + tm.wMinute * 100000L + tm.wSecond * 1000L + tm.wMilliseconds;
#else
    struct timeval tv;
    struct tm * ptm;

    gettimeofday(&tv, NULL);
    ptm = localtime((const time_t *)&(tv.tv_sec));

    // date in format of '20141120'
    qpcnt.HighPart = (ptm->tm_year + 1900) * 10000L + (ptm->tm_mon + 1) * 100 + ptm->tm_mday;
    // time in format of '142345231'
    qpcnt.LowPart  = ptm->tm_hour * 10000000L + ptm->tm_min * 100000L + ptm->tm_sec * 1000L + (tv.tv_usec / 1000L);
#endif
}

msecs_t TimerReal::GetMsecs() 
{
    msecs_t result;

#if _WIN32 || _WIN64
    SYSTEMTIME tm;
    GetLocalTime(&tm);
    result = tm.wHour * 10000000L + tm.wMinute * 100000L + tm.wSecond * 1000L + tm.wMilliseconds;
#else
    struct timeval tv;
    struct tm * ptm;

    gettimeofday(&tv, NULL);
    ptm = localtime((const time_t *)&(tv.tv_sec));
    result  = ptm->tm_hour * 10000000L + ptm->tm_min * 100000L + ptm->tm_sec * 1000L + (tv.tv_usec / 1000L);
#endif

    return result;      
}

static void _timer_handler(const int fd, const short which, void *arg) {
    // TRACE_THREAD(7, "timer_handler");
}

int TimerReal::ProcessTimers()
{
    struct event time_event; // for debugging

    if (NULL == _tl_ebase) {
        _tl_ebase = event_init();
    }

    evtimer_set(&time_event, _timer_handler, (void *)NULL);
    event_base_set(_tl_ebase, &time_event);

    // TODO
    do {
        int ret;
        ret = event_base_loop(_tl_ebase, EVLOOP_ONCE);

        if (1 == ret) { // add a 1-sec timeout when NO event was set
            struct timeval t;
            t.tv_sec  = 1;
            t.tv_usec = 0;

            if (evtimer_add(&time_event, &t) == -1) {
                LOGFILE(LOG_ERROR, "failed in TimerReal::evtimer_add");
            }            
        } else {
            // TRACE_THREAD(7, "event_base_loop@%x returns %d", _tl_ebase, ret);   
        }

    } while (!g_is_exiting);

    return 0;
}

// ----------------------------------------------------------------------------
// TODO: remove this URGLY code
INLINE void _init_base_in_thread()
{
    if (NULL == _tl_ebase) {
        _tl_ebase = event_init();
        TRACE_THREAD(7, "initialized event_base@%x", _tl_ebase);   
    }
}

void TimerReal::ProcessThread()
{
    int ret;
    _init_base_in_thread();
    ret = event_base_loop(_tl_ebase, EVLOOP_NONBLOCK); // non-block
    if (1 != ret) {
        // TRACE_THREAD(7, "event_base_loop@%x returns %d", _tl_ebase, ret);   
    }
}

void TimerReal::AddTimer(void *event)
{
    assert(NULL != _tl_ebase);
    (void)event_base_set(_tl_ebase, (struct event *)event);
    TRACE_THREAD(7, "added timer in event_base@%x", _tl_ebase);   
}
