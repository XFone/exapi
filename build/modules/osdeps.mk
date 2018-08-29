#
# $Id: $
#
# Module OSDEPS: support for OS dependant making.
#
# Copyright (c) 2013. @Company. All rights reserved.
#
# $Log: $
#
#

ifeq ($(OSTYPE), linux)
    SOURCES += $(SOURCES_LINUX)
endif

ifeq ($(OSTYPE), darwin)
    SOURCES += $(SOURCES_DARWIN)
endif
