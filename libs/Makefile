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
L1_MODULES = CJSON RESTBED
MODULES   += $(L1_MODULES)

# All
all::
	@set -e
	@echo "---------------------------------------------------------------"
	@echo "               Building Third-Party Libraries                  "
	@echo "---------------------------------------------------------------"
	-@TARGET_ARCH=$(TARGET_ARCH) $(MAKE) --no-print-directory modules

list:
	@echo "----------------------------------"
	@echo "List of Third-Party Libraries:  "
	-@for i in $(LIBNAMES) ; do ( echo " " $$i ); done
	@echo "----------------------------------"
	@echo "Libraries in Current Directory: "
	-@for i in $(LIBROOTS) ; do ( echo " " $$i ); done

clean:
	-@for i in $(LIBROOTS) ; do ( echo "cleaning $$i" && rm -rf $$i ); done

# Makefile rules post processing
# include $(BUILD)/Makefile.post

include $(BUILD)/modules.mk
MDEPENDS += $(foreach module, $(L1_MODULES), $($(module)_DEPENDS))
LIBROOTS += $(foreach module, $(L1_MODULES), $(abspath $($(module)_ROOT)))
LIBNAMES += $(foreach module, $(MODULES), $($(module)_LIBNAME))

DEPEND_LIBS += $(strip $(MDEPENDS))

modules: $(DEPEND_LIBS)
	@echo "[---]	😎 done."

