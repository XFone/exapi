#
# $Id: $
#
# Module OPENSSL: openssl 1.0.2p
#
# Copyright (c) 2013-2018 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

OPENSSL_SDK_ROOT = $(ROOT)/libs/build/openssl
OPENSSL_LIBNAME := ssl crypto
OPENSSL_LDFLAGS := 
OPENSSL_DEFINES := USE_OPENSSL

# check 'brew install openssl'
ifeq (darwin, $(OSTYPE))
  BREW_INSTALL_PATH = $(wildcard /usr/local/opt/openssl)
  ifneq (, $(BREW_INSTALL_PATH))
    OPENSSL_ROOT    := $(BREW_INSTALL_PATH)
    OPENSSL_INCDIRS := $(OPENSSL_ROOT)/include
    OPENSSL_LIBDIRS := $(OPENSSL_ROOT)/lib
    OPENSSL_LDPATH  := $(OPENSSL_LIBDIRS)
  endif
endif

# check 'yum install openssl-devel'
ifeq (linux, $(OSTYPE))
  PKG_INSTALL_PATH = $(wildcard /usr/include/openssl)
  ifneq (, $(PKG_INSTALL_PATH))
    OPENSSL_ROOT    := /usr
   #OPENSSL_INCDIRS := $(OPENSSL_ROOT)/include
   #OPENSSL_LIBDIRS := $(OPENSSL_ROOT)/lib
   #OPENSSL_LDPATH  := $(OPENSSL_LIBDIRS)
  endif
endif

# build from source 
ifeq (, $(OPENSSL_ROOT))
  OPENSSL_ROOT  = $(ROOT)/libs/openssl-1.0.2p
  OPENSSL_INCDIRS := $(OPENSSL_ROOT)/include
  OPENSSL_LIBDIRS := $(OPENSSL_ROOT)/$(OSTYPE)/lib
  OPENSSL_DEPENDS := $(OPENSSL_LIBDIRS)/libssl.$(SHLIB_SUFFIX)
endif

$(OPENSSL_DEPENDS):
	$(call make_module,OPENSSL,$(OPENSSL_SDK_ROOT))
