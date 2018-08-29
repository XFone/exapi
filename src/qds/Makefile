#
# $Id: $
#
# Makefile for Qutation Playback Server
#
# Copyright (c) 2014-2017 Zerone.IO (Shanghai). All rights reserved.
#
# $Log: Makefile,v $
#
#

BUILD = ../../build
SUBDIRS = 
# RULESET = CLANG

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

# include my rules
-include config.mk

# Build target (TARGET_LIB, TARGET_SHLIB or TARGET_PRG)
TARGET_PRG = qds

# Depended Modules (libraries)
MODULES += PTHREAD IUTILS UUID CJSON BOOST EVENT PROTOBUF ICONV
MODULES += XREDIS TBB CONCURRENT ZMQ4 PCAP

# CFLAGS, CXXFLAGS and STDCCFLAGS
CXXFLAGS += -std=c++0x                   # for <atomic> and lambda
#STDCCFLAGS += -Wno-\#pragma-messages    # not show pragma message

# Additional libraies (LIBNAMES)
LIBNAMES += boost_regex

# LDFLAGS
# LDFLAGS += -Wl,-t

# Sources to build
SOURCES = $(wildcard *.c*)                         \
          $(wildcard ./algoapi/*.c* ./timer/*.c*)

SOURCES_MPSS  = 

# Run in docker container 'devel' (centos7)
SRC_VOLUME = /devel/src/qds

docker:
	@echo "🐳  Running (in container) with 'make $(CMD)' ..."
	@docker-compose run --rm devel /bin/bash -c "make -C $(SRC_VOLUME) $(CMD)"

all::
	@set -e
	@echo "---------------------------------------------------------------"
	@echo "               Production Release ($(COMPILE_TYPE))."
	@echo "---------------------------------------------------------------"

# Makefile rules post processing
include $(BUILD)/Makefile.post
