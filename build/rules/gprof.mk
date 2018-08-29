#
# $Id: $
#
# Build rules for gprof
#
# Copyright (c) 2010-2013 @Company. All rights reserved.
#
# $Log: $
#
#

########################## For profiling ############################

# for gprof
ifeq ($(WITH_PROFILE), 1)
    CFLAGS   += -pg
    CXXFLAGS += -pg
    LDFLAGS  += -pg
endif
