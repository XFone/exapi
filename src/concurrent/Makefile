#
# $Id: $
#
# Makefile for concurrent
#
# Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
# Copyright (c) 2004-2006. All rights reserved.
#
# $Log: Makefile,v $
#
#

BUILD = ../../build
SUBDIRS = test
RULESET = CLANG

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

# Build target (TARGET_LIB, TARGET_SHLIB or TARGET_PRG)
TARGET_LIB = libconcurrent

# Depended Modules (libraries)
MODULES += IUTILS TBB EVENT

# CFLAGS, CXXFLAGS and STDCCFLAGS
CXXFLAGS += -Wno-deprecated -std=c++0x -fPIC

# CXXFLAGS_GCC, CXXFLAGS_MSVC, JFLAGS

# LDFLAGS

# Sources to build
SOURCES = $(wildcard *.c* pool/*.c* fifo/*.c* future/*.c* timer/*.c*)

SOURCES_LINUX = $(wildcard osdeps/linux/*.c*)
SOURCES_WIN32 = $(wildcard osdeps/win32/*.c*)

# SOURCES_ANDRIOD
# SOURCES_ARCH_ARM64

# Makefile rules post processing
include $(BUILD)/Makefile.post
