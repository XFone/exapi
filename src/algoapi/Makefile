#
# $Id: $
#
# Makefile for Algorithm/Auto Trading Server
#
# Copyright (c) 2014-2015 Zerone.IO. All rights reserved.
#
# $Log: Makefile,v $
#
#

BUILD = ../../build
SUBDIRS = 
RULESET = CLANG

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

# Build target (TARGET_LIB, TARGET_SHLIB or TARGET_PRG)
TARGET_LIB = algoapi

# Depended Modules (libraries)
MODULES += PROTOBUF IUTILS TBB XREDIS ZMQ4

# CFLAGS, CXXFLAGS and STDCCFLAGS
CXXFLAGS += -std=c++0x

# LDFLAGS
# LDFLAGS += -Wl,-t

# Sources to build
SOURCES = $(wildcard *.cc zmq/*.cc) 

PROTO_SOURCES = $(patsubst protos/%.proto, %.pb.cc, $(wildcard protos/*.proto))

# For linux 
ifeq (linux, $(OSTYPE))
  LD_PREFIX = LD_LIBRARY_PATH=$(LDPATHS)
endif

# For MacOS
ifeq (darwin, $(OSTYPE))
  LD_PREFIX = DYLD_LIBRARY_PATH=$(LDPATHS)
endif

%.pb.cc: protos/%.proto
	@printf '%b     %b\n' $(LD_COLOR)[PBC] $(BIN_COLOR)$<$(NO_COLOR)
	@$(LD_PREFIX) $(PROTOC) --cpp_out=. $<
	@mv protos/$@ .

proto::
	@echo "Generating pb sources..."
	@for FILE in $(PROTO_SOURCES) ; do rm -rf $$FILE && make $$FILE ; done

# Makefile rules post processing
include $(BUILD)/Makefile.post
