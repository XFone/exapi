#
# $Id: $
#
# Module EVENT: the LibEvent(http://libevent.org) library
#
# Copyright (c) 2014-2018 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

EVENT_SOURCE = $(ROOT)/libs/build/event
EVENT_LIBNAME := event
EVENT_DEFINES := USE_LIBEVENT
# EVENT_LDFLAGS := -lrt

# check 'brew install libevent'
ifeq (darwin, $(OSTYPE))
  BREW_INSTALL_PATH = $(wildcard /usr/local/opt/libevent)
  ifneq (, $(BREW_INSTALL_PATH))
    EVENT_ROOT    := $(BREW_INSTALL_PATH)
    EVENT_INCDIRS := $(EVENT_ROOT)/include
    EVENT_LIBDIRS := $(EVENT_ROOT)/lib
  endif
endif

# check 'yum install libevent' or 'apt install libevent'
ifeq (linux, $(OSTYPE))
  PKG_INSTALL_PATH = $(wildcard /opt/libevent-2.*)
  ifneq (, $(PKG_INSTALL_PATH))
    EVENT_ROOT    := $(PKG_INSTALL_PATH)
    EVENT_INCDIRS := $(EVENT_ROOT)/include
    EVENT_LIBDIRS := $(EVENT_ROOT)/lib
  endif
endif

# Use libs built from source
ifeq (, $(EVENT_ROOT))
  EVENT_ROOT   = $(ROOT)/libs/libevent-2.1.8
  EVENT_INCDIRS := $(EVENT_ROOT)/include
  EVENT_LIBDIRS := $(EVENT_ROOT)/$(OSTYPE)/lib
  EVENT_DEPENDS := $(EVENT_LIBDIRS)/libevent.a
endif

$(EVENT_DEPENDS):
	$(call make_module,EVENT,$(EVENT_SOURCE))
