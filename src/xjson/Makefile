#
# $Id: $
#
# Makefile for xJSON: a C++ extension of cJSON and tools
#
# Copyright (c) 2014-2018 ZeroneIO. All rights reserved.
#
# $Log: Makefile,v $
#
#

BUILD = ../../build
SUBDIRS = parser json2c swagger2c 
RULESET = CLANG

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

TARGET_LIB = libxjson

# Modules
MODULES += IUTILS IDATA CJSON

# CFLAGS, CXXFLAGS and STDCCFLAGS
CXXFLAGS += -fPIC                  # for linking to shared library
CXXFLAGS += -std=c++0x             # for <atomic> and lambda

SOURCES += $(wildcard *.c *.cc)

# Makefile rules post processing
include $(BUILD)/Makefile.post

