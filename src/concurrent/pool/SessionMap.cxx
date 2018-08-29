/*
 * $Id: $
 *
 * Hash map for locating session (implementation)
 *  
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */

#include "SessionMap.h"
 
using namespace ATS;

// SessionMap::TSessionMap SessionMap::_hashmap;

SessionMap *SessionMap::_instance[MAX_MAP_IDS] = { NULL, NULL, NULL };

#ifdef UNIT_TEST

#include <string>
using namespace std;

void TestSessionMap()
{
    SessionMap &sm = SessionMap::Instance(1);
    string a("a");
    string b("b");
    string b2("b");
    unsigned char c1[] = "abdef";
    unsigned char c2[] = "abdef";

    int val = 1234;
    //int *pval = NULL;

    // case 1
    sm.Set<int *>("a", &val);
    assert(NULL != sm.Get<int *>("a"));
    assert(NULL != sm.Get<int *>(a));

    // case 2
    assert(NULL == sm.Get<int *>("b"));
    assert(NULL == sm.Get<int *>(b));

    // case 3
    sm.Set<int *>(b, &val);
    assert(NULL != sm.Get<int *>("b"));
    assert(NULL != sm.Get<int *>(b));
    assert(NULL != sm.Get<int *>(b2));

    // case 4
    sm.Set<int *>(c1, &val);
    assert(NULL != sm.Get<int *>("abdef"));
    assert(NULL != sm.Get<int *>(c2));

    //assert(0);
}

#endif
