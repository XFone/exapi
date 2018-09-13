#
# $Id: $
#
# Makefile for build cJSON library
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
SRC_URL = https://github.com/DaveGamble/cJSON/archive/v1.7.7.tar.gz

# cJSON makefile variables
CJSON_VER = cjson-1.7.7
CJSON_SRC = $(abspath ./$(CJSON_VER))
CJSON_DST = $(abspath ../../$(CJSON_VER))

# configure env
CONF_ENV = CC=clang CXX=clang++ CXXFLAGS="-std=c++0x -fPIC"

# restbed source
$(CJSON_SRC):
	@echo "⬇️  Downloading $(SRC_URL)..."
	@wget -c $(SRC_URL) -O $(CJSON_VER).tgz
	@mkdir -p $(CJSON_VER)
	@tar zxf $(CJSON_VER).tgz -C $(CJSON_VER) --strip-components 1

$(CJSON_SRC)/build/Makefile:
	@mkdir -p $(CJSON_SRC)/build
	@cd $(CJSON_SRC)/build ; $(CONF_ENV) cmake .. \
		-DENABLE_CJSON_TEST=Off -DENABLE_CJSON_UTILS=On \
		-DBUILD_SHARED_AND_STATIC_LIBS=On


all:: $(CJSON_SRC) $(CJSON_SRC)/build/Makefile
	@$(MAKE) -C $(CJSON_SRC)/build -j 4

clean:
	-@$(MAKE) -C $(CJSON_SRC)/build clean
	-rm -rf $(CJSON_SRC)/build

# cJSON install
$(CJSON_DST):
	@mkdir $(CJSON_DST)

install: $(CJSON_DST)
	-mkdir -p $(CJSON_DST)/include
	-cp -a $(CJSON_SRC)/*.h $(CJSON_DST)/include
	-mkdir -p $(CJSON_DST)/$(OSTYPE)/lib
	-cp -a $(CJSON_SRC)/build/*.a $(CJSON_DST)/$(OSTYPE)/lib

# Makefile rules post processing
# include $(BUILD)/Makefile.post
