#
# $Id: $
#
# Module PCAP: libpcap library
#
# Copyright (c) 2014-2018 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

PCAP_SOURCE = $(ROOT)/libs/build/libpcap
PCAP_LIBNAME := pcap

# check 'brew install libpcap'
ifeq (darwin, $(OSTYPE))
  BREW_INSTALL_PATH = $(wildcard /usr/local/opt/libpcap)
  ifneq (, $(BREW_INSTALL_PATH))
    PCAP_ROOT    := $(BREW_INSTALL_PATH)
    PCAP_INCDIRS := $(PCAP_ROOT)/include
    PCAP_LIBDIRS := $(PCAP_ROOT)/lib
  endif
endif

# check 'yum install libpcap-devel' or 'apt install libpcap-dev'
ifeq (linux, $(OSTYPE))
  PKG_INSTALL_PATH = $(wildcard /usr/include/pcap)
  ifneq (, $(PKG_INSTALL_PATH))
    PCAP_ROOT    := /usr
   #PCAP_INCDIRS := $(PCAP_ROOT)/include
   #PCAP_LIBDIRS := $(PCAP_ROOT)/lib
  endif
endif

# Use libs built from source (windows)
ifeq (, $(PCAP_ROOT))
  PCAP_ROOT   = $(ROOT)/libs/libpcap-1.9.0
  PCAP_INCDIRS := $(PCAP_ROOT)/include
  PCAP_LIBDIRS := $(PCAP_ROOT)/$(OSTYPE)/lib
  PCAP_DEPENDS := $(PCAP_LIBDIRS)/libpcap.a
endif

$(PCAP_DEPENDS):
	$(call make_module,PCAP,$(PCAP_INCDIRS))

$(PCAP_PARSER):
	$(call make_module,PCAP_PARSER,$(PCAP_PARDIR))
