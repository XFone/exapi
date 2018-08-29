/*
 * $Id: $
 *
 * Utilities for Shell: Header File
 *
 * ----------------------------------------------------------------------------
 *
 * Copyright (c) 2014-2016 ZeroneIO. All rights reserved.
 *
 * $Log: $
 *
 */
 
/** @file ShellUtil.h Shell Utilities */
 
#ifndef _SHELL_UTIL_H_
#define _SHELL_UTIL_H_

#if defined(P_LINUX) || defined(P_DARWIN) || defined (P_UNIX)

#ifndef DIM                     
#define DIM(a)                  (sizeof(a) / sizeof(a[0]))
#endif

BEGIN_EXTERN_C

/** Call shell command synchronizely.
 * @param cmd command line to call
 * @return 0 for success, -1 for error (current process),
 *         or error exit code (value returns by child process):
 *         1: invalid parameter
 *         2: error occurred in read/write, or recv a SIGINT
 *         3: timeout
 *         4+: the n' ABORT condition occurred (see 'man waitpid')
 */
int shell_sync(const char *cmd);

/** Call shell command asynchronizely
 * @param cmd command line to call
 * @param cb callback (in a child process) when shell exits or any error occurs
 * @return pid of child process, -1 for error.
 */
int shell_async(const char *cmd, int (*cb)(int));

/** Call shell command asynchronizely with a given .pid file as a lock
 * @param pidfile the .pid file to lock with
 * @param cmd command line to call
 * @param cb callback (in a child process) when shell exits or any error occurs
 * @return pid of child process, -1 for error.
 */
int shell_async_pidfile(const char *pidfile, const char *cmd, int (*cb)(int));

/** Split a command line into an argument array.
 * @param cmbbuf buffer saved the command.
 * @param max_argc maximum argument count
 * @param argv output argument array
 * @return count of arguments, -1 for errors.
 */
int split_args(IN OUT char *cmdbuf, IN size_t max_argc, OUT char *argv[]);

/** Execute a program synchronizely.
 * NOTE: this routine does not use default 'sh', so absolute path of program
 *       must be included in the command.
 * @param cmd program command line to run.
 * @return 0 for success, elsewise for error (\ref shell_sync "shell_sync").
 */
int call_program_sync(const char *cmd);

/** Execute a program asynchronizely.
 * NOTE: this routine does not use default 'sh', so absolute path of program
 *       must be included in the command.
 * @param cmd program command line to run.
 * @param cb callback (in a child process) when shell exits or any error occurs
 * @return pid of child process, -1 for error.
 */
int call_program_async(const char *cmd, int (*cb)(int));

END_EXTERN_C

#endif /* P_LINUX || P_UNIX || P_DARWIN */
#endif /* _SHELL_UTIL_H_ */
