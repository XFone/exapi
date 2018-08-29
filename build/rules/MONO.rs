#
# $Id: $
#
# Ruleset for Mono (.NET) framework
#
# Copyright (c) 2013-2016 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

############################## MONO ##############################

#MONO_HOME should be set in somewhere
ifeq (, $(MONO_HOME))
  # MONO_HOME = /opt/mono/bin
  MONO_HOME = /usr/bin
endif

MONO = $(MONO_HOME)/mono

#
# Use module MONO for additional build settings
#
MODULES += MONO

# Additional clean files
CLEAN_FILES += bin/Debug bin/Release obj/Debug obj/Release

#
# define rule for .cs(C# suffix) files
#
define print_cs_rules
  $(shell printf "%s: %s\n\t\044(call make_cs_depend)\n\n"     \
	"$(DEPDIR)/$(notdir $(basename $(1))).d" "$(1)"        \
	> $(DEPDIR)/$(notdir $(basename $(1))).d.rule)
endef

#
# build executable (should moved into Makefile.post)
#
#$(TARGETS): $(CSPROJ)
#	@printf '%b     %b\n' $(LD_COLOR)[DMCS] $(BIN_COLOR)$@$(NO_COLOR)
#	@xbuild $(CSPROJ)

#
# Check MONO environments
#
all::
	@set -e
	@echo "---------------------------------------------------------------"
ifeq (, $(MONO_HOME))
	@echo "MONO environment not set."
	@echo "---------------------------------------------------------------"
	@exit 2
else
	@$(MONO) --version | grep version
endif
	@echo "---------------------------------------------------------------"
