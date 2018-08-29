#
# $Id: $
#
# Build rules for $OSTYPE = cygwin
#
# Copyright (c) 2010-2013 @Company. All rights reserved.
#
# $Log: $
#
#

############################ gnu-tools ##################################

ifeq ($(OSTYPE),winnt)
    YACC       := gnu-bison
    LEX        := gnu-flex
    FIND       := gnu-find
    REMOVE     := gnu-rm -vf
endif
