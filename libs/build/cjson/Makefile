#
# $Id: $
#
# Makefile for build cJSON library
#
# Copyright (c) 2014-2017 ZeroneIO. All rights reserved.
#
# $Log: Makefile,v $
#

BUILD = ../../../build
SUBDIRS =

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

# cJSON makefile variables
CJSON_VER = cjson-1.0
CJSON_SRC = $(abspath ./$(CJSON_VER))
CJSON_DST = $(abspath ../../$(CJSON_VER))

$(CJSON_DST):
	@mkdir $(CJSON_DST)

$(CJSON_SRC)/Makefile:
	cp Makefile.in $(CJSON_SRC)/Makefile

all:: $(CJSON_SRC)/Makefile
	@$(MAKE) -C $(CJSON_SRC)

clean:
	-@$(MAKE) -C $(CJSON_SRC) clean

install: $(CJSON_DST)
	-mkdir -p $(CJSON_DST)/include
	-mkdir -p $(CJSON_DST)/$(OSTYPE)/lib
	-cp -a $(CJSON_SRC)/*.h $(CJSON_DST)/include
	-cp -a $(CJSON_SRC)/$(OBJDIR)/*.a $(CJSON_DST)/$(OSTYPE)/lib

# Makefile rules post processing
# include $(BUILD)/Makefile.post
