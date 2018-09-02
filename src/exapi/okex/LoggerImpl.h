/*
 * $Id: $
 * 
 * LoggerImpl class declaration
 *  
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file LoggerImpl.h Implements restbed::Logger.
 */

#include "Base.h"
#include "Log.h"

#include <restbed>

// defined in Log.h
extern "C" void _log_impl2_vargs(int pri, const char * fmt, va_list ap);

/**
 * LoggerImpl
 */
class LoggerImpl : public restbed::Logger {
protected:
    static std::shared_ptr<LoggerImpl> _instance;

    int GetLevel(const restbed::Logger::Level level) {
        if (level == restbed::Logger::INFO) return LOG_INFO;
        else return LOG_DEBUG_STACK - (int)level / 1000;
    }

public:
    static std::shared_ptr<LoggerImpl> GetInstance() {
        if (!_instance.get()) {
            _instance = std::make_shared<LoggerImpl>();
        }
        return _instance;
    }

    LoggerImpl() {}

    void stop(void) {}

    void start(const std::shared_ptr<const restbed::Settings> &) {}

    void log(const restbed::Logger::Level level, const char *format, ...) {
        va_list arglist;
        va_start(arglist, format);
        _log_impl2_vargs(GetLevel(level), format, arglist);
        va_end(arglist);
    }

    void log_if(bool expr, const restbed::Logger::Level level, const char* format, ...) {
        if (expr) {
            va_list arglist;
            va_start(arglist, format);
            _log_impl2_vargs(GetLevel(level), format, arglist);
            va_end(arglist);
        }
    }
};