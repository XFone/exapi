#
# $Id: $
#
# Makefile for build GNU libiconv
#
# Copyright (c) 2014-2015 ZeroneIO. All rights reserved.
#
# $Log: Makefile,v $
#

BUILD = ../../../build
SUBDIRS = 

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

# libiconv makefile variables
ICONV_VER = libiconv-1.14
ICONV_SRC = $(abspath ./$(ICONV_VER))
ICONV_DST = $(abspath ../../iconv-1.14)

CONF_ENV = CC=clang CXX=clang++

$(ICONV_SRC)/.config:
	cd $(ICONV_SRC) ; $(CONF_ENV) ./configure

$(ICONV_DST):
	@mkdir $(ICONV_DST)

all:: $(ICONV_SRC)/.config
	@$(MAKE) -C $(ICONV_SRC)

clean:
	-@$(MAKE) -C $(ICONV_SRC) clean
	-rm -rf $(ICONV_SRC)/.config

install: $(ICONV_DST)
	-mkdir -p $(ICONV_DST)/include
	-mkdir -p $(ICONV_DST)/$(OSTYPE)/lib
	-cp -a $(ICONV_SRC)/include/iconv.h $(ICONV_DST)/include
	-cp -a $(ICONV_SRC)/lib/.libs/libiconv.lai $(ICONV_DST)/$(OSTYPE)/lib
	-cp -a $(ICONV_SRC)/lib/.libs/*.so* $(ICONV_DST)/$(OSTYPE)/lib

# Makefile rules post processing
# include $(BUILD)/Makefile.post
