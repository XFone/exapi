#
# $Id: $
#
# Module UUID: libuuid library
#
# Copyright (c) 2014-2018 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

ifeq (linux, $(OSTYPE))
  UUID_LIBNAME := uuid
  UUID_DEPENDS := /usr/lib64/libuuid.so 
endif
