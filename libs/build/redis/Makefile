#
# $Id: $
#
# Makefile for build Redis library
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
SRC_URL = http://download.redis.io/releases/redis-4.0.11.tar.gz

# Redis makefile variables
REDIS_VER = redis-4.0.11
REDIS_SRC = $(abspath ./$(REDIS_VER))
REDIS_DST = $(abspath ../../$(REDIS_VER))

CONF_ENV = CC=clang CXX=clang++

$(REDIS_DST):
	@mkdir -p $(REDIS_DST)

$(REDIS_SRC):
	@echo "⬇️  Downloading $(SRC_URL)..."
	@wget -c $(SRC_URL) -O $(REDIS_VER).tgz
	@tar zxf $(REDIS_VER).tgz

all:: $(REDIS_SRC)
	@$(CONF_ENV) $(MAKE) -C $(REDIS_SRC)/deps/hiredis static
	@$(CONF_ENV) $(MAKE) -C $(REDIS_SRC)/deps/lua $(OSTYPE)
	@$(CONF_ENV) $(MAKE) -C $(REDIS_SRC)

clean:
	-@$(MAKE) -C $(REDIS_SRC) clean
	-@$(MAKE) -C $(REDIS_SRC)/deps/lua clean
	-@$(MAKE) -C $(REDIS_SRC)/deps/hiredis clean


install: $(REDIS_DST)
	-mkdir -p $(REDIS_DST)/include/hiredis/
	-mkdir -p $(REDIS_DST)/$(OSTYPE)/lib
	-cp -a $(REDIS_SRC)/deps/hiredis/*.h $(REDIS_DST)/include/hiredis/
	-cp -a $(REDIS_SRC)/deps/hiredis/*.a $(REDIS_DST)/$(OSTYPE)/lib

# Makefile rules post processing
# include $(BUILD)/Makefile.post
