/*
 * $Id: $
 *
 * Config file and command line support
 *
 * Copyright (c) 2014-2016 ZeroneIO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */

#ifndef _UTILS_CONSOLE_CONFIG_H_
#define _UTILS_CONSOLE_CONFIG_H_

#ifdef WIN32
# include "osdeps/win32/getopt_win.h"
#else
# include <unistd.h>
#endif

#define DEFAULT_CONFIG_OPTIONS        ":f:v:t:ThV"

BEGIN_EXTERN_C

typedef int (*TArgOptFunc)(int opt, char *oarg);

typedef void (*TCfgReadCb)(const char *key, const char *vals[]);

/** Parse command line arguments
 * @param argc argc
 * @param argv argv
 * @return count of arguments parsed (index of argv)
 */
int  parse_args(int argc, char *argv[], const char *opts, TArgOptFunc chkopt);

/** Print default usage with preset config options
 * @param name program name
 * @param usage command line usage
 * @param optstrs user specific options
 * @return none.
 */
void print_usage(char * name, const char * usage, const char * optstrs);

/** Read config file and return <key, values> pair
 * @param fname config filename
 * @param cb callback funciotn
 * @return none.
 */
void read_config_file(const char *fname, TCfgReadCb cb);

/** Program version */
extern const char *version_str;

END_EXTERN_C

#endif /* _UTILS_CONSOLE_CONFIG_H_ */
