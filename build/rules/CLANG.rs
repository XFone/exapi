#
# $Id: $
#
# Ruleset for LLVM/Clang Compiler Infrastructure
#
# Copyright (c) 2013-2018 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

ifeq (, $(CLANG_HOME))
  CLANG_HOME = /usr/bin
  # CLANG_HOME = $(readlink -f `which clang` | sed "s:llc::")
endif

CC       := clang
CXX      := clang++

CLANG_BIN = $(CLANG_HOME)
CLANG     = $(CLANG_BIN)/$(CROSS_COMPILE)clang
CLANGXX   = $(CLANG_BIN)/$(CROSS_COMPILE)clang++

#
# Use module CLANG for additional build settings
#
MODULES += CLANG

#
# Check CLang environments
#
all::
	@set -e
	@echo "---------------------------------------------------------------"
ifeq (, $(CLANG_HOME))
	@echo "Clang environment not set."
	@echo "---------------------------------------------------------------"
	@exit 2
else
	@$(CC) --version | grep version
endif
	@echo "---------------------------------------------------------------"
