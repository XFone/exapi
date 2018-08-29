/*
 * $Id: DateTime.h,v 1.3 2005/03/01 02:32:06 xiaofeng Exp $
 *
 * Time and Date routines
 *
 * Copyright (c) 2004-2006. All rights reserved.
 *
 * $Log: DateTime.h,v $
 *
 */

#ifndef _UTILS_DATETIME_H_
#define _UTILS_DATETIME_H_

#ifndef DATEBUF_SIZE
# define DATEBUF_SIZE 256
#endif

/** @file DateTime.h Time and Date Routines */



BEGIN_EXTERN_C

/** Get current time string in format yyyy-mm-dd HHMMSS.uuu
 *  @param sTime - char sTime[20]
 */
void get_curtime_l(char * sTime);

/** Get current time string in format HHMMSS.uuu
 *  @param sTime - char sTime[20]
 */
void get_curtime_s(char * sTime);

/** Get current time string in format yyyy-mm-dd HH:MM:SS
 *  @param sTime - char sTime[20]
 */
void get_curtime_f(char * sTime);

/** Get current date string in format yyyymmdd
 *  @param szDate - char szDate[20]
 */
void get_curdate_s(char * szDate);

/** Get timeval string in format HHMMSS.uuu
 *  @param szDate - char szDate[20]
 *  @param maxLen - maximum length of szData in bytes
 *  @param ts - struct timeval
 */
char *strtimeval_s(char *szDate, size_t maxLen, struct timeval ts);

END_EXTERN_C

#endif /* _UTILS_DATETIME_H_ */
