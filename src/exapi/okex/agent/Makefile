#
# $Id: $
#
# Makefile for Digital Currency Exchange
#
# Copyright (c) 2014-2018 . All rights reserved.
#
# $Log: Makefile,v $
#
#

BUILD = ../../../../build
SUBDIRS =
RULESET = CLANG

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

# include my rules
-include config.mk

# Build target (TARGET_LIB, TARGET_SHLIB or TARGET_PRG)
TARGET_PRG = exapi-okex

# Depended Modules (libraries)
MODULES += PTHREAD IUTILS UUID CJSON OPENSSL RESTBED PROTOBUF ZMQ4

# CFLAGS, CXXFLAGS and STDCCFLAGS
CXXFLAGS += -std=c++0x             # for <atomic> and lambda
CXXFLAGS += -UDEBUG                # conficts with corvusoft/restbed/logger.hpp

# Additional LDFLAGS
# LDFLAGS += -Wl,-t

# Sources to build
SOURCES = $(wildcard *.cc ../*.cc ../*.cxx ../../common/*.cc)
INCDIRS  += ../include ../../common/include

# All
all::
	@set -e
	@echo "---------------------------------------------------------------"
	@echo "               Production Release ($(COMPILE_TYPE))."
	@echo "---------------------------------------------------------------"

# Makefile rules post processing
include $(BUILD)/Makefile.post
