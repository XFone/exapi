#
# $Id: $
#
# Module RESTBED: Corvusoft's Restbed framework.
# Brings asynchronous RESTful functionality to C++11 applications. 
# http://www.corvusoft.co.uk
#
# GNU Affero General Public License (AGPL), or the
# Corvusoft Permissive License (CPL)
#
# $Log: $
#

RESTBED_SDK  = $(ROOT)/libs/build/restbed
RESTBED_DEFINES := USE_RESTBED
RESTBED_LIBNAME := restbed

# Use libs built from source
ifeq (, $(RESTBED_ROOT))
  RESTBED_ROOT 	  := $(ROOT)/libs/restbed-4.6
  RESTBED_INCDIRS := $(RESTBED_ROOT)/include
  RESTBED_LIBDIRS := $(RESTBED_ROOT)/$(OSTYPE)/lib
  RESTBED_LDPATH  := $(RESTBED_LIBDIRS)
  RESTBED_DEPENDS := $(RESTBED_LIBDIRS)/librestbed.a
endif

$(RESTBED_DEPENDS):
	$(call make_module,RESTBED,$(RESTBED_SDK),$(RESTBED_ROOT))

