#
# $Id: $
#
# Build rules for $OSTYPE = linux
#
# Copyright (c) 2010-2014 @Company. All rights reserved.
#
# $Log: $
#
#

############################## Linux ################################

ifeq (linux, $(OSTYPE))
    YACC       := bison
    LEX        := flex
    CFLAGS     := 
    DFLAGS     := -w
    CXXFLAGS   := 
    PREP_FLAGS := -E
    FIND       := find
    REMOVE     := rm -f
    STDCCFLAGS += -DP_LINUX=$(OSRELEASE)
    SHLIB_SUFFIX := so
endif

