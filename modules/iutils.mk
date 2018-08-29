#
# $Id: $
#
# Module IUTILS: provide platform independant support for cmd, log and trace.
#
# Copyright (c) 2013. @Company. All rights reserved.
#
# $Log: $
#
#

IUTILS_INCDIRS := $(ROOT)/src/iutils
IUTILS_LIBDIRS := $(ROOT)/src/iutils/$(OBJDIR)
IUTILS_LIBNAME := iutils
IUTILS_LDFLAGS :=
IUTILS_DEPENDS := $(IUTILS_LIBDIRS)/libiutils.a
IUTILS_DEFINES := USE_IUTILS

$(IUTILS_DEPENDS):
	$(call make_module,IUTILS,$(IUTILS_INCDIRS))
