#
# $Id: $
#
# Makefile for build libevent library
#
# Copyright (c) 2014-2015 ZeroneIO. All rights reserved.
#
# $Log: Makefile,v $
#

BUILD = ../../../build
SUBDIRS = 

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

# source url
SRC_URL = https://github.com/libevent/libevent/releases/download/release-2.1.8-stable/libevent-2.1.8-stable.tar.gz

# libevent makefile variables
LIBEVENT_VER = libevent-2.1.8
LIBEVENT_SDK = $(abspath ./$(LIBEVENT_VER))
LIBEVENT_DST = $(abspath ../../$(LIBEVENT_VER))

CONF_ENV = CC=clang CXX=clang++

$(LIBEVENT_SDK)/configure:
	cd $(LIBEVENT_SDK) ; ./autogen.sh

$(LIBEVENT_SDK)/Makefile: $(LIBEVENT_SDK)/configure
	cd $(LIBEVENT_SDK) ; $(CONF_ENV) ./configure --disable-shared --disable-openssl

$(LIBEVENT_DST):
	@mkdir $(LIBEVENT_DST)

$(LIBEVENT_SDK):
	@echo "⬇️  Downloading $(SRC_URL)..."
	@wget -c $(SRC_URL) -O $(LIBEVENT_VER).tgz
	@tar zxf $(LIBEVENT_VER).tgz

all:: $(LIBEVENT_SDK) $(LIBEVENT_SDK)/Makefile
	@$(MAKE) -C $(LIBEVENT_SDK)

clean:
	-@$(MAKE) -C $(LIBEVENT_SDK) clean
	-rm -rf $(LIBEVENT_SDK)/configure

install: $(LIBEVENT_DST)
	-mkdir -p $(LIBEVENT_DST)/include
	-cp -a $(LIBEVENT_SDK)/include/event2 $(LIBEVENT_DST)/include
	-mkdir -p $(LIBEVENT_DST)/$(OSTYPE)/lib
	-cp -a $(LIBEVENT_SDK)/.libs/*.a $(LIBEVENT_DST)/$(OSTYPE)/lib
	-cp -a $(LIBEVENT_SDK)/.libs/*.lai $(LIBEVENT_DST)/$(OSTYPE)/lib

# Makefile rules post processing
# include $(BUILD)/Makefile.post
