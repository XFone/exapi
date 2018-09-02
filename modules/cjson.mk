#
# $Id: $
#
# Module CJSON: cJSON is implemented in pure C language and lightweight
#
# Copyright (c) 2013-2015 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

CJSON_SDK   = $(ROOT)/libs/build/cjson
CJSON_ROOT  = $(ROOT)/libs/cjson-1.0
CJSON_INCDIRS := $(CJSON_ROOT)/include
CJSON_LIBDIRS := $(CJSON_ROOT)/$(OSTYPE)/lib
CJSON_LIBNAME := cjson
CJSON_DEPENDS := $(CJSON_LIBDIRS)/libcjson.a
CJSON_DEFINES := USE_CJSON

$(CJSON_DEPENDS):
	$(call make_module,CJSON,$(CJSON_SDK))
