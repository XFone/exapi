#
# $Id: $
#
# Build rules for $OSTYPE = cygwin
#
# Copyright (c) 2010-2013 @Company. All rights reserved.
#
# $Log: $
#
#

############################ cygwin ##################################

ifeq ($(OSTYPE),cygwin)
    YACC       := bison
    LEX        := flex
    CFLAGS     += -D_REENTRANT
    CXXFLAGS   += -D_REENTRANT
    PREP_FLAGS := -E
    USE_LIBGETOPT = 1
#   CFLAGS += -I$(ROOT)/contrib/win32/mingw/include \
#             -I$(ROOT)/contrib/win32/pthreads
#   CXXFLAGS += -I$(ROOT)/contrib/win32/mingw/include \
#               -I$(ROOT)/contrib/win32/pthreads
    CXXFLAGS += -I/usr/include/g++-3

endif
