/*
 * $Id: $
 *
 * TimerEvent is a event timer for state-machine
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file TimerEvent.h Event Timer */
#include "Base.h"

const long USECS_PER_SEC = 1000000L;

/**
 * Event timer for state-machine
 */
class TimerEvent
{
public:
    typedef void *PTimer;
    typedef void *POwner;
    typedef void (*TimeoutCallback)(int tag, POwner);

private:
    /** Timer list header */
    PTimer _timer_list;

protected:
    /** Hiden constructor
     */
    TimerEvent() : _timer_list(NULL) {}

public:
    /**
     * Get a thread-local instance
     * [thread-local]
     */
    static TimerEvent& Instance();

    /**
     * Call by a single thread to process all timers
     * Timeout callback function is called in this thread context
     * [thread]
     */
    static void ProcessTimers();

    /**
     * Initialize timer
     * @param pt timer instance
     * @param tag timer tag
     * @param cb callback when timer is timeout
     * @param usec time
     */
    void InitTimer(PTimer &pt, int tag, TimeoutCallback cb, long usec);

    /**
     * Start a timer, NOTHING happens if a timer is already started
     * @param pt timer instance
     * @param po owner of this timer
     * @return timer is successful started
     */
    bool StartTimer(PTimer pt, POwner po);

    /**
     * Restart a timer
     * @param pt timer instance
     * @param po owner of this timer
     * @return timer is successful re-started
     */
    bool RestartTimer(PTimer pt, POwner po);

    /**
     * Stop a timer
     * @return timer is successful stopped
     */
    bool StopTimer(PTimer pt);

    /**
     * Desctructor
     */ 
    ~TimerEvent();

};