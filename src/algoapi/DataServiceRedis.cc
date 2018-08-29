/*
 * $Id: $
 *
 * Quotation utility class for auto-trade strategy and algorithm system
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */

#include "Base.h"
#include "Log.h"

#include "DataServiceRedis.h"

using namespace AlgoApi;

DataServiceRedis *DataServiceRedis::_instance = NULL;
string DataServiceRedis::m_host = "localhost";

