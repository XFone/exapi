/*
 * $Id: $
 *
 * Utilities for Shell: implementation
 *
 * ----------------------------------------------------------------------------
 *
 * Copyright (c) 2014-2016 ZeroneIO. All rights reserved.
 *
 * $Log: $
 *
 */
 
/** @file ShellUtil.h Shell Utilities Implementation */

// system headers
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <alloca.h>
#include <sys/wait.h>
#include <fcntl.h>

// libutil headers
#include "Base.h"
#include "Trace.h"

// dont print file:line in log
#define NO_FILE_LINE
#include "Log.h"

#include "osdeps/ShellUtil.h"

#ifndef MAX_PARAMS
#define MAX_PARAMS              64
#endif

#define DEBUG_SIGCHLD           FALSE

extern char **environ;

static void _shell_start(const char *cmd)
{
    char *argv[4];
    argv[0] = (char *)"sh";
    argv[1] = (char *)"-c";
    argv[2] = (char *)cmd;
    argv[3] = 0;
    (void)execve("/bin/sh", argv, environ);
    LOGFILE(LOG_ERROR, "shell '%s': %m", cmd);
    exit(127);
}

static void _program_start(const char *cmd)
{
    char *buf;
    char *argv[MAX_PARAMS];
    int len = strlen(cmd);

    // allocate memory in stack, which auto-free after process exits
    if (len > 0 && (buf = alloca(len + 1)) != NULL) {
        strcpy(buf, cmd);
        (void)split_args(buf, DIM(argv), argv);
        (void)execve(argv[0], argv, environ);
        //LOGFILE(LOG_ERROR, "call '%s': %m", cmd);
        LOGFILE(LOG_ERROR, "call '%s': %s", cmd, strerror(errno));
    } else {
        LOGFILE(LOG_ERROR, "call '%s': no enough resource");
    }
    exit(127);
}

int shell_sync(const char *cmd)
{
    int pid;
    int status = 0;

    assert(NULL != cmd);
    TRACE(5, "shell: %s", cmd);
    
    if ((pid = fork()) == 0) {
        _shell_start(cmd);
    } else if (pid > 0) {
        int sta = 0;
        // wait for child process exit
        while (waitpid(pid, &sta, 0) == -1) {
            if (errno != EINTR) {
                TRACE(5, "waitpid: %m");        
                break;
            }
        }
        if (WIFEXITED(sta)) {
            char val = WEXITSTATUS(sta); // low-order of 8 bits of exit()
            if ((status = (int)val) != 0) { // not normal termination
                TRACE(2, "child %d exits with %d", pid, val);
                // return status;
            }            
        }
    } else {
        status = -1;
        LOGFILE(LOG_ERROR, "shell_sync() fork: %m");
    }

    return status;
}

#if DEBUG_SIGCHLD
static void sig_chld(int signo)
{
    pid_t pid;
    int   sta;
    while ((pid = waitpid(-1, &sta, WNOHANG)) > 0) {
        if (WIFEXITED(sta)) {
            TRACE(5, "(%d): shell exited with %d", pid, (int)WEXITSTATUS(sta));
        } else {
            // unhandled signal
            TRACE(2, "(%d): shell terminated", pid);
        }
    }

    return;
}
#endif // DEBUG_SIGCHLD

int shell_async(const char *cmd, int (*cb)(int))
{
    int pid;

    if ((pid = fork()) == 0) {
        int result = 0;
#if DEBUG_SIGCHLD
        struct sigaction sa;    
        sigemptyset(&sa.sa_mask);
        sigaddset(&sa.sa_mask, SIGCHLD);    
        sa.sa_flags   = 0;
        sa.sa_handler = NULL;
        sigaction(SIGCHLD, &sa, sig_chld);
#endif   
        if (cb != NULL) {
            int status = shell_sync(cmd);
            if (NULL != cb) {
                // NOTE: since we are in a child process here, callback only
                //       handles global resources (devices) which can be shared
                //       between processes.
                result = (*cb)(status);
            }
        } else {
            _shell_start(cmd);
        }
        exit(result);
    } else if (pid < 0) {
        LOGFILE(LOG_ERROR, "shell_async() fork: %m");
    }
    
    return pid;
}

