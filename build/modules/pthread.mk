#
# $Id: $
#
# Module PTHREAD: support for POSIX THREAD.
#
# Copyright (c) 2013. @Company. All rights reserved.
#
# $Log: $
#
#

PTHREAD_LIBNAME := pthread
PTHREAD_DEFINES := USE_PTHREAD
PTHREAD_CFLAGS  := -pthread

ifeq ($(OSTYPE), hpux)
    LIBNAMES += rt
endif

# only for x86: xiaofeng 13-03-21 TO BE CLEAR
ifeq (, $(TARGET_ARCH))
  ifneq (, $(findstring x86, $(MACHTYPE)))
    PTHREAD_DEFINES += USE_POSIX_SEMAPHORE USE_RECURSIVE_MUTEX REENTRANT
  endif
endif

