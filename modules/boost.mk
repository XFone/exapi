#
# $Id: $
#
# Module BOOST: Boost (http://www.boost.org) provides free peer-reviewed 
#               portable C++ source libraries.  
#
# Copyright (c) 2014-2018 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

BOOST_SDK  = $(ROOT)/libs/build/boost
BOOST_DEFINES := USE_BOOST
BOOST_LIBNAME := boost_system
BOOST_LDFLAGS := 

# check 'brew install boost@1.60'
ifeq (darwin, $(OSTYPE))
  BREW_INSTALL_PATH = $(wildcard /usr/local/opt/boost)
  ifneq (, $(BREW_INSTALL_PATH))
    BOOST_ROOT    := $(BREW_INSTALL_PATH)
    BOOST_INCDIRS := $(BOOST_ROOT)/include
    BOOST_LIBDIRS := $(BOOST_ROOT)/lib
    BOOST_LDPATH  := $(BOOST_LIBDIRS)
  endif
  BOOST_CXXFLAGS := -Wno-unused-local-typedef
endif

# check 'yum install boost-devel'
ifeq (linux, $(OSTYPE))
  PKG_INSTALL_PATH = $(wildcard /opt/boost-1.6*)
  ifneq (, $(PKG_INSTALL_PATH))
    BOOST_ROOT    := $(PKG_INSTALL_PATH)
    BOOST_INCDIRS := $(BOOST_ROOT)/include
    BOOST_LIBDIRS := $(BOOST_ROOT)/lib
    BOOST_LDPATH  := $(BOOST_LIBDIRS)
  endif
endif

# Use libs built from source
ifeq (, $(BOOST_ROOT))
  BOOST_ROOT    := $(ROOT)/libs/boost-1.66.0
  BOOST_INCDIRS := $(BOOST_ROOT)/include
  BOOST_LIBDIRS := $(BOOST_ROOT)/$(OSTYPE)/lib
  BOOST_LDPATH  := $(BOOST_LIBDIRS)
  BOOST_DEPENDS := $(BOOST_LIBDIRS)/libboost_system.a
endif

$(BOOST_DEPENDS):
	$(call make_module,BOOST,$(BOOST_SDK))
