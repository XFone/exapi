#
# $Id: $
#
# Makefile for build Intel TBB
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
SRC_URL = https://github.com/01org/tbb/archive/2018_U5.tar.gz

# protobuf makefile variables
TBB_VER = tbb2018_20180618oss
TBB_SDK = $(abspath ./tbb-2018_U5)
TBB_DST = $(abspath ../../tbb-2018_U5)

BLD_ENV = compiler=clang

# work_dir
compiler=clang
tbb_root?=$(TBB_SDK)
-include $(tbb_root)/build/common.inc

# lib path
_LIB_DEBUG   = $(work_dir)_debug
_LIB_RELEASE = $(work_dir)_release

$(TBB_DST):
	@mkdir -p $(TBB_DST)

$(TBB_SDK):
	@echo "⬇️  Downloading $(SRC_URL)..."
	@wget -c $(SRC_URL) -O $(TBB_VER).tgz
	@tar zxf $(TBB_VER).tgz

all:: $(TBB_SDK) $(TBB_SDK)/build
	@$(BLD_ENV) $(MAKE) -C $(TBB_SDK)

clean:
	-@$(MAKE) -C $(TBB_SDK) clean

install: $(TBB_DST)
	@mkdir -p $(TBB_DST)/include $(TBB_DST)/$(OSTYPE)/lib
	-cp -a $(TBB_SDK)/include/* $(TBB_DST)/include
	-cp -a $(_LIB_DEBUG)/libtbb* $(TBB_DST)/$(OSTYPE)/lib
	-cp -a $(_LIB_RELEASE)/libtbb* $(TBB_DST)/$(OSTYPE)/lib

# Makefile rules post processing
# include $(BUILD)/Makefile.post
