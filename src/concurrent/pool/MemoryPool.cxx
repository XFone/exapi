/*
 * $Id: $
 *
 * Memory pool (implementation)
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */

#include "MemoryPool.h"

using namespace ATS;

TDefaultAllocator ATS::g_defaultPool;
TCachedAllocator  ATS::g_cachedPool;
