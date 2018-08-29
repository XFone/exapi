/*
 * $Id: $
 *
 * Routines for CLI (Command Line Interface) Parsing: C++ Implementation
 *
 * Copyright (c) 2004-2006. All rights reserved.
 *
 * $Log: $
 *
 */

#include "Base.h"
#include "Cli.h"
#include "Trace.h"

// Dont print c filename and line no
#ifndef NO_FILE_LINE
#define NO_FILE_LINE
#endif
#include "Log.h"

#include <string>
#include <hash_map>

using namespace std;
using namespace __gnu_cxx;

namespace __gnu_cxx {
    /// my hash function
    template<> struct hash<string> {
        size_t operator()(string s) const {
            return __stl_hash_string(s.c_str());
        }
    };
}

typedef hash_map <string, size_t> TStringMap;

/// Loopup table using hash_map
struct CCmdTblIterator {
    TPCMD_ITEM tbl;
    size_t     size;
    TStringMap smap;
    void       init(void);
};

#ifdef CLI_BACKWARD
typedef hash_map <unsigned long, CCmdTblIterator> TIterMap;
static TIterMap iter_map;

TCMD_TBL_ITER cli_get_iterator(struct TCMD_ITEM tbl[], size_t tblsiz)
{
    // xiaofeng 09/03/27: this is only for backward compatible, for
    // performance, we build a iterator-map to save all given tables, which
    // is initialized only when the iterator is referenced at first time.
    TIterMap::iterator it;
    it = iter_map.find(reinterpret_cast<unsigned long>(tbl));
    if (it == iter_map.end()) {
        CCmdTblIterator &iter = iter_map[reinterpret_cast<unsigned long>(tbl)];
        iter.tbl  = &tbl[0];
        iter.size = tblsiz;
        iter.init();
    } else {
        CCmdTblIterator &iter = it->second;
        if (iter.size != tblsiz) {
            LOGFILE(LOG_WARN, "Table size changed, reinitializing...");
            iter.size = tblsiz;
            iter.init();
        }        
    }    
    
    return static_cast<TCMD_TBL_ITER>(&it->second);
}
#endif // CLI_BACKWARD

    
TCMD_TBL_ITER cli_alloc_iterator(struct TCMD_ITEM tbl[], size_t tblsiz)
{
    CCmdTblIterator *res;

    res = new CCmdTblIterator;
    if (NULL == res) {
        LOGFILE(LOG_ERROR, "Cannot allocate iterator for table");
    } else {
        res->tbl  = &tbl[0];
        res->size = tblsiz;
        res->init();
    }
    
    return static_cast<TCMD_TBL_ITER>(res);
}


void cli_free_iterator(TCMD_TBL_ITER iter)
{
    CCmdTblIterator *res = static_cast<CCmdTblIterator *>(iter);

    assert(NULL != res);

    delete res;
}


ssize_t cli_find_str(TCMD_TBL_ITER iter, const char *name)
{
    ssize_t              res;
    CCmdTblIterator     *tit;
    TStringMap::iterator it;

    tit = static_cast<CCmdTblIterator *>(iter);
    assert(NULL != tit);

    it = tit->smap.find(name);
    if (it != tit->smap.end()) {
        res = it->second;
    } else {
        res = -1;
    }
    return res; 
}

ssize_t cli_find_prefix(TCMD_TBL_ITER iter, const char *str)
{
    // TO BE DONE.
    return -1;
}

ssize_t cli_match_str(TCMD_TBL_ITER iter, const char *name,
                      OUT size_t match_list[], size_t lstsiz)
{
    // TO BE DONE.
    return -1;
}

void CCmdTblIterator::init()
{
    smap.clear();
    for (size_t n = 0; n < size; n++) {
        // initialize hash-table
        smap[tbl[n].name] = n;
    }
}
