#
# $Id: $
#
# Modules and libraries for building projects
#
# Copyright (c) 2010-2013 @Company. All rights reserved.
#
# $Log: $
#
#

# Modules are included in Makefile.post

# Common modules distributed with build 
include $(BUILD)/modules/*.mk

# Project related modules
-include $(BUILD)/../modules/*.mk
#ifneq (, $(strip $(MODULES)))
#  MOD_INCS = $(strip $(foreach mod, $(MODULES), $(BUILD)/../modules/$(mod).mk))
#  -include $(MOD_INCS)
#endif
