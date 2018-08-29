#
# $Id: Makefile,v 1.1.1.1 2005/02/02 21:25:40 xiaofeng Exp $
#
# Makefile for utilities
#
# Copyright (c) 2004-2006. All rights reserved.
# 
# $Log: Makefile,v $
#
#

BUILD = ../../../build

SUBDIRS = 

include $(BUILD)/Makefile.pre

# Build target
TARGET_PRG = utils-test

# Modules
MODULES += IUTILS

# Sources to build
# STDCCFLAGS += 
SOURCES = $(wildcard *.c)

include $(BUILD)/Makefile.post

# Test cases
test: all
	$(BINDIR)/$(TARGET_PRG)
