#
# $Id: $
#
# Makefile for thid-party libraries
#
# Copyright (c) 2014-2018. All rights reserved.
#
# $Log: Makefile,v $
#
#

BUILD = ../build
SUBDIRS = build

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

# Depended Modules (libraries)
MODULES += CJSON RESTBED

# All
all::
	@set -e
	@echo "---------------------------------------------------------------"
	@echo "               Building Third-party Libraries                  "
	@echo "---------------------------------------------------------------"
	-@TARGET_ARCH=$(TARGET_ARCH) $(MAKE) --no-print-directory modules

list:
	@echo "List of Third-party Libraries:"
	-@for i in $(LIBNAMES) ; do ( echo "  " $$i ); done

# Makefile rules post processing
# include $(BUILD)/Makefile.post
include $(BUILD)/modules.mk
MDEPENDS += $(foreach module, $(MODULES), $($(module)_DEPENDS))
LIBDIRS  += $(foreach module, $(MODULES), $(abspath $($(module)_LIBDIRS)))
LIBNAMES += $(foreach module, $(MODULES), $($(module)_LIBNAME))

DEPEND_LIBS += $(strip $(MDEPENDS))

modules: $(DEPEND_LIBS)
	@echo "[---]	😎 done."
