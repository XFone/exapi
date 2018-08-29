#
# $Id: $
#
# Module IMPI: Intel(R) MPI Library for Linux* OS
# MPI version 2.2 (MPI-2.2)
# *NOTE: please use mpiicc (CC) and mpiicpc (CXX) instead
#
# Copyright (c) 2014 ZeroneIO. All rights reserved.
#
# $Log: $
#

IMPI_INCDIRS := $(I_MPI_ROOT)/$(ARCH)/include
IMPI_LIBDIRS := $(I_MPI_ROOT)/$(ARCH)/lib
IMPI_LIBNAME := mpi mpigf mpigi 
IMPI_LDFLAGS :=
IMPI_DEFINES := USE_IMPI

$(IMPI_INCDIRS):
	@echo "---------------------------------------------------------------"
	@echo "Intel environment not set, please run 'source mpivars.sh' first"
	@echo "---------------------------------------------------------------"
