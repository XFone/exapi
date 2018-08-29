/*
 * $Id: timetype.h,v 1.2 2005/02/26 02:38:22 xiaofeng Exp $
 *
 * Time and Date routines
 *
 * Copyright (c) 2004-2008. All rights reserved.
 *
 * $Log: timetype.h,v $
 *
 */

#ifndef _UTILS_TIMETYPE_H_
#define _UTILS_TIMETYPE_H_

/** @file timetype.h Time and Date Routines */

/* Header for struct timeval */
#ifdef WIN32
# include <WinSock2.h>
//# include <wtypes.h>
# define _SEC             dwHighDateTime
# define _USEC            dwLowDateTime
typedef struct _FILETIME TTimeInterval, *TPTimeInterval;
#else
# include <sys/time.h>
# define _SEC             tv_sec
# define _USEC            tv_usec
typedef struct timeval TTimeInterval, *TPTimeInterval;
#endif

#define MAX_TIME_INTERVAL 0x7FFFFFFFL
#define MIN_TIME_INTERVAL 0x0000001FL

BEGIN_EXTERN_C

/** Time Interval */

extern TTimeInterval gMinTimeInterval;

/** Get current time 
 * @param val to save the result
 */
INLINE void get_current_time(TPTimeInterval tv)
{
    assert(tv != NULL);
#ifdef WIN32    
    GetSystemTimeAsFileTime(tv);
#else    
    gettimeofday(tv, NULL);
#endif
}


/** Initialize a time interval type with milli-seconds (ms)
 * @param l left value
 * @param msec millisecond
 * @return l
 */
TPTimeInterval INLINE init_time_ms(TPTimeInterval l, long msec)
{
    l->_SEC  = msec / 1000;
    l->_USEC = (msec % 1000) * 1000;
    return l;
}


/** Increment time l by r
 * @param l left value
 * @param r right value (delta to increment)
 * @return l
 */
TPTimeInterval inc_time(TPTimeInterval l, TPTimeInterval r);

/** Decrement time l by r
 * @param l left value
 * @param r right value (delta to decrement)
 * @return l
 */
TPTimeInterval dec_time(TPTimeInterval l, TPTimeInterval r);

/** Compare two time value
 * @param l first time
 * @param r second time
 * @return check the result when:
 *         - =0: equre
 *         - >0: l is later than r
 *         - <0: l is earlier than r 
 */
int compare_time(TPTimeInterval l, TPTimeInterval r);

END_EXTERN_C

#endif /* _UTILS_TIMETYPE_H_ */
