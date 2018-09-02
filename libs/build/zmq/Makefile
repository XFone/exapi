#
# $Id: $
#
# Makefile for build ZeroMQ 4.x library
#
# Copyright (c) 2014-2018 ZeroneIO. All rights reserved.
#
# $Log: Makefile,v $
#

BUILD = ../../../build
SUBDIRS = 

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

# source url
SRC_URL = https://github.com/zeromq/libzmq/releases/download/v4.2.3/zeromq-4.2.3.tar.gz

# zeromq makefile variables
ZEROMQ_VER = zeromq-4.2.3
ZEROMQ_SDK = $(abspath ./$(ZEROMQ_VER))
ZEROMQ_DST = $(abspath ../../$(ZEROMQ_VER))

CONF_ENV = CC=clang CXX=clang++

$(ZEROMQ_SDK)/configure:
	cd $(ZEROMQ_SDK) ; ./autogen.sh

$(ZEROMQ_SDK)/.config: $(ZEROMQ_SDK)/configure
	cd $(ZEROMQ_SDK) ; $(CONF_ENV) ./configure --without-libsodium

$(ZEROMQ_DST):
	@mkdir -p $(ZEROMQ_DST)

$(ZEROMQ_SDK):
	@echo "⬇️  Downloading $(SRC_URL)..."
	@wget -c $(SRC_URL) -O $(ZEROMQ_VER).tgz
	@tar zxf $(ZEROMQ_VER).tgz

all:: $(ZEROMQ_SDK) $(ZEROMQ_SDK)/.config
	@$(MAKE) -C $(ZEROMQ_SDK)

clean:
	-@$(MAKE) -C $(ZEROMQ_SDK) clean
	-rm -rf $(ZEROMQ_SDK)/configure

install: $(ZEROMQ_DST)
	-mkdir -p $(ZEROMQ_DST)/include
	-cp -a $(ZEROMQ_SDK)/include/*.h $(ZEROMQ_DST)/include
	-mkdir -p $(ZEROMQ_DST)/$(OSTYPE)/lib
	-cp -a $(ZEROMQ_SDK)/src/.libs/* $(ZEROMQ_DST)/$(OSTYPE)/lib
	-rm -rf `find $(ZEROMQ_DST) -name '*.o'`


# Makefile rules post processing
# include $(BUILD)/Makefile.post
