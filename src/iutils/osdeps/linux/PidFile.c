/*
 * $Id: $
 *
 * Utilities for PidFile: implementation
 *
 * ----------------------------------------------------------------------------
 *
 * Copyright (c) 2014-2017 ZeroneIO. All rights reserved.
 *
 * $Log: $
 *
 */
 
/** @file PidFile.c PidFile Utilities Implementation */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

// libutil headers
#include "Base.h"

// dont print file:line in log
#define NO_FILE_LINE
#include "Log.h"

static char *saved_pidfile;

static void delete_pidfile(void)
{
    if (saved_pidfile) unlink(saved_pidfile);
}

int pidfile_acquire(const char *pidfile)
{
    int pid_fd;
    if (!pidfile) return -1;

    pid_fd = open(pidfile, O_CREAT | O_WRONLY, 0644);
    if (pid_fd < 0) {
        LOGFILE(LOG_ERROR, "Unable to open pidfile %s: %s", pidfile, strerror(errno));
    } else {
        lockf(pid_fd, F_LOCK, 0);
        if (!saved_pidfile) atexit(delete_pidfile);
        saved_pidfile = (char *) pidfile;
    }

    return pid_fd;
}

void release_pidfile(int pid_fd)
{
    if (pid_fd < 0) return;
    lockf(pid_fd, F_UNLCK, 0);
    close(pid_fd);
}

int create_pidfile(const char *pidfile_path)
{
    int fd = pidfile_acquire(pidfile_path);

    if (fd >= 0) {
        FILE *out;
        if ((out = fdopen(fd, "w")) != NULL) {
            fprintf(out, "%d\n", getpid());
            fclose(out);
        }
    }

    release_pidfile(fd);

    return fd;
}

void fork_daemon()
{
    pid_t pid, sid;
    
    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* Change the file mode mask */
    umask(0);       
    
    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        LOGFILE(LOG_ERROR, "Failed to setsid() %d: %s", sid, strerror(errno));
        exit(EXIT_FAILURE);
    }
        
    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    //close(STDOUT_FILENO);
    //close(STDERR_FILENO);
}
