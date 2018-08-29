/*
 * $Id: Log.c,v 1.5 2005/03/10 16:00:19 xiaofeng Exp $
 *
 * System logging support
 *
 * Copyright (c) 2004-2006. All rights reserved.
 *
 * $Log: Log.c,v $
 *
 */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifndef WIN32
# include <syslog.h>
# include <unistd.h>
#else
# define STDERR_FILENO    2
# define vsnprintf        _vsnprintf
# define snprintf         _snprintf
#endif /* !WIN32 */

#include "Base.h"
#include "Log.h"
#include "DateTime.h"

#ifdef USE_PTHREAD
# include "Thread.h"
static TThreadId l_tInitThread = 0;
#else
#endif

// bypass warnings in VS 8.0 
#if (_MSC_VER > 1200)
#pragma warning(disable:4996)
#endif

#define make_syslog_time(buf)   get_curtime_s(buf)

enum ELogMethod g_nLogMethod = LOGM_FILE;
int  g_nLogPriority = LOG_ERROR;

static FILE * l_fLogFile = NULL;
static LOG_CALLBACK_F l_fLogFunc = NULL;

const char * aPriNameLong[] = {
    "EMERG",
    "ALERT",
    "CRIT",
    "ERR",
    "WARN",
    "NOTICE",
    "INFO",
    "DEBUG",
    "STACK",
    0
};

const char * aPriNameShort[] = {
    "EMRG",
    "ALRT",
    "CRIT",
    "ERR ",
    "WARN",
    "NOTC",
    "INFO",
    "DBUG",
    "DSTK",
    "DOP ",
    "DHB ",
    0
};

static const char *
_get_thread_str(void)
{
    // get_current_thread_id();
    return "";
}


void _log_impl1_vargs(int pri, const char *file, int line,  
                      const char * fmt, va_list ap)
{
    char aDateBuf[DATEBUF_SIZE];
    char buf[1024];
    int  i;
    switch (g_nLogMethod) {
    case LOGM_FILE:
        get_curtime_l(aDateBuf);
    
        if (NULL == l_fLogFile)
            l_fLogFile = fdopen(STDERR_FILENO, "w");

        i = snprintf(buf, sizeof(buf) - 1, "%s %s%s%s: %s:%d: ",
                     aDateBuf, _get_thread_str(),
                     aPriNameShort[pri], "", file, line);
        vsnprintf(buf + i, sizeof(buf) - 1 - i, fmt, ap);
        fprintf(l_fLogFile, "%s\n", buf);
        fflush(l_fLogFile);
        
        /* in case we just pushed the current file past the size limit... */
        // rotateFilesIfNecessary();
        break;

    case LOGM_SYSLOG:
#ifdef WIN32
#else /* !WIN32 */
        make_syslog_time(aDateBuf);
        
        vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
        syslog((pri <= LOG_DEBUG) ? pri : LOG_DEBUG,
               "%s %s%s%s: %s:%d: %s\n",
               aDateBuf, _get_thread_str(),
               aPriNameShort[pri], "", file, line, buf);
#endif /* !WIN32 */
        break;

    case LOGM_CALLBACK:
        if (NULL != l_fLogFunc) {
            get_curtime_l(aDateBuf);
            i = sprintf(buf, "%s %s%s%s: %s:%d: ", 
                        aDateBuf, _get_thread_str(),
                        aPriNameShort[pri], "", file, line);
            vsnprintf(&buf[i], sizeof(buf) - i, fmt, ap);

            (*l_fLogFunc)(pri, buf);
        }
        break;

    default:
        assert(0);
        break;
    }
}

