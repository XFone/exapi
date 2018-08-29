#
# $Id: $
#
# Build rules for $OSTYPE = Darwin (Mac OS)
#
# Copyright (c) 2010-2016 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

############################## Darwin ################################

ifeq (darwin, $(OSTYPE))
    YACC       := bison
    LEX        := flex
    CFLAGS     := 
    DFLAGS     := -w
    CXXFLAGS   := 
    PREP_FLAGS := -E
    FIND       := find
    REMOVE     := rm -f
    STDCCFLAGS += -DP_DARWIN=$(OSRELEASE)
    SHLIB_SUFFIX := dylib
endif