int shell_async_pidfile(const char *pidfile, const char *cmd, int (*cb)(int))
{
    int pid;

    if ((pid = fork()) == 0) {
        int fd = 0;
        int result = 0;

        if (0 != (fd = open(pidfile, O_RDWR))) {
            struct flock lock;
            char buf[32];
            int len, outs;

            lock.l_type   = F_WRLCK;
            lock.l_whence = SEEK_SET;
            if (fcntl(fd, F_SETLK, &lock) < 0) {
                // lock pidfile failed, just exit
                LOGFILE(LOG_ERROR, "shell_async_pidfile() lock '%s': %m", pidfile);            
                goto close_exit;
            }
            len = sprintf(buf, "%d\n", (int)pid);
            if ((outs = write(fd, buf, len)) != len) {
                // write failed, exit
                LOGFILE(LOG_ERROR, "shell_async_pidfile() write '%s': %m", pidfile);            
                goto close_exit;
            }
        } else {
            LOGFILE(LOG_ERROR, "shell_async_pidfile() failed open '%s': %m", pidfile);            
        }

        if (cb != NULL) {
            int status = shell_sync(cmd);
            if (NULL != cb) {
                // NOTE: since we are in a child process here, callback only
                //       handles global resources (devices) which can be shared
                //       between processes.
                result = (*cb)(status);
            }
        } else {
            _shell_start(cmd);
        }

close_exit:
        if (0 != fd) {
            close(fd);
        }

        exit(result);
    } else if (pid < 0) {
        LOGFILE(LOG_ERROR, "shell_async() fork: %m");
    }
    
    return pid;
}

int split_args(char *cmdbuf, size_t max_argc, char *argv[])
{
    char *last = cmdbuf;
    int i;    
    
    assert(NULL != cmdbuf);
    assert(NULL != argv);

    for (i = 0; i < (int)max_argc; i++) {
        char *tk = strtok_r(last, " \t", &last);
        if (NULL == tk)
            break;
        argv[i] = tk;
    } // for (i)
    
    // end with NULL
    argv[i] = NULL;
    
    // for debug
#if 0
    for (i = 0; i < (int)max_argc; i++) {
        if (argv[i] == NULL)
            break;
        printf("[%d] %s\n", i, argv[i]);
    }    
#endif
    return i;
}

int call_program_sync(const char *cmd)
{
    int pid;
    int status = 0;

    assert(NULL != cmd);
    TRACE(5, "shell: %s", cmd);
    
    if ((pid = fork()) == 0) {
        _program_start(cmd);
    } else if (pid > 0) {
        int sta = 0;
        // wait for child process exit
        while (waitpid(pid, &sta, 0) == -1) {
            if (errno != EINTR) {
                TRACE(5, "waitpid: %m");        
                break;
            }
        }
        if (WIFEXITED(sta)) {
            char val = WEXITSTATUS(sta); // low-order of 8 bits of exit()
            if ((status = (int)val) != 0) { // not normal termination
                TRACE(2, "child %d exits with %d", pid, val);
                // return status;
            }            
        }
    } else {
        status = -1;
        LOGFILE(LOG_ERROR, "shell_sync() fork: %m");
    }

    return status;    
}

int call_program_async(const char *cmd, int (*cb)(int))
{
    int pid;

    assert(NULL != cmd);
    TRACE(5, "call: %s", cmd);
    
    if ((pid = fork()) == 0) {
        int result = 0;
        if (NULL != cb) {
            int status = call_program_sync(cmd);
            if (NULL != cb) {
                // NOTE: since we are in a child process here, callback only
                //       handles global resources (devices) which can be shared
                //       between processes.
                result = (*cb)(status);
            }
        } else {
            _program_start(cmd);
        }        
        exit(result);        
    } else if (pid < 0) {
        LOGFILE(LOG_ERROR, "call_program_async() fork: %m");
    }

    return pid;    
}
