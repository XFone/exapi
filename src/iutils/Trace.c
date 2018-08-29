/*
 * $Id: Trace.c,v 1.3 2005/03/15 03:04:46 xiaofeng Exp $
 *
 * Debug tracing support
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

#include "Trace.h"
#include "DateTime.h"
#include "osdeps/osutil.h"

#ifdef TRACE_USE_PTHREAD
# include "thread.h"
#endif

int  g_nTraceLevel = 0;
BOOL g_bTraceTimestamp = FALSE;
char g_aTracePrefix[16] = "";

#ifndef WIN32
void _trace_impl(int lev, const char *fmt, ...)
{
    FILE *fConsole = stdout;
    char buf[DATEBUF_SIZE];
    va_list ap;

    va_start(ap, fmt);
    if (g_bTraceTimestamp) {        
        get_curtime_s(buf);
        fprintf(fConsole, "%s %s[%u]: ", buf, g_aTracePrefix, lev);
    } else {        
        fprintf(fConsole, "%s[%u]: ", g_aTracePrefix, lev);
    }
    vfprintf(fConsole, fmt, ap);    
    fprintf(fConsole, "\n");
    fflush(fConsole);
    va_end(ap);
}

#else /* WIN32 */
void _trace_impl_vargs(int lev, const char *fmt, va_list ap)
{
    FILE *fConsole = stdout;
    char buf[DATEBUF_SIZE];

    if (g_bTraceTimestamp) {        
        get_curtime_s(buf);
        fprintf(fConsole, "%s %s[%u]: ", buf, g_aTracePrefix, lev);
    } else {        
        fprintf(fConsole, "%s[%u]: ", g_aTracePrefix, lev);
    }
    vfprintf(fConsole, fmt, ap);    
    fprintf(fConsole, "\n");
    fflush(fConsole);
}

#endif /* !WIN32 */

#ifdef TRACE_USE_PTHREAD
void _trace_impl_thread(int lev, const char *fmt, ...)
{
    FILE *fConsole = stdout;
    char buf[DATEBUF_SIZE];
    va_list ap = NULL;

    va_start(ap, fmt);
    if (g_bTraceTimestamp) {        
        get_curtime_s(buf);
        fprintf(fConsole, "%s %s.%8x[%u]: ", buf, g_aTracePrefix,
                (unsigned int)get_current_thread_id(), lev);
    } else {
        fprintf(fConsole, "%s.%8x[%u]: ", g_aTracePrefix,
                (unsigned int)get_current_thread_id(), lev);
    }

    vfprintf(fConsole, fmt, ap);    
    fprintf(fConsole, "\n");
    fflush(fConsole);
    va_end(ap);
}
#endif /* TRACE_USE_PTHREAD */
