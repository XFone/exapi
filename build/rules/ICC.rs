#
# $Id: $
#
# Ruleset for Intel ICC/ICPC compiler (from Composer XE 2013 SP1)
#
# Copyright (c) 2013-2014 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

############################## INTEL ##############################
# Compiler settings:
#   CC   = icc
#   CXX  = icpc
# with libraries:
#   mkl  - Intel Math Library
#   mic  - Intel MIC processor Support
#   impi - Intel MPI Library
#   tbb  - Thread Building Blocks

# IDB_HOME is set by /opt/intel/bin/iccvars.sh
ifeq (, $(IDB_HOME))
  # IDB_HOME := $(readlink -f `which icc` | sed "s:icc::")
endif

# 
# Use module ICC for additional build settings
#
MODULES += ICC

#
# C/C++ compiler settings
# ivvcars.sh preset variables:
#   CPATH GDB_CROSS LD_LIBRARY_PATH MIC_LD_LIBRARY_PATH 
#   IPPROOT MKLROOT NLSPATH TBBROOT 
#
CC  := icc
CXX := icpc
ifneq (, $(IDB_HOME))
  ifneq (, $(findstring MIC, $(RULESET)))
    ARCH        := mic
    CFLAGS      := -O3 -mmic
    CXXFLAGS    := -O3 -mmic
    ICC_LIBDIRS := $(subst :, , $(MIC_LIBRARY_PATH))
    ICC_LDFLAGS := -mmic
  else
    ARCH        := $(notdir $(IDB_HOME))
    CFLAGS      := -O3
    CXXFLAGS    := -O3
    ICC_LIBDIRS := $(subst :, , $(LIBRARY_PATH))
    ICC_LDFLAGS :=
  endif
  ICC_INCDIRS := $(subst :, , $(CPATH))
  ICC_LIBNAME :=
endif

#
# Check ICC environments and print version number
#
# DEPENDS += $(DEPDIR)/icc_version.d
# icc_version.cc: 
all::
	@set -e 
	@echo "---------------------------------------------------------------"
ifeq (, $(IDB_HOME))
	@echo "Intel environment not set, please run 'source iccvars.sh' first"
	@echo "---------------------------------------------------------------"
	@exit 2
else
	@$(CC) -V
endif
	@echo "---------------------------------------------------------------"
