/*
 * $Id: $
 *
 * Worker implemented as a docker instance
 *  
 * Copyright (c) 2014-2016 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file WorkerDocker.h Worker Interface for docker container */
#include "Base.h"
#include "Log.h"
#include "osdeps/ShellUtil.h"

#include "IWorker.h"

#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#if !(_WIN32 || _WIN64)
#include <sys/wait.h>
#include <sys/types.h>
#endif
     
namespace ATS {
    extern string worker_cmd_path;

    class WorkerDocker : public IWorker
    {
    private:
        int pid_child;
        
    public:
        /** 
         * Get a new instance.
         */
        static WorkerDocker &Instance(const char *domain = NULL) {
            WorkerDocker *instance = new WorkerDocker();
            LOGFILE(LOG_INFO, "new WorkerDocker(%p) in domain '%s'", 
                    instance, domain);
            return *instance;
        }

        /**
         * Run task in a docker container
         * @param cb callback function (in child process)
         * @param cmd command line and arguments (after docker run ...)
         * @return pid of child process, -1 for error
         */
        virtual int Run(void *(*cb)(void *), void *cmd) {
            char path[FILENAME_MAX];
            // IN status (error-code), OUT result (exit-code)
            int (*child_cb)(int) = reinterpret_cast<int (*)(int)>(cb);
            string run_script(getcwd(path, sizeof(path)));
            //run_script.append("/../bin/run-docker.sh ");
            run_script.append("/");
            run_script.append(worker_cmd_path);
            run_script.append(" ");
            run_script.append(static_cast<const char *>(cmd));
            LOGFILE(LOG_DEBUG, "Run %s", run_script.c_str());
#if !(_WIN32 || _WIN64)
            //pid_child = call_program_async(run_script.c_str(), child_cb);
            pid_child = shell_async(run_script.c_str(), child_cb);
#endif
            return pid_child;
        }

        virtual int WaitStop(int wait_msecs) {
            int status;
            // TODO: to implement wait_msecs
#ifndef WIN32
            waitpid(pid_child, &status, 0);
            return WIFSTOPPED(status) ? -EACCES : 0;
#else
            return -EACCES;
#endif
        }

        virtual int Kill(int signal = 9) {
#ifndef WIN32
            return kill(pid_child, signal);
#else
            return -EACCES;
#endif
        }
    };
}
