/*
 * $Id: Log.h,v 1.4 2005/03/01 02:32:06 xiaofeng Exp $
 *
 * system logging support
 *
 * Copyright (c) 2004-2006. All rights reserved.
 *
 * $Log: Log.h,v $
 *
 */

#ifndef _UTILS_LOG_H_
#define _UTILS_LOG_H_

/** @file Log.h System Logging Support */

#ifndef  LOG_EMERG                      /* may also defined in <sys/syslog.h> */
# define LOG_EMERG              0       /**< system is unusable */
# define LOG_ALERT              1       /**< action must be taken immediately */
# define LOG_CRIT               2       /**< critical conditions */
# define LOG_ERROR              3       /**< error conditions */
# define LOG_WARN               4       /**< warning conditions */
# define LOG_NOTICE             5       /**< normal but significant condition */
# define LOG_INFO               6       /**< informational */
# define LOG_DEBUG              7       /**< debug-level messages */
#else
# define LOG_ERROR              3       /* syslog defines LOG_ERR */
#endif

#ifndef  LOG_DEBUG_STACK
# define LOG_DEBUG_STACK        8       /**< stack debug-level messages */
#endif

#ifndef SUPPORT_MACRO_VARGS
# if defined(__GNUC__)
#  define SUPPORT_MACRO_VARGS   1
# elif defined(WIN32)
#  define SUPPORT_MACRO_VARGS   0
# else
#  define SUPPORT_MACRO_VARGS   2
# endif
#endif

#if (SUPPORT_MACRO_VARGS == 1)
# ifndef NO_FILE_LINE
#  define LOGFILE(p, f, a...)   do { if (p <= g_nLogPriority)                 \
                                  _log_impl1(p, __FILE__, __LINE__, f, ##a);  \
                                } while (0)
# else
#  define LOGFILE(p, f, a...)   do { if (p <= g_nLogPriority)                 \
                                  _log_impl2(p, f, ##a);                      \
                                } while (0)
# endif
#elif (SUPPORT_MACRO_VARGS == 2)
# ifndef NO_FILE_LINE
#  define LOGFILE(p, ...)       do { if (p <= g_nLogPriority)                 \
                                 _log_impl1(p, __FILE__, __LINE__, __VA_ARGS__);\
                                } while (0)
# else
#  define LOGFILE(p, ...)       do { if (p <= g_nLogPriority)                 \
                                  _log_impl2(p, __VA_ARGS__);                 \
                                } while (0)
# endif
#elif (SUPPORT_MACRO_VARGS == 0)
# ifndef NO_FILE_LINE
#  ifdef __cplusplus
#   include <cstdarg>
#   define LOG_USE_CLASS
#   define LOGFILE              _CLogOutputFileLine(__FILE__, __LINE__)
#  else
#   include <stdarg.h>
#   define LOG_USE_INLINE
#   define LOGFILE              _log_impl2_inline
#  endif
# else
#  include <stdarg.h>
#  define LOG_USE_INLINE
#  define LOGFILE               _log_impl2_inline
# endif
#endif /* SUPPORT_MACRO_VARGS */


enum ELogMethod {
    LOGM_FILE = 0,
    LOGM_SYSLOG,
    LOGM_CALLBACK
};

typedef BOOL (*LOG_CALLBACK_F)(int pri, const char * str);



BEGIN_EXTERN_C

extern int  g_nLogPriority;
extern enum ELogMethod g_nLogMethod;

void _log_impl1(int pri, const char * file, int line, const char * fmt, ...);
void _log_impl2(int pri, const char * fmt, ...);
void _log_append(const char *fmt, ...);

/** Open a log file.  A program should call this when it wants to begin
 *  logging to a file, as opposed to some other sink like standard error.
 *  @param filename the path to the file
 *  @return 0 if the log file was successfully opened for writing, -1 for errors
 */
int open_logfile(const char * filename);

/** Set log callback function
 * @param func 
 */
int set_log_callback(LOG_CALLBACK_F func);

/** Set the priority level at which messages should be printed.
 *  @param pri the new priority level
 */
void set_log_priority(int pri);

/** Set the priority level at which messages should be printed.
 *  @param name the new priority level string, where name is:
 *   - "LOG_EMERG"
 *   - "LOG_ALERT"
 *   - "LOG_CRIT"
 *   - "LOG_ERR"
 *   - "LOG_WARN"
 *   - "LOG_NOTICE"
 *   - "LOG_INFO"
 *   - "LOG_DEBUG"
 *   - "LOG_DEBUG_STACK"
 */
void set_log_priority_name(const char * name);

/** Get the current priority level.
 * @return the current priority level
 * @see set_log_priority
*/
INLINE int get_log_priority(void)
{
    return g_nLogPriority;
}

/* Internal functions should not be called directly */

/** An vargs version of _log_impl1
 */
#ifdef LOG_USE_CLASS
void _log_impl1_vargs(int pri, const char *file, int line, 
                      const char * fmt, va_list parg);
#endif /* LOG_USE_CLASS */

/** An inline version of _log_impl2
 */
#ifdef LOG_USE_INLINE
void _log_impl2_vargs(int pri, const char * fmt, va_list parg);
INLINE void _log_impl2_inline(int pri, const char * fmt, ...)
{
    if (pri <= g_nLogPriority) {
        va_list argList;
        va_start(argList, fmt);
        _log_impl2_vargs(pri, fmt, argList);
        va_end(argList);
    }
}
#endif /* LOG_USE_INLINE */


END_EXTERN_C



#ifdef LOG_USE_CLASS
/** COutputFileLine
 * Help class for outputting trace information with file name and line number
 * of current source code.
 */
class _CLogOutputFileLine
{
private:
    const char * m_file;
    const int    m_line;

public:
    _CLogOutputFileLine(const char *file, int line) : m_file(file), m_line(line) {}

    void operator()(int pri, const char * fmt, ...) {
        if (pri <= g_nLogPriority) {
	        va_list argList;
            va_start(argList, fmt);
            _log_impl1_vargs(pri, m_file, m_line, fmt, argList);
            va_end(argList);
        }
    }
};
#endif /* LOG_USE_CLASS */

#endif /* _UTILS_LOG_H_ */
