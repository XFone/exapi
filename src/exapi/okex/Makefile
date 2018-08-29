#
# $Id: $
#
# Makefile for Digital Currency Exchange - OKEX/OKCoin
#
# Copyright (c) 2014-2018 . All rights reserved.
#
# $Log: Makefile,v $
#
#

BUILD = ../../../build
SUBDIRS = agent
RULESET = CLANG

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

# include my rules
-include config.mk

# Build target (TARGET_LIB, TARGET_SHLIB or TARGET_PRG)
TARGET_LIB = libexapi-okex
BUILD_SHARED_LIBRARY := 1

# Depended Modules (libraries)
MODULES += PTHREAD IUTILS UUID OPENSSL RESTBED

# CFLAGS, CXXFLAGS and STDCCFLAGS
CXXFLAGS += -fPIC                  # for linking to shared library
CXXFLAGS += -std=c++0x             # for <atomic> and lambda
CXXFLAGS += -UDEBUG                # conficts with corvusoft/restbed/logger.hpp

# Additional arguments for linker
#LDFLAGS += -Wl,-t

# Sources to build
SOURCES = $(wildcard *.cc *.cxx)
INCDIRS  += ./include ../common/include

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
	@mkdir -p $(INSTALL_ROOT)/include/okex
	@mkdir -p $(INSTALL_ROOT)/$(OSTYPE)/bin 
	@mkdir -p $(INSTALL_ROOT)/$(OSTYPE)/lib
	@cp -a include/*  $(INSTALL_ROOT)/include/okex/
	-@cp -a ./agent/$(BINDIR)/*     $(INSTALL_ROOT)/$(OSTYPE)/bin
	-@cp -a $(BINDIR)/*             $(INSTALL_ROOT)/$(OSTYPE)/lib
	-@cp -a $(OBJDIR)/*.a           $(INSTALL_ROOT)/$(OSTYPE)/lib

# Makefile rules post processing
include $(BUILD)/Makefile.post
