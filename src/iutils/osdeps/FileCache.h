/*
 * $Id: $
 *
 * File Cache (File Mapping) for WIN32
 *
 * Copyright (c) 2004-2006. All rights reserved.
 *
 * $Log:  $
 *
 */

#ifndef _UTILS_OSDEPS_FILECACHE_H_
#define _UTILS_OSDEPS_FILECACHE_H_

/* For BYTE/WORD/DWORD types */
#if defined(__GNUC__)
# include <netinet/in.h>
#elif defined (WIN32) && !defined(NT_INCLUDED)
# include <winsock2.h>
#elif defined (WIN32) && defined(NT_INCLUDED)
# include <windef.h>
#endif /* __GNUC__ */

BEGIN_EXTERN_C

/** InitFileCacheSystem
 * Reset all system parameters for file caching
 */
void InitFileCacheSystem();

/** LocateFileMapping
 * Calculate where to start the file mapping, round down the offset of the 
 * data into the file to the nearest multiple of the system allocation 
 * granularity. 
 *
 * @param start current start offset
 * @param pSize IN OUT view of file mapping size
 * @param pDelta OUT offset to new mapping of the given start offset
 * @return new start offset
 */
DWORD LocateFileMapping(DWORD start, DWORD *pSize, DWORD *pDelta);

/** RemapFileView
 * Update file mapping with new size, and remap view of the file
 * @param hFile file handle
 * @param p_hMapFile point to the file mapping handle
 * @param p_pMapAddr point to the file mapping (cache) address
 * @param start start offset of the file
 * @param size view of file mapping size
 * @param readonly true if write access is not permitted
 * @return success
 */
BOOL RemapFileView(HANDLE hFile, HANDLE *p_hMapFile, void **p_pMapAddr,
                   DWORD start, DWORD size, BOOL readonly);


/** RemapViewAddress
 * remap view of the file
 * @param hMapFile point to the file mapping handle
 * @param pMapAddr old file mapping (cache) address
 * @param start start offset of the file
 * @param size view of file mapping size
 * @return new file mapping (cache) address
 * @param readonly true if write access is not permitted
 */
void *RemapViewAddress(HANDLE hMapFile, void *pMapAddr, 
                       DWORD start, DWORD size, BOOL readonly);

END_EXTERN_C

#endif /* _UTILS_OSDEPS_FILECACHE_H_ */

