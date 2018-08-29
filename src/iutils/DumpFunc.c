/*
 * $Id: DumpFunc.c,v 1.2 2005/02/04 02:15:53 xiaofeng Exp $
 *
 * dump functions, from msgdump
 *
 * Copyright (c) 2004-2006. All rights reserved.
 *
 * $Log: DumpFunc.c,v $
 *
 */

#include <time.h>
#include <stdio.h>
#include <string.h>

#include "Base.h"
#include "DumpFunc.h"

// bypass warnings in VS 8.0 
#if (_MSC_VER > 1200)
#pragma warning(disable:4996)
#else
#pragma GCC diagnostic ignored "-Wcast-align"
#endif


T_FIELD_FILL_FUNC  *fill_fields   = sprintf;
T_FIELD_PRINT_FUNC *dump_output   = printf;
    
char *my_ctime(unsigned long l)
{
    static char buf[256]; 
    struct tm *p_tm = localtime((const time_t *)&l);
    sprintf(buf, "%4u/%02u/%02u %02u:%02u:%02u", 
            p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday,
            p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec); 
    return buf;
}

/**
 * Dump in format
 * LSB:MSB
 * IN: 
 * [datetime] <<<<< FF {HEX}*
 * 
 * OUT:
 * [datetime] <<<<< FF FF FF FF FF FF FF FF  FF FF FF FF FF FF FF FF   [ASCII]
 *            0000 TAG: 0xFF
 *            0001 Address: 0xFF
 *            0002 Control: 0xFF ()
 *                   111----- = Receive SN: 7
 *                   ---1---- = 
 *                   ----00-- =
 *                   ------11 = 
 *            0003 CRC: 0xFFFF (correct)
 *            001A TAG: 0xFF
 * 
 */
static char   _indents_buf[256] = "";
static size_t _indents = 0;
// static int _level = 1;

/**
 * bytes must be 1, 2, 4, 8
 *
 * MSB (Most Significant Byte first)
 *    start     -->      end
 * 0   1   2   3   ...   7           ...        15         ...
 * +-------------------------+-------------------------+--------
 * |   | BYTE 1  / buf[0]    |   BYTE 2 / buf[1]       |
 * +-------------------------+-------------------------+--------
 *
 * LSB (Least Significant Byte first) - two bytes:
 *     end      <--      start
 * 15  14  13      ...   8     7     ...              0    ...
 * +-------------------------+-------------------------+--------
 * |   | BYTE 2  / buf[1]    |   BYTE 1 / buf[0]       |
 * +-------------------------+-------------------------+--------
*/
int sprintf_bin(char *out, int start, int end, const char *buf, size_t bytes)
{
    int i;
    unsigned char val, mask;
    char *pout = out;
    
#if HOST_IS_MSB
    const char *pin = buf;
#else  /* LSByte */
    const char *pin = buf + bytes - 1;
    i     = (bytes << 3) - start - 1;
    start = (bytes << 3) - end - 1;
    end   = i;
#endif /* HOST_IS_MSB(yte) */

    assert(start <= end);
    assert(start < (int)(bytes << 3));
    assert(out != NULL);
    assert(buf != NULL);

    i = 1;
    while (1) {
        for (; i <= start; i++) { // starting with '-'
            *pout++ = '-';
            if ((i & 0x7) == 0) {      // pass a byte
                *pout++ = ' ';
#if HOST_IS_MSB
                pin++;
#else
                pin--;
#endif
            }
        }
        if (start <= end) {
            i--;
            while (i <= end) {
                val = *pin++;
                mask = (0x80 >> (i & 0x7));
                for (; i <= end && mask; i++, mask >>= 1)
                    *pout++ = (val & mask) ? '1' : '0'; 
                for (; 0 != mask; mask >>= 1)
                    *pout++ = '-';
                *pout++ = ' ';
            }
            i += 7;
            i &= 0xFFF8;
            i++;
            start = (bytes << 3);
        } else {
            break;
        }
    }

    return (pout - out);
}

int sprintf_hex(char *out, const char *buf, size_t size)
{
    register int i;
    unsigned char val;
    register char *p = out;
    register const char *p_buf;
    const char *buf_end = buf + size;
    
    for (i = 0, p_buf = buf; p_buf < buf_end; i++, p_buf++) {
        val = 0x0F & (*p_buf >> 4);
        *p++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        val = 0x0F & *p_buf;
        *p++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        *p++ = ' ';
        if (0x07 == (i & 0x07))
            *p++ = ' ';
    }
    return (p - out);
}

int snprintf_hex(char *out, size_t max, const char *buf, size_t *size)
{
    register int i;
    unsigned char val;
    register char *p_out = out;
    register const char *p_buf;
    const char *buf_end = buf + *size;
    char *out_end = out + max - 4;
    
    for (i = 0, p_buf = buf; p_out < out_end && p_buf < buf_end; i++, p_buf++) {
        val = 0x0F & (*p_buf >> 4);
        *p_out++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        val = 0x0F & *p_buf;
        *p_out++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        *p_out++ = ' ';
        if (0x07 == (i & 0x07))
            *p_out++ = ' ';
    }

    *size = (p_buf - buf);
    return (p_out - out);
}

