#
# $Id: $
#
# Module PROTOBUF: Protocol Buffers - Google's data interchange format
#
# Copyright (c) 2013-2018 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

PROTOBUF_SDK  = $(ROOT)/libs/build/protobuf
PROTOBUF_DEFINES := USE_PROTOBUF _THREAD_SAFE
PROTOBUF_LIBNAME := protobuf

# check 'brew install protobuf@3.6'
ifeq (darwin, $(OSTYPE))
  BREW_INSTALL_PATH = $(wildcard /usr/local/opt/protobuf)
  ifneq (, $(BREW_INSTALL_PATH))
    PROTOBUF_ROOT    := $(BREW_INSTALL_PATH)
    PROTOC           := $(PROTOBUF_ROOT)/bin/protoc
    PROTOBUF_INCDIRS := $(PROTOBUF_ROOT)/include
    PROTOBUF_LIBDIRS := $(PROTOBUF_ROOT)/lib
    PROTOBUF_LDPATH  := $(PROTOBUF_LIBDIRS)
  endif
endif

# check 'yum install protobuf-devel'
ifeq (linux, $(OSTYPE))
  PKG_INSTALL_PATH = $(wildcard /opt/protobuf-3.6*)
  ifneq (, $(PKG_INSTALL_PATH))
    PROTOBUF_ROOT    := $(PKG_INSTALL_PATH)
    PROTOC           := $(PROTOBUF_ROOT)/bin/protoc
    PROTOBUF_INCDIRS := $(PROTOBUF_ROOT)/include
    PROTOBUF_LIBDIRS := $(PROTOBUF_ROOT)/lib
    PROTOBUF_LDPATH  := $(PROTOBUF_LIBDIRS)
  endif
endif

# Use libs built from source
ifeq (, $(PROTOBUF_ROOT))
  PROTOBUF_ROOT    := $(ROOT)/libs/protobuf-3.6.1
  PROTOC           := $(PROTOBUF_ROOT)/$(OSTYPE)/bin/protoc
  PROTOBUF_INCDIRS := $(PROTOBUF_ROOT)/include
  PROTOBUF_LIBDIRS := $(PROTOBUF_ROOT)/$(OSTYPE)/lib
  PROTOBUF_LDPATH  := $(PROTOBUF_LIBDIRS) $(PROTOBUF_ROOT)/$(OSTYPE)/bin
 #PROTOBUF_DEPENDS := $(PROTOBUF_LIBDIRS)/libprotobuf.a
endif

$(PROTOBUF_DEPENDS):
	$(call make_module,PROTOBUF,$(PROTOBUF_SDK))

