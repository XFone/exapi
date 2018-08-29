/*
 * $Id: timetype.c,v 1.3 2005/02/26 02:38:22 xiaofeng Exp $
 *
 * Time and Date routines
 *
 * Copyright (c) 2004-2006. All rights reserved.
 *
 * $Log: timetype.c,v $
 *
 */

#include "Base.h"
#include "TimeType.h"

#include "osdeps/osutil.h"

#ifdef WIN32    
TTimeInterval gMinTimeInterval = { MIN_TIME_INTERVAL, 0 };
#else
TTimeInterval gMinTimeInterval = { 0, MIN_TIME_INTERVAL };
#endif


TPTimeInterval inc_time(TPTimeInterval l, TPTimeInterval r)
{
    assert(l != NULL);
    assert(r != NULL);
    
    l->_SEC += r->_SEC;
    l->_USEC += r->_USEC;
   
    while (l->_USEC >= 1000000L) {
        l->_SEC++;
        l->_USEC -= 1000000L;
    }
    
    return l;
}


TPTimeInterval dec_time(TPTimeInterval l, TPTimeInterval r)
{
    assert(l != NULL);
    assert(r != NULL);
    
    l->_SEC -= r->_SEC;
    l->_USEC -= r->_USEC;

    while (l->_USEC < 0 && l->_SEC > 0) {
        l->_SEC--;
        l->_USEC += 1000000L;
    }
    
    return l;
}

int compare_time(TPTimeInterval l, TPTimeInterval r)
{
    int result;
    assert(l != NULL);
    assert(r != NULL);    

    if (0 == (result = l->_SEC - r->_SEC)) {
        result = l->_USEC - r->_USEC;
        result = ((result > 0) ? 1 : ((result < 0) ? -1 : 0));
    }

    return result;
}
