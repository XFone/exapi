/*
 * $Id: $
 *
 * Routines for CLI (Command Line Interface) Parsing
 *
 * Copyright (c) 2009. All rights reserved.
 *
 * $Log: $
 *
 */

#ifndef _UTILS_CLI_H_
#define _UTILS_CLI_H_

#ifdef WIN32

typedef int     ssize_t;

#endif

BEGIN_EXTERN_C

#define INVALID_CMD_CODE            -1 /**< invalid code      */

/** Pointer to Command Code Table Item.
 */
typedef struct TCMD_ITEM *TPCMD_ITEM;

/** Command Code Table Item.
 */
typedef struct TCMD_ITEM {
    const char * name;          /**< string name (or index value )   */
    int          code;          /**< used defined enumerators        */
} TCMD_ITEM;

/** Command Code Table Iterator.
 * Iterator of the lookup table (or hash-table in C++)
 */
typedef void *TCMD_TBL_ITER;

/** Get an temporary iterator for lookup table.
 * NOTE: no need to free
 * @param tbl command string-code table
 * @param tblsiz items in table
 * @return the iterator.
 */
TCMD_TBL_ITER cli_get_iterator(struct TCMD_ITEM tbl[], size_t tblsiz);

/** Allocate an iterator for lookup table
 * NOTE: should be freed with cli_free_iterator
 * @param tbl command string-code table
 * @param tblsiz items in table
 * @return the iterator allocated.
 */
TCMD_TBL_ITER cli_alloc_iterator(struct TCMD_ITEM tbl[], size_t tblsiz);

/** Free the lookup table
 * @param iter the iterator.
 * @return none.
 */
void cli_free_iterator(TCMD_TBL_ITER iter);

/** Find a string in the command table.
 * The string must be identical.
 * @param iter iterator of lookup table
 * @param name the string to find
 * @return index number to the command string-code table, -1 if not found.
 */
ssize_t cli_find_str(TCMD_TBL_ITER iter, const char *name);

/** Find a string prefix in the command table.
 * The name in table is identical to the given string, or a prefix of the
 * string. 
 * @param iter iterator of lookup table
 * @param str the string to find
 * @return index number to the command string-code table, -1 if not found.
 */
ssize_t cli_find_prefix(TCMD_TBL_ITER iter, const char *str);

/** Try to match a string in the command table.
 * NOTE: in current implementation, we matches prefix only
 * @param iter iterator of lookup table
 * @param name the string to match
 * @param match_list list to save matched index.
 * @param lstsiz size of match_list
 * @return count items matched, index number is saved in match_list.
 */
ssize_t cli_match_str(TCMD_TBL_ITER iter, const char *name,
                      OUT size_t match_list[], size_t lstsiz);

END_EXTERN_C

#endif /* _UTILS_CLI_H_ */
