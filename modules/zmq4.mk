#
# $Id: $
#
# Module ZMQ: the ZeorMQ 4.x (http://zeromq.org) messaging library
# Depended module: PTHREAD SODIUM
#
# Copyright (c) 2014-2018 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

ZMQ4_SDK   = $(ROOT)/libs/build/zmq
ZMQ4_ROOT  = $(ROOT)/libs/zeromq-4.2.3
ZMQ4_INCDIRS := $(ZMQ4_ROOT)/include
ZMQ4_LIBDIRS := $(ZMQ4_ROOT)/$(OSTYPE)/lib
ZMQ4_LIBNAME := zmq
ZMQ4_LDPATH  := $(ZMQ4_LIBDIRS)
ZMQ4_DEPENDS := $(ZMQ4_LIBDIRS)/libzmq.a
ZMQ4_DEFINES := USE_ZMQ4

$(ZMQ4_DEPENDS):
	$(call make_module,ZMQ4,$(ZMQ4_SDK))
