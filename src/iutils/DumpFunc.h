/*
 * $Id: DumpFunc.h,v 1.3 2005/02/26 02:52:49 xiaofeng Exp $
 *
 * dump functions header, from msgdump
 *
 * Copyright (c) 2004-2006. All rights reserved.
 *
 * $Log: DumpFunc.h,v $
 *
 */

#ifndef _UTILS_DUMPFUNC_H_
#define _UTILS_DUMPFUNC_H_

/** @file DumpFunc.h Data dumpping functions */

#ifndef MAX_LINE_SIZE
# define MAX_LINE_SIZE   1024
#endif

#ifndef MAX_FRAME_SIZE
# define MAX_FRAME_SIZE  2048
#endif

#ifdef __cplusplus
# include <cstdio>
#else
# include <stdio.h>
#endif


BEGIN_EXTERN_C

/* fill a string field with format, use sprintf (to a string) by default */
typedef int (T_FIELD_FILL_FUNC)(char *str, const char *format, ...);
extern T_FIELD_FILL_FUNC  *fill_fields;

/* print out to a stream, use printf by default */
typedef int (T_FIELD_PRINT_FUNC)(const char *format, ...);
extern T_FIELD_PRINT_FUNC *dump_output;


/* print out of to real stream */
void dump_frame(const char *dtime, const char *fmark, 
                const char *frame, size_t fsize);

void dump_packet_raw(const char *packet, size_t psize);

void dump_line(const char *line);
void dump_hex(const char *buf, size_t size);


/* output to a string */
int sprintf_bin(OUT char *out, IN int start, IN int end, 
                IN const char *buf, IN size_t bytes);

int sprintf_hex(OUT char *out, IN const char *buf, IN size_t size);

int snprintf_hex(OUT char *out, IN size_t max, 
                 IN const char *buf, IN OUT size_t * size);

int snprintf_word(OUT char *out, IN size_t max, 
                  IN const char *buf, IN OUT size_t * size);

int snprintf_dword(OUT char *out, IN size_t max, 
                   IN const char *buf, IN OUT size_t * size);

int sprintf_asc(OUT char *out, IN const char *buf, IN size_t size);

int sprintf_field(OUT char *out, IN int offset, 
                  IN const char *name,  IN const char *desc);

void init_line(char **p);

void flush_line(char **p);

int cal_indents(int len);

INLINE size_t add_indents(size_t step)
{
    return cal_indents(step);
}

INLINE size_t del_indents(size_t step)
{
    return cal_indents(-(int)step);
}

END_EXTERN_C

#endif /* _UTILS_DUMPFUNC_H_ */
