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
TARGET_PRG = test-bitmex-quote test-bitmex-trade test-bitmex-wss

# Depended Modules (libraries)
MODULES += PTHREAD IUTILS UUID CJSON OPENSSL RESTBED

# CFLAGS, CXXFLAGS and STDCCFLAGS
CXXFLAGS += -std=c++0x             # for <atomic> and lambda
CXXFLAGS += -UDEBUG                # conficts with corvusoft/restbed/logger.hpp

# Additional LDFLAGS
# LDFLAGS += -Wl,-t
# LIBNAMES += z                    # zlib for deflate

# for UNIT_TEST and LOGFILE: dont print filename and lineno
STDCCFLAGS += -DNO_FILE_LINE

# Sources to build
SOURCES = $(wildcard ../*.cc ../json2c/*.cc ../../common/*.cc)
INCDIRS  += ../include ../json2c ../../common/include

SOURCES_test-bitmex-quote = test_spi.cc
SOURCES_test-bitmex-trade = test_trade.cc
SOURCES_test-bitmex-wss   = test_wss.cc

# All
all::
	@set -e
	@echo "---------------------------------------------------------------"
	@echo "               Production Release ($(COMPILE_TYPE))."
	@echo "---------------------------------------------------------------"

# Test
CONFIG_FILE = bitmex.conf

test: test-quote test-trade test-wss
	@echo "🌞  done."

test-quote: all
	@echo "🌞  Testing quote api..."
	@$(LD_PREFIX) $(BINDIR)/test-bitmex-quote -t 7 -v 7 -c $(CONFIG_FILE) 2>&1

test-trade: all
	@echo "🌞  Testing trader api..."
	@$(LD_PREFIX) $(BINDIR)/test-bitmex-trade -t 7 -v 7 -c $(CONFIG_FILE) 2>&1

test-wss: all
	@echo "🌞  Testing websocket api..."
	@$(LD_PREFIX) $(BINDIR)/test-bitmex-wss -t 7 -v 7 -c $(CONFIG_FILE) 2>&1

# Makefile rules post processing
include $(BUILD)/Makefile.post
