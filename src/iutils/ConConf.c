/*
 * $Id: Config.c,v 1.1 2005/03/15 03:05:38 xiaofeng Exp $
 *
 * Config file and command line support
 *
 * Copyright (c) 2004-2006. All rights reserved.
 *
 * $Log: Config.c,v $
 *
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "Base.h"
#include "ConConf.h"
#include "Trace.h"

// Dont print c filename and line no
#ifndef NO_FILE_LINE
#define NO_FILE_LINE
#endif
#include "Log.h"

#define _DEFAULT_OFFSET            24
#define _MAX_VALS                  16
#define _SUPPRT_STRING             1

// bypass warnings in VS 8.0 
#if (_MSC_VER > 1200)
#pragma warning(disable:4996)
#endif

const char *version_str = NULL;

#ifdef WIN32
char* strsep(char** stringp, const char *delim)
{
    char *start = *stringp;
    char *p;

    p = (start != NULL) ? strpbrk(start, delim) : NULL;

    if (p == NULL) {
        *stringp = NULL;
    } else {
        *p = '\0';
        *stringp = p + 1;
    }

    return start;
}
#endif /* WIN32 */

int
parse_args(int argc, char *argv[], const char *opts, TArgOptFunc chkopt)
{
    int result = 0;
    int i = 0;
    char optbuf[256] = DEFAULT_CONFIG_OPTIONS;

    if (NULL != opts)
        strncat(optbuf, opts, sizeof(optbuf) - sizeof(DEFAULT_CONFIG_OPTIONS));

    opterr = 0;
    
    while ((i = getopt(argc, argv, optbuf)) != EOF && result >= 0) {
        switch (i) {
        case 'f':
            if (optarg) {
                open_logfile(optarg);
                result++;
                result++;
            } else {
                result = -1;
            }            
            break;
            
        case 'v':
            if (optarg) {
                if (isdigit(*optarg))
                    set_log_priority(atoi(optarg));
                else
                    set_log_priority_name(optarg);
                result++;
                result++;
            } else {
                result = -1;
            }            
            break;

        case 't':
            if (optarg) {
                set_trace_level(atoi(optarg), TRUE);
                LOGFILE(LOG_INFO, "Trace level set to %d", atoi(optarg));
                result++;
                result++;
            } else {
                result = -1;
            }
            break;

        case 'T':
            g_bTraceTimestamp = TRUE;
            break;
            
        case ':':
            printf("Error: option requires an argument: -%c\n\n", optopt);
            result = -1;
            break;

        case '?':
            printf("Error: invalid option: -%c\n\n", optopt);
            result = -1;
            break;

        case 'h':
            //print_usage(argv[0]);
            //exit(0);
            result = -1;
            break;
            
        case 'V':
            printf("version: %s\n\n",
                   (version_str != NULL) ? version_str : "");
            result = -1;
            break;
            
        default:
            if (NULL != chkopt) {
                result = (*chkopt)(i, optarg);
            } else {
                printf("Error: invalid option: -%c\n\n", optopt);
                result = -1;
            }
            break;
        }
    }
    
    return result;
}


void print_usage(char * name, const char * usage, const char * optstrs)
{
    int i, offset;
    struct sOptionMap {
        char key;
        int  opt;
        const char *value;
        const char *desc;
    } omap [] = {
        { 'f', 1, "Log File",         "Specify log file."            },
        { 'v', 1, "Log Level",        "Specify log level."           },
        { 't', 1, "Trace Level",      "Specify debug trace level."   },
        { 'T', 0, "Trace Time Stamp", "Also print timestamp in trace output." },
        { 'V', 0, "Version Number",   "Print version number."        },
        { 'h', 0, "Help",             "Print this message and exit." },
    };
    
    if (NULL == usage)
        printf("Usage: %s [options] <parameters> ...\n"
               "Options:\n", name);
    else
        printf("Usage: %s [options] %s\n"
               "Options:\n", name, usage);

    
    for (i = 0; i < GET_ARRAY_SIZE(omap); i++) {
        printf("  -%c ", omap[i].key);
        offset = _DEFAULT_OFFSET;
        if (omap[i].opt) {
            printf("<%s>", omap[i].value);
            offset -= strlen(omap[i].value) + 2;
        }
        printf("%*c%s\n", offset, ' ', omap[i].desc);
    }
    printf("\n");

    if (NULL != optstrs) {
        printf("More Options:\n%s\n", optstrs);
    }
}

void read_config_file(const char *fname, TCfgReadCb cb)
{
    char line[256];
    int lineno = 0;
 
    FILE *fd = fopen(fname, "r");
    if (NULL != fd) {
        while (fgets(line, sizeof(line) - 2, fd)) {
            char *pline = &line[0];
            const char *token, *key = NULL;
            const char *val[_MAX_VALS + 1];
            const char **pval = &val[0];
            int  ntoken = 0;
 
#if _SUPPRT_STRING
            char *chr = strchr(pline, '\"');
#endif 

            TRACE(8, "%s------------", line);

            while ((token = strsep(&pline, " ,=\t\r\n\"")) != NULL) {
                // printf("<%p: %s>\n", token, token);
                if (token[0] == '#')  break;        // ignore comment
                if (ntoken > _MAX_VALS) break;      // too many values                
                if (token[0] != '\0') {             // NOT empty token
                    if (ntoken++ == 0)  {           // first token is key
                        key = token;
                    } else {
                        *pval++ = token;
                    }
                } else {
 #if _SUPPRT_STRING
                    if (NULL != chr && token >= chr) { 
                        token = chr + 1; 
                        if (NULL != (chr = strchr(token, '\"'))) {
                            *chr  = '\0';
                            pline = chr + 1; 
                            chr = strchr(pline, '\"');
                            *pval++ = token;
                            ntoken++;
                        } 
                    }
#endif
                    *pval = NULL;
                }
            } // while (strsep)

            if (NULL != key && NULL != cb) {
                (*cb)(key, val);
            }

            TRACE(8, "<ignored line %d>", lineno);

            lineno++;
            
        } // while (fgets(line))

        fclose(fd);
    } else {
        LOGFILE(LOG_ERROR, "failed to open config file %s", fname);
    }
}

