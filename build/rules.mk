#
# $Id: $
#
# All make rules and OS related macros.
#
# Copyright (c) 2010-2013 @Company. All rights reserved.
#
# $Log: $
#
#

#
# Added tools path
#
ROOT = $(abspath $(BUILD)/..)
PATH := $(BUILD)/tools:$(PATH)

#
# Build-rules depends on OS and compiler.
#
include $(BUILD)/rules/*.mk

#
# Ruleset related to SDK, platform DK, or development studio
# $RULESET MUST be pre-set before include Makefile.pre
#
ifneq (, $(strip $(RULESET)))
  RULESET_INCS = $(strip $(foreach rs, $(RULESET), $(BUILD)/rules/$(rs).rs))
  -include $(RULESET_INCS)
endif

######################################################################

#
# Optimize for specific processor
#
ifeq ($(WITH_PROCESSOR_OPTS),1)
  CXXFLAGS += -march=$(MACHTYPE)
  CFLAGS   += -march=$(MACHTYPE)
  LDFLAGS  +=
  LDLIBS   +=
endif

#
# Set up compiler flags and macros for debug/release versions ###
#

# override WITH_DEBUG for specific targets
opt     :: WITH_DEBUG = 0
debug	:: WITH_DEBUG = 1

ifeq (,  $(WITH_DEBUG))
  WITH_DEBUG := 0
endif

ifeq (1, $(WITH_DEBUG))
  COMPILE_TYPE := debug
  CFLAGS       += -g -DDEBUG=1
  CXXFLAGS     += -g -DDEBUG=1
else
  COMPILE_TYPE := opt
  CFLAGS       += -DNDEBUG -DNO_TRACE
  CXXFLAGS     += -DNDEBUG -DNO_TRACE
endif

OS_ARCH	    := $(OSTYPE).$(ARCH)
TARGET_NAME := $(COMPILE_TYPE).$(OS_ARCH)

ifndef OUTPUT_FLAG
  OUTPUT_FLAG = -o
endif

################## Compilation WARNING/Error Settings ####################
#
# For gcc/g++ only: strict warning settings
#
ifeq (0, $(WITH_DEBUG))
  ifeq (gcc, $(CC))
    CFLAGS   += -O2 -Wall -Wuninitialized
  endif
  ifeq (g++, $(CXX))
    CXXFLAGS += -O2 -Wall
  endif
else
  ifeq (gcc, $(CC))
    CFLAGS   += -Wall -Wunused -Wimplicit                                \
                -Wreturn-type -Wswitch -Wcomment -Wtrigraphs -Wformat    \
                -Wchar-subscripts -Wparentheses -Wshadow                 \
                -Wpointer-arith -Wcast-align -Wwrite-strings             \
                -Waggregate-return -Wstrict-prototypes                   \
                -Wnested-externs -Winline

  endif
  ifeq (g++, $(CXX))
    CXXFLAGS += -Wall
  endif
  ifeq (aCC, $(CC))
    CFLAGS   += +W281
  endif
endif

################## General compilation options #####################

# LDFLAGS  +=
# CXXFLAGS +=
# CFLAGS   +=
# LDLIBS   +=
INCDIRS  += $(BUILD)

################## Target system specific rules ####################
ifneq (, $(TARGET_ARCH))
  MODULES += $(TARGET_ARCH)
endif

