/*
 * $Id: $
 *
 * Atomic types is thread-safe
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/** @file Atomic.h Atomic types */

#ifdef USE_TBB
#include <tbb/atomic.h>
#include <tbb/concurrent_vector.h>
#else
#include <atomic>
#include <vector>
#endif

namespace ATS {
  #ifdef USE_TBB
    typedef tbb::atomic<long> atomic_t;
  #else
    typedef std::atomic<long> atomic_t;
  #endif
}