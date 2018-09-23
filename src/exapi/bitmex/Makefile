#
# $Id: $
#
# Makefile for Digital Currency Exchange - BitMEX
#
# Copyright (c) 2014-2018 . All rights reserved.
#
# $Log: Makefile,v $
#
#

BUILD = ../../../build
SUBDIRS = agent sample
RULESET = CLANG

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

# include my rules
-include config.mk

# Build target (TARGET_LIB, TARGET_SHLIB or TARGET_PRG)
TARGET_LIB = libexapi-bitmex
BUILD_SHARED_LIBRARY := 1

# Depended Modules (libraries)
MODULES += PTHREAD IUTILS UUID CJSON OPENSSL RESTBED

# CFLAGS, CXXFLAGS and STDCCFLAGS
CXXFLAGS += -fPIC                  # for linking to shared library
CXXFLAGS += -std=c++0x             # for <atomic> and lambda
CXXFLAGS += -UDEBUG                # conficts with corvusoft/restbed/logger.hpp

# Additional arguments for linker
#LDFLAGS += -Wl,-t
LIBNAMES += z                      # zlib for deflate

# Sources to build
SOURCES = $(wildcard *.cc *.cxx ../common/*.cc ./json2c/*.cc)
INCDIRS += ./include ../common/include ./json2c

# All
all::
	@set -e
	@echo "---------------------------------------------------------------"
	@echo "               Production Release ($(COMPILE_TYPE))."
	@echo "---------------------------------------------------------------"

agent_bin:
	@$(MAKE) -C agent

# Install
INSTALL_ROOT=$(ROOT)/dist

install: all agent_bin
	@mkdir -p $(INSTALL_ROOT)/include/bitmex
	@mkdir -p $(INSTALL_ROOT)/$(OSTYPE)/bin 
	@mkdir -p $(INSTALL_ROOT)/$(OSTYPE)/lib
	@cp -a include/*  $(INSTALL_ROOT)/include/bitmex/
	-@cp -a ./agent/$(BINDIR)/*     $(INSTALL_ROOT)/$(OSTYPE)/bin
	-@cp -a $(BINDIR)/*             $(INSTALL_ROOT)/$(OSTYPE)/lib
	-@cp -a $(OBJDIR)/*.a           $(INSTALL_ROOT)/$(OSTYPE)/lib

# Makefile rules post processing
include $(BUILD)/Makefile.post
