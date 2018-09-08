/*
 * $Id: $
 *
 * Realtime timer implementation
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
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

// using C++11 chrono
#if __cplusplus >= 201103L
#include <chrono>
#define USE_CHRONO
#endif

#if _WIN32 || _WIN64
//#include "machine/windows_api.h"
#elif __linux__
#include <ctime>
#else /* generic Unix */
#include <sys/time.h>
#endif /* (choice of OS) */

//#define TIMER_INTERVAL


#if defined(USE_TBB)
const ticks_t TimerReal::m_start = tbb::tick_count::now();
#else 
# if defined(USE_CHRONO)
static auto _m_clock_start = std::chrono::steady_clock::now();
# endif
const ticks_t TimerReal::m_start = TimerReal::GetUsecsOffset();
#endif

// thread_local variable event_base is thread-unsafe
static _THREAD_LOCAL_ struct event_base *_tl_ebase = NULL;
extern int g_is_exiting;

using namespace ATS;

double TimerReal::Ticks2Secs(ticks_t ticks)
{
#if defined(USE_TBB) // tbb::tick_count
    return (ticks - m_start).seconds();
#elif defined(USE_CHRONO) // ticks is same to usecs
    return (double)ticks / 1000000L;
#else
    // TODO
    return 0L;
#endif
}

/* CAN NOT IMPLEMENT
ticks_t TimerReal::Secs2Ticks(double secs)
{
#if defined(USE_TBB) // tbb::tick_count
    tbb::tick_count::interval_t it(secs);
    return tbb::tick_count(it);
#elif defined(USE_CHRONO) // ticks is same to usecs
    return (ticks_t)(secs * 1000000L);
#else
    // TODO
    return 0L;
#endif
}
*/

void TimerReal::GetClock(LARGE_INTEGER &qpcnt)
{
#if defined(USE_CHRONO)
    // qpcnt.QuadPart = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - _m_clock_start).count();
    return;
#endif

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

int64_t TimerReal::GetUsecsOffset()
{
    int64_t result;
    
#if defined(USE_CHRONO)
    result = std::chrono::steady_clock::now().time_since_epoch().count();
#elif _WIN32 || _WIN64
    /* CAN NOT IMPLEMENT
    SYSTEMTIME tm;
    GetLocalTime(&tm);
    result += static_cast<int64_t>(tm.wHour * 3600L) * 1000000L + 
              static_cast<int64_t>(tm.wMinute * 60L) * 1000000L +
              static_cast<int64_t>(tm.wSecond) * 1000000L + 
              static_cast<int64_t>(tm.wMilliseconds) * 1000L;
    */
    assert(0);
#elif __linux__
    struct timespec ts;
    (void)clock_gettime(CLOCK_REALTIME, &ts);
    result = static_cast<int64_t>(1000000) * static_cast<int64_t>(ts.tv_sec) + static_cast<int64_t>(ts.tv_nsec) / 1000;
#else /* generic Unix */
    struct timeval tv;
    (void)gettimeofday(&tv, NULL);
    result = static_cast<int64_t>(1000000) * static_cast<int64_t>(tv.tv_sec) + static_cast<int64_t>(tv.tv_usec);
#endif /*(choice of OS) */

    return result;
}

msecs_t TimerReal::GetMsecs()
{
    msecs_t result;

#if _WIN32 || _WIN64
    SYSTEMTIME tm;
    GetLocalTime(&tm);
    msecs_t seconds = tm.wHour * 3600L + tm.wMinute * 60L + tm.wSecond;
    result = seconds * 1000L + tm.wMilliseconds;
#else
    struct timeval tv;
    struct tm * ptm;

    gettimeofday(&tv, NULL);
    ptm = localtime((const time_t *)&(tv.tv_sec));

    msecs_t seconds = ptm->tm_hour * 3600L + ptm->tm_min * 60L + ptm->tm_sec;
    result = seconds * 1000L + tv.tv_usec / 1000L;
#endif

    return result;
}

ticks_t TimerReal::GetTicks()
{
#if defined(USE_TBB) // tbb::tick_count
    return tbb::tick_count::now();
#else // long long
    LARGE_INTEGER qpcnt;
    GetClock(qpcnt);
    return qpcnt.QuadPart;
#endif
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
