/*
 * $Id: Trace.h,v 1.3 2005/03/15 03:09:41 xiaofeng Exp $
 *
 * unit-debug trace support
 *
 * Copyright (c) 2004-2006. All rights reserved.
 *
 * $Log: Trace.h,v $
 *
 */
#ifndef _UTILS_TRACE_H_
#define _UTILS_TRACE_H_

/** @file Trace.h Trace function for debug and unit test.
 * 
 * TRACE() is print to stdout, for unit debugging only, will be ignored in
 * release version (when NO_TRACE is defined).
 *
 * TRACE_THREAD() is similar to TRACE(), the difference is that TRACE_THREAD
 * can work well with thread (also print out the thread id).
 *
 * The trace level is customized by applications, the recommended trace level
 * is defined as following:
 * <pre>
 *  1:  Main task general event
 *  2:  Process or task critical event (error)
 *  3:  Queue / FIFO / Message
 *  4:  I/O access event
 *  5:  Thread/mutex/semaphore create/destroy event
 *  6:  Encoder/Decoder events
 *  7:  Internal state machine
 *  8:  sub-state machine (DFA) states
 *  9:  Memory management
 * </pre>
 */

#include <string.h>

// #include "osdeps/osutil.h"
#include "Base.h"

#ifndef SUPPORT_MACRO_VARGS
# if defined(__GNUC__)
#  define SUPPORT_MACRO_VARGS   1
# elif defined(WIN32)
#  define SUPPORT_MACRO_VARGS   0
# else
#  define SUPPORT_MACRO_VARGS   2
# endif
#endif

#ifndef TRACE
# ifndef NO_TRACE
#  if (SUPPORT_MACRO_VARGS == 1)
#   define TRACE(l, s, a...)           do { if (l <= g_nTraceLevel)           \
                                           _trace_impl(l, s, ##a);            \
                                       } while (0)
#   define TRACE_THREAD(l, s, a...)    do { if (l <= g_nTraceLevel)           \
                                           _trace_impl_thread(l, s, ##a);     \
                                       } while (0)
#  elif (SUPPORT_MACRO_VARGS == 2)
#   define TRACE(l, ...)               do { if (l <= g_nTraceLevel)           \
                                           _trace_impl(l, __VA_ARGS__);       \
                                       } while (0)
#   define TRACE_THREAD(l, ...)        do { if (l <= g_nTraceLevel)           \
                                           _trace_impl_thread(l, __VA_ARGS__);\
                                       } while (0)
#  elif (SUPPORT_MACRO_VARGS == 0)
#   define TRACE_USE_INLINE
#   include <stdarg.h>
#   define TRACE                       _trace_impl_inline
#   define TRACE_THREAD                _trace_impl_thread
#  endif /* SUPPORT_MACRO_VARGS */
#  define TRACE_IF(lev, cmd)           do { if (lev <= g_nTraceLevel) cmd;    \
                                       } while (0)
# else  /* NO_TRACE */
#  if (SUPPORT_MACRO_VARGS != 0)
#   define TRACE(lev, ...)
#   define TRACE_THREAD(lev, ...)
#  else
#    define TRACE
#    define TRACE_THREAD
#  endif
#  define TRACE_IF(lev, cmd)
# endif /* NO_TRACE */
#endif /* TRACE */

// bypass warnings in VS 8.0 for strncpy()
#if (_MSC_VER > 1200)
#pragma warning(disable:4996)
#endif



BEGIN_EXTERN_C

extern int  g_nTraceLevel;
extern BOOL g_bTraceTimestamp;
extern char g_aTracePrefix[16];

/** Get the current trace level.
 * @return the current trace level
 */
INLINE int get_trace_level(void)
{
    return g_nTraceLevel;
}

/** Set the current trace level.
 * @param l trace level.
 * @param print_time print time (e.g. '113725.709')
 * @return none.
 */
INLINE void set_trace_level(int l, BOOL print_time)
{
    g_nTraceLevel = l;
    g_bTraceTimestamp = print_time;
}

/** Set trace prefix
 * Add a string in each output line (just after time if enabled).
 * @param prefix prefix string.
 * @return none.
 */
INLINE void set_trace_prefix(const char *prefix)
{
    if (NULL != prefix) {
        strncpy(g_aTracePrefix, prefix, sizeof(g_aTracePrefix) - 1);
    }
}

/** Internal function implemented trace.
 */
void _trace_impl(int lev, const char * fmt, ...);

#ifdef TRACE_USE_PTHREAD
void _trace_impl_thread(int lev, const char * fmt, ...);
#else
# undef  TRACE_THREAD
# define TRACE_THREAD TRACE
#endif

/** An inline version of _trace_impl
 */
#ifdef TRACE_USE_INLINE
void _trace_impl_vargs(int lev, const char * fmt, va_list parg);
_inline void _trace_impl_inline(int lev, const char * fmt, ...)
{
    if (lev <= g_nTraceLevel) {
        va_list argList;
        va_start(argList, fmt);
        _trace_impl_vargs(lev, fmt, argList);
        va_end(argList);
    }
}
#endif /* LOG_USE_INLINE */

END_EXTERN_C

#endif /* _UTILS_TRACE_H_ */
