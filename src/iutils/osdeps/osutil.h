/*
 * $Id: osutil.h,v 1.3 2005/02/26 02:38:22 xiaofeng Exp $
 *
 * Operating System routines implementation
 *
 * Copyright (c) 2004-2006. All rights reserved.
 *
 * $Log: osutil.h,v $
 *
 */

#ifndef _UTILS_OSUTIL_H_
#define _UTILS_OSUTIL_H_

#if !(_WIN32 || _WIN64)
# include <signal.h>
#else
# define siginfo_t void
#endif

/** @file osutil.h Common System Routines */

#ifndef CHECK
# define CHECK(v, log)         if (!(v)) _check_func(__FILE__, __LINE__, (log))
#endif

#ifndef _assert_PTHREAD
#define _assert_PTHREAD(func, args) \
    { \
        unsigned threadOpRetry = 0; \
        while (_assert_thread_op(func args, &threadOpRetry,\
               #func, __FILE__, __LINE__)); \
    }
#endif /* _assert_PTHREAD */



BEGIN_EXTERN_C

BOOL _assert_thread_op(int retval, unsigned *retry,
                       const char *func, const char *file, unsigned line);



void _check_func(const char * file, int line, const char * msg);

/**
 * Catch sigfault and printout
 */
void catch_segfault(void (*handler)(int, siginfo_t *, void *));

END_EXTERN_C

#endif /* _UTILS_OSUTIL_H_ */
