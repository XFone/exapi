/*
 * $Id: $
 *
 * Utilities for PidFile: Header File
 *
 *  --pid-path=/var/run/pidfile.pid
 *  --lock-path=/var/lock/lockfile.lock
 * ----------------------------------------------------------------------------
 *
 * Copyright (c) 2014-2017 ZeroneIO. All rights reserved.
 *
 * $Log: $
 *
 */
 
/** @file PidFIle.h Pid File Utilities */
 
#ifndef _PIDFILE_H_
#define _PIDFILE_H_


BEGIN_EXTERN_C

/** Create pidfile in path (e.g., /var/run/foo.pid)
 * @param pidfile_path path for the pidfile
 * @return < 0 for error, elsewise for pid_fd
 */
int create_pidfile(const char *pidfile_path);

/** Release (unlock) pidfile and close the fd
 * @param pid_fd the valid fd returned by create_pidfile()
 * @return nothing
 */
void release_pidfile(int pid_fd);

/** Fork current process as a daemon
 */
void fork_daemon();

END_EXTERN_C

#endif /* _PIDFILE_H_ */
