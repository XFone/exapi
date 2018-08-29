/*
 * $Id: $
 *
 * Global definition for ATS
 *
 * Copyright (c) 2014-2016 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#ifndef _ATS_GLOBAL_DEFINE_H_
#define _ATS_GLOBAL_DEFINE_H_

#include <uuid/uuid.h>

#define ATS_VERSION            "$Version$"
#define DEFAULT_PORT_SF        8601
#define DEFAULT_PORT_PB        8602 
#define DEFAULT_PORT_QUOT      8608 

#define DEFAULT_PORT           DEFAULT_PORT_PB

#define MIN_SO_EVENT           1000
#define MIN_SI_EVENT           2000
#define MIN_SA_EVENT           3000

#define MIN_SO_STATE           0
#define MIN_SI_STATE           100
#define MIN_SA_STATE           200
#define MAX_STATE_BITS         8
#define MAX_STATES             (1 << MAX_STATE_BITS)

#ifdef __GNUC__
#define likely(x)              __builtin_expect(!!(x), 1)
#define unlikely(x)            __builtin_expect(!!(x), 0)
#else
#define likely(x)              (x)
#define unlikely(x)            (x)
#endif

#define CAL_STA(s, e)          (((e) << MAX_STATE_BITS) | ((s) & (MAX_STATES - 1)))
//#define CAL_STA3(s1, s2, s3)   

typedef uuid_t TInstanceId;
typedef void  *FsmTimer;
typedef void  *StrategyTimer;

typedef struct CtxAlgoRule *PCtxAlgoRule; 

extern int g_is_exiting;

#endif /* _ATS_GLOBAL_DEFINE_H_ */
