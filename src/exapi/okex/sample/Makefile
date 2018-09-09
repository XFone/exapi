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
RULESET =

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

# include my rules
-include config.mk

# Build target (TARGET_LIB, TARGET_SHLIB or TARGET_PRG)
TARGET_PRG = test-okex-spi

# Depended Modules (libraries)
MODULES += PTHREAD IUTILS UUID CJSON OPENSSL RESTBED

# CFLAGS, CXXFLAGS and STDCCFLAGS
CXXFLAGS += -std=c++0x             # for <atomic> and lambda
CXXFLAGS += -UDEBUG                # conficts with corvusoft/restbed/logger.hpp

# Additional LDFLAGS
# LDFLAGS += -Wl,-t

# for UNIT_TEST and LOGFILE: dont print filename and lineno
STDCCFLAGS += -DNO_FILE_LINE

# Sources to build
SOURCES = $(wildcard *.cc ../*.cc ../../common/*.cc)
INCDIRS  += ../include ../../common/include

# All
all::
	@set -e
	@echo "---------------------------------------------------------------"
	@echo "               Production Release ($(COMPILE_TYPE))."
	@echo "---------------------------------------------------------------"

# Test
test: all
	@echo "🌞  Testing..."
	@$(LD_PREFIX) $(BINDIR)/$(TARGET_PRG) -t 7 -v 7 2>&1

# Makefile rules post processing
include $(BUILD)/Makefile.post
