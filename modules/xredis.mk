#
# $Id: $
#
# Module XREDIS: the Redis C/C++ Client: xRedis (derived from Swordfish libx-1.0)
#
# Copyright (c) 2013-2015 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

XREDIS_ROOT     = $(ROOT)/src/xredis
XREDIS_INCDIRS := $(XREDIS_ROOT)/include
XREDIS_LIBDIRS := $(XREDIS_ROOT)/$(OBJDIR)
XREDIS_LIBNAME := xredis
XREDIS_DEPENDS := $(XREDIS_LIBDIRS)/libxredis.a
XREDIS_DEFINES := USE_XREDIS

$(XREDIS_DEPENDS):
	$(call make_module,XREDIS,$(XREDIS_ROOT))
