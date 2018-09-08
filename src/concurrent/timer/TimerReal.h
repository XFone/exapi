/*
 * $Id: $
 *
 * TimerReal is a realtime timer for filters and queues
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file TimerReal.h Realtime Timer */
#include "Base.h"
#include "TimeType.h"

#include "fifo/IQueue.h"

#ifdef USE_TBB
#include <tbb/tick_count.h>
typedef tbb::tick_count ticks_t;
#else
typedef long long ticks_t;
#endif

typedef long usecs_t;
typedef long msecs_t;

#if !(_WIN32 || _WIN64)
typedef union _LARGE_INTEGER {
    struct { 
        uint32_t    LowPart;
        int32_t     HighPart;
    }; 
    int64_t     QuadPart; 
} LARGE_INTEGER;
#endif

/**
 * Realtime timer is tiggered by datetime or periodically
 */
class TimerReal
{
public:
    typedef void *POwner;
    typedef void (*TimeoutCallback)(int tag, POwner);

    static const ticks_t m_start;

protected:
   /**
    * Call by worker (threads)
    * [thread]
    */
   int  ProcessTimers();

public:

    /**
     * Ticks to seconds
     */
    static double Ticks2Secs(ticks_t ticks);

    /**
     * Seconds to ticks
     */
    static ticks_t Secs2Ticks(double secs);

    /**
     * Get current time-stamp in us (realtime-clock)
     * Windows: use performance counter retrieves from QueryPerformanceCounter
     * Linux/Unix: The time is expressed in microseconds (us) since 1970/1/1 00:00:00
     */
    static void GetClock(LARGE_INTEGER &qpcnt);

    /**
     * Get current datetime in msecs as format of :
     * HighPart: date in format of '20141120'
     * LowPart:  time in format of '142345231' (as 14:23:45.231)
     */
    static void GetDateTime(LARGE_INTEGER &qpcnt);

    /**
     * Current microseconds (us) offset to specific time (e.g., 1970/1/1 00:00:00)
     * chrono::steady_clock::now().time_since_epoch()
     */
    static int64_t GetUsecsOffset();

    /**
     * Current usecs (MAX 2147 seconds) offset to every 15 minutes
     */
    static usecs_t GetUsecsShort() {
        return (usecs_t)GetUsecsOffset(); // lose high part
    }

    /**
     * Current milli-seconds offset to midnight (00:00:00)
     */
    static msecs_t GetMsecs();

    /**
     * Current seconds offset to midnight (00:00:00)
     */
    static time_t GetSecs() {
        return GetMsecs() / 1000L;
    }

    /**
     * Current ticks from power-on
     */
    static ticks_t GetTicks();

    /**
     * Get a thread-local instance
     * [thread-local]
     */
    static TimerReal& Instance();

    void StartOnce(TTimeInterval tm, TimeoutCallback cb, POwner po);

    /**
     * Start a timer with an interval of usecs
     * @param cb callback when timer is tiggered
     * @param po owner of this timer
     * @return timer is successful started
     */
    bool StartTimer(usecs_t usecs, TimeoutCallback cb, POwner po);

    /**
     * Stop a timer with an interval of usecs
     * @return timer is successful stopped
     */
    bool StopTimer(usecs_t usecs);

    /* --- TO REMOVE -- */
    static void ProcessThread();
    
    static void AddTimer(void *event);
};