int snprintf_word(char *out, size_t max, const char *buf, size_t *size)
{
    register int i;
    unsigned char val;
    register char *p_out = out;
    register const unsigned short *p_buf = (const unsigned short *)buf;
    const unsigned short *buf_end = (const unsigned short *)(buf + *size);
    char *out_end = out + max - 4;
    
    for (i = 0; p_out < out_end && p_buf < buf_end; i++, p_buf++) {
        val = 0x0F & (*p_buf >> 12);
        *p_out++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        val = 0x0F & (*p_buf >> 8);
        *p_out++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        val = 0x0F & (*p_buf >> 4);
        *p_out++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        val = 0x0F & *p_buf;
        *p_out++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        *p_out++ = ',';
        if (0x07 == (i & 0x07))
            *p_out++ = ' ';
    }

    *size = ((const char *)p_buf - buf);
    return (p_out - out);
}

int snprintf_dword(char *out, size_t max, const char *buf, size_t *size)
{
    register int i;
    unsigned char val;
    register char *p_out = out;
    register const unsigned long *p_buf = (const unsigned long *)buf;
    const unsigned long *buf_end = (const unsigned long *)(buf + *size);
    char *out_end = out + max - 4;
    
    for (i = 0; p_out < out_end && p_buf < buf_end; i++, p_buf++) {
        val = 0x0F & (unsigned char)(*p_buf >> 28);
        *p_out++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        val = 0x0F & (unsigned char)(*p_buf >> 24);
        *p_out++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        val = 0x0F & (unsigned char)(*p_buf >> 20);
        *p_out++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        val = 0x0F & (unsigned char)(*p_buf >> 16);
        *p_out++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        val = 0x0F & (unsigned char)(*p_buf >> 12);
        *p_out++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        val = 0x0F & (unsigned char)(*p_buf >> 8);
        *p_out++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        val = 0x0F & (unsigned char)(*p_buf >> 4);
        *p_out++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        val = 0x0F & (unsigned char)(*p_buf);
        *p_out++ = (val >= 0x0A) ? (val + 'A' - 0x0A) : ('0' + val);
        *p_out++ = ',';
        if (0x07 == (i & 0x07))
            *p_out++ = ' ';
    }

    *size = ((const char *)p_buf - buf);
    return (p_out - out);
}


int sprintf_asc(char *out, const char *buf, size_t size)
{
    register int i;
    register char *p = out;
    register const char *p_buf;
    const char *buf_end = buf + size;
        
    for (i = 0, p_buf = buf; p_buf < buf_end; p_buf++, i++) {
        *p++ = (*p_buf > 0x20 && *p_buf < 0x7F) ? *p_buf : '.';
        if (0x07 == (i & 0x07))
            *p++ = ' ';
    }
    return (p - out);
}

int sprintf_field(char *out, int offset, const char *name, const char *desc)
{
    if (offset >= 0)
        return fill_fields(out, "%04X %s : %s", offset, name, desc);
    else
        return fill_fields(out, "%s : %s", name, desc);
}

void init_buffer(void)
{
    memset(_indents_buf, ' ', sizeof(_indents_buf));    
}


void dump_frame(const char *dtime, const char *fmark, 
                const char *frame, size_t fsize)
{
    char hex[MAX_LINE_SIZE];
    char asc[MAX_LINE_SIZE];
    const char *p;
    int i, j, len;
         
    for (i = fsize, p = frame; i > 0; i -= 16) {
        j = (i > 16) ? 16 : i;
        len = sprintf_hex(hex, p, j);
        hex[len] = '\0';
        len = sprintf_asc(asc, p, j);
        asc[len] = '\0';
        p += 16;
        dump_output("%s %s %-50s : %s\n", 
                    (i == (int)fsize) ? dtime : _indents_buf, fmark, hex, asc);
    }
}

void dump_packet_raw(const char *packet, size_t psize)
{
    char hex[MAX_FRAME_SIZE];
    const char *p;
    int i, j, len;

    for (i = psize, p = packet; i > 0; i -= 16) {
        j = (i > 16)? 16 : i;
        len = sprintf_hex(hex, p, j);
        hex[len] = '\0';
        p += 16;
        dump_output("%s%s\n", _indents_buf, hex);
    }
}

void dump_line(const char *line)
{
    dump_output("%s", line);
}

void dump_hex(const char *buf, size_t size)
{
    char out[MAX_FRAME_SIZE];
    int i;
    i = sprintf_hex(out, buf, size);
    out[i] = '\0';
    dump_output("  %s\n", out);
}

int cal_indents(int len)
{
    _indents += len;
    memset(_indents_buf, ' ', _indents);
    _indents_buf[_indents] = '\0';
    return _indents;
}

/*---------------------------------------------------------------------------*/
/*  Routines for dumping packet fields                                       */
/*---------------------------------------------------------------------------*/

static char line[MAX_LINE_SIZE];

void init_line(char **p)
{
    *p = &line[0];
}

void flush_line(char **p)
{
    *(*p)++ = '\0';
    *p = &line[0];
    dump_output("%s%s\n", _indents_buf, line);
}
