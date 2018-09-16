#
# $Id: Makefile,v 1.5 2005/03/15 03:01:39 xiaofeng Exp $
#
# Makefile for Json2C encoder/decoder
#
# Copyright (c) 2004-2006 $Company. All rights reserved.
#
# $Log: Makefile,v $
#

BUILD = ../../../../build

# SUBDIRS = test

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

SWAGGER_JSON = okex.json

# Depended Modules (libraries)
MODULES += IUTIL ZPROTO

# CXXFLAGS += -Wno-deprecated
INCDIRS += ../include

# Add rules into 'clean' for removing all auto-generated files
CLEAN_FILES += *.i

# Zproto pattern file
CODER_PARSER := $(ROOT)/src/xjson/parser/$(BINDIR)/parser

# Rules for zproto parser tool
%.i: %_package.h
	@echo "---------------------------------------------"
	@printf "Parsing coder definition %b...\n" $(MK_COLOR)$<$(NO_COLOR)
	@echo "---------------------------------------------"
	@$(CC) $(CFLAGS) $(STDCCFLAGS) $(PREP_FLAGS) -D_PATTERN_TOOL_ $< > $@

%.json: %.i $(CODER_PARSER)
	@echo "Generating $<..."
	cat $< | $(CODER_PARSER) -j $@.tmp > $@

# All
all:: $(SWAGGER_JSON)
	@set -e
	@echo "---------------------------------------------------------------"
	@echo "               Production Release ($(COMPILE_TYPE))."
	@echo "---------------------------------------------------------------"

# Makefile rules post processing
include $(BUILD)/Makefile.post

distclean: clean cleanall
	@rm -rf $(SWAGGER_JSON) json_decoder.*
