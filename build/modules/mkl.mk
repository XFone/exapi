#
# $Id: $
#
# Module MKL: Intel(R) Math Kernel Library 11.x for Linux
#
# Copyright (c) 2014 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

MKL_INCDIRS := $(MKL_ROOT)/include
MKL_LIBDIRS := $(MKL_ROOT)/lib
MKL_LIBNAME := mkl
MKL_LDFLAGS :=
MKL_DEPENDS := $(IMPI_LIBDIRS)/$(ARCH)/libmkl_rt.so
MKL_DEFINES := USE_MKL

$(MKL_DEPENDS):
	@echo "---------------------------------------------------------------"
	@echo "Intel environment not set, please run 'source mklvars.sh' first"
	@echo "---------------------------------------------------------------"
