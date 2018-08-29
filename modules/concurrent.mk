#
# $Id: $
#
# Module CONCURRENT: C++ concurrent features (need C++11)
#
# Copyright (c) 2014-2018 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

CONCURRENT_ROOT = $(ROOT)/src/concurrent
CONCURRENT_INCDIRS := $(CONCURRENT_ROOT)
CONCURRENT_LIBDIRS := $(CONCURRENT_ROOT)/$(OBJDIR)
CONCURRENT_LIBNAME := concurrent
CONCURRENT_DEPENDS := $(CONCURRENT_LIBDIRS)/libconcurrent.a
CONCURRENT_DEFINES := USE_CONCURRENT

$(CONCURRENT_DEPENDS):
	$(call make_module,CONCURRENT,$(CONCURRENT_ROOT))

