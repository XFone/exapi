#
# $Id: $
#
# Makefile for build Protocol Buffers - Google's data interchange format
#     protobuf-3.x.0 requires:
#         googlemock-release-1.7.0 ==> ./gmock
#         googletest-release-1.7.0 ==> ./gmock/gtest
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
SRC_URL = https://github.com/google/protobuf/archive/v3.6.1.tar.gz

# protobuf makefile variables
PROTOBUF_VER = protobuf-3.6.1
PROTOBUF_SDK = $(abspath ./$(PROTOBUF_VER))
PROTOBUF_DST = $(abspath ../../$(PROTOBUF_VER))

CONF_ENV = CC=clang CXX=clang++

$(PROTOBUF_SDK)/configure:
	cd $(PROTOBUF_SDK) ; ./autogen.sh

$(PROTOBUF_SDK)/.config: $(PROTOBUF_SDK)/configure
	cd $(PROTOBUF_SDK) ; $(CONF_ENV) ./configure

$(PROTOBUF_DST):
	@mkdir -p $(PROTOBUF_DST)/include

$(PROTOBUF_SDK):
	@echo "⬇️  Downloading $(SRC_URL)..."
	@wget -c $(SRC_URL) -O $(PROTOBUF_VER).tgz
	@tar zxf $(PROTOBUF_VER).tgz

all:: $(PROTOBUF_SDK) $(PROTOBUF_SDK)/.config
	@$(MAKE) -C $(PROTOBUF_SDK)

clean:
	-@$(MAKE) -C $(PROTOBUF_SDK) clean
	-rm -rf $(PROTOBUF_SDK)/configure

install: $(PROTOBUF_DST)
	@mkdir -p $(PROTOBUF_DST)/$(OSTYPE)/bin
	@mkdir -p $(PROTOBUF_DST)/$(OSTYPE)/lib
	-cp -a $(PROTOBUF_SDK)/src/google $(PROTOBUF_DST)/include
	-cp -a $(PROTOBUF_SDK)/src/.libs/libproto* $(PROTOBUF_DST)/$(OSTYPE)/bin
	-cp -a $(PROTOBUF_SDK)/src/.libs/protoc $(PROTOBUF_DST)/$(OSTYPE)/bin
	-rm -rf `find $(PROTOBUF_DST) -name '*.o'`
	-rm -rf `find $(PROTOBUF_DST) -name '*.cc'`
	-rm -rf `find $(PROTOBUF_DST) -name '.libs'`
	-rm -rf `find $(PROTOBUF_DST) -name '*.a' -or -name '*.lai'`
	-cp -a $(PROTOBUF_SDK)/src/.libs/*.a $(PROTOBUF_DST)/$(OSTYPE)/lib
	-cp -a $(PROTOBUF_SDK)/src/.libs/*.lai $(PROTOBUF_DST)/$(OSTYPE)/lib

# Makefile rules post processing
# include $(BUILD)/Makefile.post
