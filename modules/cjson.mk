#
# $Id: $
#
# Module CJSON: cJSON is implemented in pure C language and lightweight
#
# Copyright (c) 2013-2018 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

CJSON_SDK   = $(ROOT)/libs/build/cjson
CJSON_DEFINES := USE_CJSON
CJSON_LIBNAME := cjson

# Use libs built from source
ifeq (, $(CJSON_ROOT))
  CJSON_ROOT 	  := $(ROOT)/libs/cjson-1.7.7
  CJSON_INCDIRS := $(CJSON_ROOT)/include
  CJSON_LIBDIRS := $(CJSON_ROOT)/$(OSTYPE)/lib
  CJSON_LDPATH  := $(CJSON_LIBDIRS)
  CJSON_DEPENDS := $(CJSON_LIBDIRS)/libcjson.a
endif

$(CJSON_DEPENDS):
	$(call make_module,CJSON,$(CJSON_SDK))
