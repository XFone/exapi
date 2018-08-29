/*
 * $Id: Base.h,v 1.5 2005/03/15 21:03:46 xiaofeng Exp $
 *
 * Global definitions
 *
 * Copyright (c) 2004-2006. All rights reserved.
 *
 * $Log: Base.h,v $
 *
 */

#ifndef _UTILS_GLOBAL_H_
#define _UTILS_GLOBAL_H_

/** @file Base.h Global Definitions */

#ifdef __cplusplus
# include <cstdlib>
# include <cassert>
# include <cerrno>
# if defined(__GNUC__) && __GNUC__ >= 3
#  define _INTERFACE_         struct
#  define _THREAD_LOCAL_      __thread
# elif defined(WIN32)
#  define _INTERFACE_         struct // __interface
#  define _THREAD_LOCAL_      __declspec(thread)
# elif defined(P_HPUX)
# endif
# define BEGIN_EXTERN_C       extern "C" {
# define END_EXTERN_C         };
# ifndef EXTERN_C
#  define EXTERN_C            extern "C"
# endif
# define PUBLIC               public:
# define PROTECTED            protected:
# define PRIVATE              private:
#else /* !__cplusplus */
# include <stdlib.h>
# include <assert.h>
# include <errno.h>
# define _INTERFACE_          struct
# if defined(__GNUC__)
#  include <stdint.h>
#  define INLINE              static inline
# elif defined(WIN32)
#  define INLINE              _inline
# elif defined(P_HPUX)
#  include <sys/_inttypes.h>
# else
#  include <sys/types.h>
# endif /* __GNUC__ */
# define BEGIN_EXTERN_C
# define END_EXTERN_C
# ifndef EXTERN_C
#  define EXTERN_C            extern
# endif
# define PUBLIC
# define PROTECTED
# define PRIVATE
#endif /* __cplusplus */

#ifndef TRUE
# define TRUE                 1
# define FALSE                0
typedef int BOOL;
#endif /* TRUE */

#ifndef INLINE
# define INLINE               __inline
#endif

#ifndef  IN
# define IN
# define OUT
# define INOUT
#endif /* IN */

#define GET_ARRAY_SIZE(a)      (sizeof(a) / sizeof(a[0]))

#ifndef SAFE_FREE
# define SAFE_FREE(obj)        if (NULL != obj) { free((void*)obj); obj = NULL; }
# define SAFE_FREE2(obj, cmd)  if (NULL != obj) { cmd((void*)obj);  obj = NULL; }
#endif

#endif /* _UTILS_GLOBAL_H_ */

