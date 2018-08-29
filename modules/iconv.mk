#
# $Id: $
#
# Module ICONV: GNU libiconv
#
# Copyright (c) 2013-2018 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

ICONV_LIBNAME := iconv
ICONV_DEFINES := USE_ICONV

ifeq (linux,$(OSTYPE))
  ICONV_SRC      = $(ROOT)/libs/build/iconv
  ICONV_ROOT     = $(ROOT)/libs/iconv-1.14
  ICONV_INCDIRS := $(ICONV_ROOT)/include
  ICONV_LIBDIRS := $(ICONV_ROOT)/$(OSTYPE)/lib
  ICONV_LDPATH  := $(ICONV_LIBDIRS)
  ICONV_DEPENDS := $(ICONV_LIBDIRS)/libiconv.so
else
  ICONV_DEPENDS := 
endif

$(ICONV_DEPENDS):
	$(call make_module,ICONV,$(ICONV_SRC))
	