void _log_impl2_vargs(int pri, const char * fmt, va_list ap)
{
    char aDateBuf[DATEBUF_SIZE];
    char buf[1024];
    int  i;
    switch (g_nLogMethod) {
    case LOGM_FILE:
        get_curtime_l(aDateBuf);

        if (NULL == l_fLogFile)
            l_fLogFile = fdopen(STDERR_FILENO, "w");

        i = snprintf(buf, sizeof(buf) - 1, "%s %s%s: ",
                     aDateBuf, _get_thread_str(),
                     aPriNameShort[pri]);
        vsnprintf(buf + i, sizeof(buf) - 1 - i, fmt, ap);
        fprintf(l_fLogFile, "%s\n", buf);
        fflush(l_fLogFile);
        
        /* in case we just pushed the current file past the size limit... */
        // rotateFilesIfNecessary();
        break;

    case LOGM_SYSLOG:
#ifdef WIN32
#else /* !WIN32 */
        make_syslog_time(aDateBuf);

        vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
        syslog((pri <= LOG_DEBUG) ? pri : LOG_DEBUG,
               "%s %s%s: %s\n",
               aDateBuf, _get_thread_str(),
               aPriNameShort[pri], buf);
#endif /* !WIN32 */
        break;

    case LOGM_CALLBACK:
        if (NULL != l_fLogFunc) {
            get_curtime_l(aDateBuf);
            i = sprintf(buf, "%s %s%s: ", 
                        aDateBuf, _get_thread_str(), 
                        aPriNameShort[pri]);
            vsnprintf(&buf[i], sizeof(buf) - i, fmt, ap);

            (*l_fLogFunc)(pri, buf);
        }
        break;

    default:
        assert(0);
        break;
    }        
}

void
_log_impl1(int pri, const char * file, int line, const char * fmt, ...)
{
    va_list ap; // = NULL;

    if (pri <= g_nLogPriority && pri >= 0) {
        va_start(ap, fmt);
        _log_impl1_vargs(pri, file, line, fmt, ap);
        va_end(ap);
    }
}

void
_log_impl2(int pri, const char * fmt, ...)
{
    va_list ap; // = NULL;

    if (pri <= g_nLogPriority && pri >= 0) {
        va_start(ap, fmt);
        _log_impl2_vargs(pri, fmt, ap);        
        va_end(ap);
    }
}

void
_log_append(const char *fmt, ...)
{
    va_list ap; // = NULL;
    va_start(ap, fmt);
    vfprintf(l_fLogFile, fmt, ap);
    fflush(l_fLogFile);
    va_end(ap);
}


int
open_logfile(const char * filename)
{
    FILE * file = fopen(filename, "a");
    FILE * tmp;
    int result = -1;

#ifdef USE_PTHREAD
    l_tInitThread = get_current_thread_id();
#endif

    g_nLogMethod = LOGM_FILE;
    
    if (NULL != file) {
        if (NULL != (tmp = l_fLogFile)) {
            _log_impl2(LOG_NOTICE, "Log file is switched to %s", filename);
        }
        
        l_fLogFile = file;

        if (NULL != tmp) {
            fclose(tmp);
        }        

        _log_impl2(LOG_NOTICE, "Log file set to %s", filename);

        result = 0;
    }
    
    return result;
}

int 
set_log_callback(LOG_CALLBACK_F func)
{
    if (NULL != func) {
        l_fLogFunc = func;
        g_nLogMethod = LOGM_CALLBACK;
    } else {
        g_nLogMethod = LOGM_FILE;
        l_fLogFunc = NULL;
    }
  
    return 0;
}


void
set_log_priority(int pri)
{
    if (pri >= 0 && pri <= LOG_DEBUG_STACK) {        
        if (pri != g_nLogPriority) {
            g_nLogPriority = LOG_NOTICE;
            _log_impl2(LOG_NOTICE, "Log level set to LOG_%s", aPriNameLong[pri]);
        }                
        g_nLogPriority = pri;
    }
}

void
set_log_priority_name(const char * name)
{
    int pri;
    const char ** pch;
    if (strstr(name, "LOG_") == name) {
        for (pch = &aPriNameLong[0]; *pch; pch++) {
            if (!strcmp(&name[sizeof("LOG_") - 1], *pch)) {
                pri = (pch - &aPriNameLong[0]);
                if (pri != g_nLogPriority)
                {
                    g_nLogPriority = LOG_NOTICE;
                    _log_impl2(LOG_NOTICE, "Log level set to %s", name);
                    g_nLogPriority = pri;
                }                
                break;
            }            
        } /* for */
    } /* if (strstr...)*/
}

