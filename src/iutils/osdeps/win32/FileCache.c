/*
 * $Id: $
 *
 * File Cache (File Mapping) functions
 *
 * Copyright (c) 2004-2006. All rights reserved.
 *
 * $Log:  $
 *
 */

#include "../../Base.h"
#include "../../Log.h"

#include "../FileCache.h"

static DWORD g_dwSysGran = 65536;

void InitFileCacheSystem()
{
    SYSTEM_INFO SysInfo;
    GetSystemInfo(&SysInfo);
    g_dwSysGran = SysInfo.dwAllocationGranularity;
    LOGFILE(LOG_DEBUG, "System allocation granularity is %d", g_dwSysGran);
}

DWORD LocateFileMapping(DWORD start, DWORD *pSize, DWORD *pDelta)
{
    DWORD result;
    DWORD size;

    // To calculate where to start the file mapping, round down the
    // offset of the data into the file to the nearest multiple of the
    // system allocation granularity. 
    result = (start / g_dwSysGran) * g_dwSysGran;
    LOGFILE(LOG_DEBUG, 
            "The file map view starts at %ld bytes into the file.",
            result);

    // Calculate the size of the file mapping view.
    size = (result % g_dwSysGran) + *pSize;
    *pSize = size;
    LOGFILE(LOG_DEBUG, "The file map view size is %ld bytes", size);

    // How large will the file object be?
    LOGFILE(LOG_DEBUG, "The file object is %ld bytes", start + size);

    // The data of interest isn't at the beginning of the
    // view, so determine how far into the view to set the pointer.
    assert(NULL != pDelta);
    *pDelta = start - result;
    LOGFILE(LOG_DEBUG, "The data is %d bytes into the view", *pDelta);

    return result;
}

BOOL RemapFileView(HANDLE hFile, HANDLE *p_hMapFile, void **p_pMapAddr,
                   DWORD start, DWORD size, BOOL readonly)
{
    BOOL result;
    HANDLE hMapFile;
    void * pMapAddr;
    DWORD flProtect;

    assert(INVALID_HANDLE_VALUE != hFile);
    assert(NULL != p_hMapFile);
    assert(NULL != p_pMapAddr);

    if (NULL != (hMapFile = *p_hMapFile)) {
        if (NULL != (pMapAddr = *p_pMapAddr)) {
            UnmapViewOfFile(pMapAddr);
        }
        CloseHandle(hMapFile);
    }

    flProtect = readonly ? PAGE_READONLY : PAGE_READWRITE;

    // Create a file mapping object for the file 
    hMapFile = CreateFileMapping(hFile,           // current file handle
                                 NULL,            // default security
                                 flProtect,       // read/write permission
                                 0,               // size of file object, high
                                 start + size,    // size of file object, low
                                 NULL);           // name of mapping object

    if (NULL == hMapFile) {
        LOGFILE(LOG_WARN, "Failed to create file mapping: %d", GetLastError());
        result = FALSE;
    } else {
        DWORD dwDesiredAccess = readonly ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS;

        // Map the view
        pMapAddr = MapViewOfFile(hMapFile,       // handle to mapping object
                                 dwDesiredAccess,// read/write 
                                 0,              // file offset(high)
                                 start,          // file offset(low)
                                 size);          // number of bytes to map

        if (NULL == pMapAddr) {
            LOGFILE(LOG_ERROR, "Failed map view of file: %d", GetLastError());
            result = FALSE;
        } else {
            LOGFILE(LOG_DEBUG, "File map address 0x%X", pMapAddr);
            *p_hMapFile = hMapFile;
            *p_pMapAddr = pMapAddr;

            result = TRUE;
        }
    }

    return result;
}

void *RemapViewAddress(HANDLE hMapFile, void *pMapAddr,
                       DWORD start, DWORD size, BOOL readonly)
{
    void *result;
    DWORD dwDesiredAccess = readonly ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS;

    assert(NULL != hMapFile);

    if (NULL != pMapAddr) {
        UnmapViewOfFile(pMapAddr);
    }

    result = MapViewOfFile(hMapFile,          // handle to mapping object
                           dwDesiredAccess,   // read/write 
                           0,                 // file offset(high)
                           start,             // file offset(low)
                           size);             // number of bytes to map

    if (NULL == result) {
        LOGFILE(LOG_ERROR, "Failed map view of file: %d", GetLastError());
    } else {
        LOGFILE(LOG_DEBUG, "File map address 0x%X", result);
    }

    return result;
}
