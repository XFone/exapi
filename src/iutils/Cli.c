/*
 * $Id: $
 *
 * Routines for CLI (Command Line Interface) Parsing: Implementation
 *
 * Copyright (c) 2004-2006. All rights reserved.
 *
 * $Log: $
 *
 */

#include <stdlib.h>
#include <string.h>

#include "Base.h"
#include "Cli.h"
#include "Trace.h"

// Dont print c filename and line no
#ifndef NO_FILE_LINE
#define NO_FILE_LINE
#endif
#include "Log.h"

/** Simple loopup table using bi-search (for embedded application)
 */
struct _CMD_TBL_ITER {
    TPCMD_ITEM tbl;
    size_t     size;
};

static struct _CMD_TBL_ITER tmp_iter;

TCMD_TBL_ITER cli_get_iterator(struct TCMD_ITEM tbl[], size_t tblsiz)
{
    tmp_iter.tbl  = &tbl[0];
    tmp_iter.size = tblsiz;
    return (TCMD_TBL_ITER)&tmp_iter;
}

    
TCMD_TBL_ITER cli_alloc_iterator(struct TCMD_ITEM tbl[], size_t tblsiz)
{
    struct _CMD_TBL_ITER *res;

    res = (struct _CMD_TBL_ITER *)malloc(sizeof(*res));
    if (NULL == res) {
        LOGFILE(LOG_ERROR, "Cannot allocate iterator for table");
    } else {
        // TODO: check tbl, which must be sorted in alpha-beta !
        res->tbl  = &tbl[0];
        res->size = tblsiz;
    }    
    
    return (TCMD_TBL_ITER)res;
}


void cli_free_iterator(TCMD_TBL_ITER iter)
{
    assert(NULL != iter);
    free(iter);
}


ssize_t cli_find_str(TCMD_TBL_ITER iter, const char *name)
{
    // Locate a string in the table with bi-search.
    TPCMD_ITEM tbl;
    int low, upper, mid; 

    assert(NULL != iter);
    tbl   = ((struct _CMD_TBL_ITER *)iter)->tbl;
    low   = 0; 
    upper = ((struct _CMD_TBL_ITER *)iter)->size - 1; 
    while (low <= upper) {
        register int ret;
        register const char *pc1, *pc2;
        mid = (low + upper);
        mid >>= 1;
        for (pc1 = tbl[mid].name, pc2 = name; *pc1 == *pc2; pc1++, pc2++) {
            if (*pc1 == '\0' || *pc2 == '\0')
                break;
        }
        // match identical
        ret = (*pc1 - *pc2);
        if (ret < 0) 
            low   = mid + 1; 
        else if(ret > 0) 
            upper = mid - 1; 
        else 
            return mid; 
    } 
    return -1;    
}

ssize_t cli_find_prefix(TCMD_TBL_ITER iter, const char *str)
{
    // Locate a string in the table with bi-search.
    TPCMD_ITEM tbl;
    int low, upper, mid; 

    assert(NULL != iter);
    tbl   = ((struct _CMD_TBL_ITER *)iter)->tbl;
    low   = 0; 
    upper = ((struct _CMD_TBL_ITER *)iter)->size - 1; 

    while (low <= upper) {
        register int ret;
        register const char *pc1, *pc2;
        mid = (low + upper);
        mid >>= 1;
        for (pc1 = tbl[mid].name, pc2 = str; *pc1 == *pc2; pc1++, pc2++) {
            if (*pc1 == '\0' || *pc2 == '\0')
                break;
        }
        // match pc1 as prefix
        ret = ((*pc1 == '\0') ? 0 : (*pc1 - *pc2));
        if (ret < 0) 
            low   = mid + 1; 
        else if(ret > 0) 
            upper = mid - 1; 
        else 
            return mid; 
    } 
    return -1;    
}

ssize_t cli_match_str(TCMD_TBL_ITER iter, const char *name,
                      OUT size_t match_list[], size_t lstsiz)
{
    // TO BE DONE.
    return -1;
}

