#
# $Id: $
#
# Module REDIS: the hiredis C/C++ Client: Minimalistic client for Redis
#
# Copyright (c) 2013-2018 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

REDIS_SDK   = $(ROOT)/libs/build/redis
REDIS_ROOT  = $(ROOT)/libs/redis-4.0.11
REDIS_INCDIRS := $(REDIS_ROOT)/include
REDIS_LIBDIRS := $(REDIS_ROOT)/$(OSTYPE)/lib
REDIS_LIBNAME :=
REDIS_DEPENDS := $(REDIS_LIBDIRS)/libhiredis.a
REDIS_DEFINES := USE_REDIS

$(REDIS_DEPENDS):
	$(call make_module,REDIS,$(REDIS_SDK))
