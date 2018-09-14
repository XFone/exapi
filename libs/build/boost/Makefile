#
# $Id: $
#
# Makefile for build Boost C++ library 1.6x
#
# Copyright (c) 2014-2017 ZeroneIO. All rights reserved.
#
# $Log: Makefile,v $
#

BUILD = ../../../build
SUBDIRS = 

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

# source url
SRC_URL = https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz

# Boost makefile variables
BOOST_VER = boost-1.66.0
BOOST_SDK = $(abspath ./$(BOOST_VER))
BOOST_DST = $(abspath ../../$(BOOST_VER))
BUILDLIB = $(BOOST_SDK)/stage/lib/libboost_system.a

CONF_ENV = --with-toolset=clang

# boot source
$(BOOST_SDK):
	@echo "⬇️  Downloading $(SRC_URL)..."
	@wget -c $(SRC_URL) -O $(BOOST_VER).tgz
	@mkdir -p $(BOOST_VER)
	@tar zxf $(BOOST_VER).tgz -C $(BOOST_VER) --strip-components 1

$(BUILDLIB):
	@cd $(BOOST_SDK) ; ./bootstrap.sh $(CONF_ENV)
#	@cd $(BOOST_SDK) ; ./b2 --layout=versioned --build-type=complete
	@cd $(BOOST_SDK) ; ./b2

$(BOOST_DST):
	@mkdir $(BOOST_DST)

all:: $(BOOST_SDK) $(BUILDLIB)
	@echo "Building Boost C++ library done."

clean:
	-@$(MAKE) -C $(BOOST_SDK) clean
	-@cd $(BOOST_SDK) ; ./b2 --clean

install: $(BOOST_DST)
	-mkdir -p $(BOOST_DST)/include/boost
	-cp -a $(BOOST_SDK)/boost/* $(BOOST_DST)/include/boost
	-mkdir -p $(BOOST_DST)/$(OSTYPE)/lib
	-cp -a $(BOOST_SDK)/stage/lib/*.a $(BOOST_DST)/$(OSTYPE)/lib

# Makefile rules post processing
# include $(BUILD)/Makefile.post
