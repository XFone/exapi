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


/*-------------------------- cplusplus -------------------------------*/

#if defined(__cplusplus) && (__cplusplus >= 201103L)

#include <string>
#include <typeinfo>

/**
 * type_name(T) is to demangle c++ type name for print out
 * This function is to replace the typeid(T).name(), only supported in C++11
 * <pre>
 *   try {
 *      // some exceptions ocur...
 *   } catch (exception &ex) {
 *      cout << "exception caught " << type_name(ex).data() << ": " << ex.what()); 
 *   }
 * </pre>
 */
template<typename T> std::string type_name(T &t);

#ifdef __GNUG__
# include <cxxabi.h>
# include <cstdlib>
# include <memory>
template<typename T>
std::string type_name(T &t) {
    int status;
    std::unique_ptr<char[], decltype(&std::free)> 
    buffer(__cxxabiv1::__cxa_demangle(typeid(t).name(), nullptr, 0, &status), &std::free);
    return (status == 0) ? buffer.get() : 
            /*"__cxa_demangle error"*/ typeid(t).name();
}
#else // !defined __GNUG__
template<typename T>
std::string type_name(T &t) {
    return typeid(t).name();
}
#endif //__GNUG__

#endif /* __cplusplus >= 201103L */

/*-------------------------- C Functions -----------------------------*/

BEGIN_EXTERN_C

/**
 * assert in thread-safe
 */
BOOL _assert_thread_op(int retval, unsigned *retry,
                       const char *func, const char *file, unsigned line);


void _check_func(const char * file, int line, const char * msg);

/**
 * Catch sigfault and printout
 */
void catch_segfault(void (*handler)(int, siginfo_t *, void *));

END_EXTERN_C

#endif /* _UTILS_OSUTIL_H_ */
