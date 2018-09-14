#
# $Id: $
#
# Module TBB: Intel(c) TBB (Threading Building Blocks) 4.0
#
# Copyright (c) 2014-2018 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

TBB_SDK = $(ROOT)/libs/build/tbb
TBB_DEFINES := USE_TBB
TBB_LIBNAME := tbb tbbmalloc
TBB_LDFLAGS :=

# check 'brew install tbb'
ifeq (darwin, $(OSTYPE))
  BREW_INSTALL_PATH = $(wildcard /usr/local/opt/tbb)
  ifneq (, $(BREW_INSTALL_PATH))
    TBB_ROOT    := $(BREW_INSTALL_PATH)
    TBB_INCDIRS := $(TBB_ROOT)/include
    TBB_LIBDIRS := $(TBB_ROOT)/lib
    TBB_LDPATH  := $(TBB_LIBDIRS)
  endif
endif

# check 'yum install tbb-devel' or 'apt install tbb-devel'
ifeq (linux, $(OSTYPE))
  PKG_INSTALL_PATH = $(wildcard /opt/tbb-2018*)
  ifneq (, $(PKG_INSTALL_PATH))
    TBB_ROOT    := $(PKG_INSTALL_PATH)
    TBB_INCDIRS := $(TBB_ROOT)/include
    TBB_LIBDIRS := $(TBB_ROOT)/lib
    TBB_LDPATH  := $(TBB_LIBDIRS)
  endif
endif

# Use libs built from source
ifeq (, $(TBB_ROOT))
  TBB_ROOT = $(ROOT)/libs/tbb-2018_U5
  TBB_INCDIRS := $(TBB_ROOT)/include
  TBB_LIBDIRS := $(TBB_ROOT)/$(OSTYPE)/lib
  TBB_LDPATH  := $(TBB_LIBDIRS)
  TBB_DEPENDS := $(TBB_LIBDIRS)/libtbb.$(SHLIB_SUFFIX)
endif

$(TBB_DEPENDS):
	$(call make_module,TBB,$(TBB_SDK),$(TBB_ROOT))