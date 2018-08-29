/*
 * $Id: DateTime.c,v 1.2 2005/03/01 02:32:06 xiaofeng Exp $
 *
 * Implemention of time and date routines
 *
 * Copyright (c) 2004-2006. All rights reserved.
 *
 * $Log: DateTime.c,v $
 *
 */

#include "Base.h"
#include "DateTime.h"
#include "TimeType.h"

#include <stdio.h>
#include <time.h>

// bypass warnings in VS 8.0 
#if (_MSC_VER > 1200)
#pragma warning(disable:4996)
#endif

void get_curtime_l(char * sTime)
{
#ifdef WIN32
    SYSTEMTIME tm;
    GetLocalTime(&tm);
    sprintf(sTime, "%4d%02d%02d-%02d%02d%02d.%3.3ld",
            tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, 
            tm.wSecond, tm.wMilliseconds);
#else
    struct timeval tv;
    struct tm * ptm, stm = { 0 };

    gettimeofday(&tv, NULL);
    ptm = localtime_r((const time_t *)&(tv.tv_sec), &stm);

    sprintf(sTime, "%4d%02d%02d-%02d%02d%02d.%3.3ld",
            ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,
            ptm->tm_hour, ptm->tm_min, ptm->tm_sec, tv.tv_usec / 1000L);
#endif
}

void get_curtime_s(char * sTime)
{
#ifdef WIN32
    SYSTEMTIME tm;
    GetLocalTime(&tm);
    sprintf(sTime, "%02d%02d%02d.%3.3ld",
            tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
#else
    struct timeval tv;
    struct tm *ptm, stm = { 0 };

    gettimeofday(&tv, NULL);
    ptm = localtime_r((const time_t *)&(tv.tv_sec), &stm);

    sprintf(sTime, "%02d%02d%02d.%3.3ld",
            ptm->tm_hour, ptm->tm_min, ptm->tm_sec, tv.tv_usec / 1000L);
#endif
}

char *strtimeval_s(char *buf, size_t max_len, struct timeval ts)
{
#ifdef WIN32
    // TODO
    assert(0);
#else
    struct tm * ptm, stm = { 0 };
    ptm = localtime_r((const time_t *)&(ts.tv_sec), &stm);
    snprintf(buf, max_len, "%02d%02d%02d.%03d", ptm->tm_hour, ptm->tm_min, 
             ptm->tm_sec, (int)(ts.tv_usec / 1000L));
#endif
    return buf;
}

void get_curtime_f(char * sTime)
{
    time_t t;
    struct tm * tm_buf, stm = { 0 };
    t = time(0);
    tm_buf = localtime_r(&t, &stm);
    sprintf(sTime, "%04d-%02d-%02d %02d:%02d:%02d",
            tm_buf->tm_year + 1900,
            tm_buf->tm_mon + 1,
            tm_buf->tm_mday,
            tm_buf->tm_hour,
            tm_buf->tm_min,
            tm_buf->tm_sec);
}

void get_curdate_s(char * szDate)
{
    time_t t;
    struct tm * tm_buf, stm = { 0 };
    t = time(0);
    tm_buf = localtime_r(&t, &stm);
    sprintf(szDate, "%04d%02d%02d",
            tm_buf->tm_year + 1900,
            tm_buf->tm_mon + 1,
            tm_buf->tm_mday);
}

