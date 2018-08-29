#
# $Id: $
#
# Module OPENMP: provides OpenMP support in gcc/icc
#
# Copyright (c) 2013-2014 ZeroneIO. All rights reserved.
#
# $Log: $
#
#

OPENMP_LIBNAME :=
OPENMP_DEFINES := USE_OPENMP

ifeq (icc, $(CC))
  OPENMP_CFLAGS  := -openmp
  OPENMP_LDFLAGS := -openmp
else
  # gcc
  OPENMP_CFLAGS  := -fopenmp
  OPENMP_LDFLAGS := -fopenmp
endif

ifeq (icpc, $(CXX))
  OPENMP_CXXFLAGS:= -openmp
  OPENMP_LDFLAGS := -openmp
else
  # g++
  OPENMP_CXXFLAGS:= -fopenmp
  OPENMP_LDFLAGS := -fopenmp
endif
