/*
 * $Id: osutil.c,v 1.3 2005/03/15 03:04:46 xiaofeng Exp $
 *
 * Operating System routines implementation
 *
 * Copyright (c) 2004-2006. All rights reserved.
 *
 * $Log: osutil.c,v $
 *
 */

#include <stdio.h>
#include <stdarg.h>
#ifdef WIN32
#else
# include <unistd.h>
#endif

#include "../Base.h"
#include "../Log.h"
#include "../DateTime.h"
#include "osutil.h"

#ifdef USE_PTHREAD
# include "thread.h"
#endif

BOOL _assert_thread_op(int retval, unsigned *retry,
                       const char *func, const char *file, unsigned line)
{
#ifdef USE_PTHREAD
    if (retval == 0) {
        return FALSE;
    }

    if (errno == EINTR || errno == EAGAIN)  {
        if (++*retry < 1000) {
            usleep(10);
            return TRUE;   // Return value to try again
        }
    }
#endif
    return FALSE;
}

void _check_func(const char * file, int line, const char * msg)
{
    int err = errno;
    _log_impl1(0, file, line, msg);
    
    if (NULL == msg)
        msg = "?";
    fprintf(stderr, "Assertion fail: %s(%s:%u), error= %d",
            msg, file, line, err);
    exit(err);
}

