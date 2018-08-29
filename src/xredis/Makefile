#
# $Id: $
#
# Makefile for build Redis Client (Swordfish libx-1.0)
#
# Copyright (c) 2014-2015 ZeroneIO. All rights reserved.
#
# $Log: Makefile,v $
#

BUILD = ../../build
SUBDIRS = 
RULESET = CLANG 

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

# Build target (TARGET_LIB, TARGET_SHLIB or TARGET_PRG)
TARGET_LIB = libxredis

# Depended Modules (libraries)
MODULES += BOOST PROTOBUF REDIS

# CFLAGS, CXXFLAGS and STDCCFLAGS
CXXFLAGS += -std=c++0x -fPIC

# Sources
SOURCES  = $(wildcard *.cc)
INCDIRS  += ./include

# Makefile rules post processing
include $(BUILD)/Makefile.post
