/*
 * $Id: $
 *
 * Worker implemented as thread
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file WorkerThread.h Worker Interface */
#include "Base.h"
#include "Log.h"

#include "IWorker.h"

// C++11 thread features
#include <thread>
#include <chrono>

#ifdef USE_PTHREAD
#include <pthread.h>
#include <signal.h>
#endif

#if _WIN32 || _WIN64
#include <Windows.h>
#endif

namespace ATS {

    typedef std::thread::id thread_t;

    class WorkerThread : public IWorker
    {
    private:
        std::thread m_thread;

    public:
        virtual int Run(void *(*func)(void *), void *arg) {
            m_thread = std::thread(func, arg);
            return 0; // TODO
        }

        virtual int WaitStop(int wait_msecs) {
            if (!m_thread.joinable()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(wait_msecs));
            }
            if (m_thread.joinable()) {
                m_thread.join();
                return 0;
            }
            return -1;
        }

        virtual int Kill(int signal = 9) {
            void *handle = reinterpret_cast<void *>(m_thread.native_handle());
            LOGFILE(LOG_DEBUG, "Force killing thread [0x%x]", handle);
#if _WIN32 || _WIN64
            return TerminateThread(handle, signal);
#else
            return pthread_kill((pthread_t)handle, signal);
#endif
        }

        static thread_t GetId() {
            return std::this_thread::get_id();
        }

        static void USleep(int usecs) {
            std::this_thread::sleep_for(std::chrono::microseconds(usecs));
        }
    };
}
