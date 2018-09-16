#
# $Id: $
#
# Makefile for build C++ restbed library
#
# Copyright (c) 2014-2018 ZeroneIO. All rights reserved.
#
# $Log: Makefile,v $
#

BUILD = ../../../build
SUBDIRS = 

# Makefile rules pre processing
include $(BUILD)/Makefile.pre

# source url
SRC_URL = https://github.com/Corvusoft/restbed/archive/4.6.tar.gz

# C++ REST makefile variables
RESTBED_VER = restbed-4.6
RESTBED_SRC = $(abspath ./$(RESTBED_VER))
RESTBED_DST = $(abspath ../../$(RESTBED_VER))

# CLEAN_FILES += $(RESTBED_SRC)/build

# Depend third-party libraries
# MODULES += BOOST
BOOST_ROOT = $(ROOT)/libs/boost-1.66.0/include

# configure env
CONF_ENV = CC=clang CXX=clang++ CXXFLAGS="-std=c++11 -fPIC -I$(BOOST_ROOT)" BOOST_ROOT=$(BOOST_ROOT)

# restbed dependancy
RESTBED_DEPENDS = $(RESTBED_SRC)/dependency/asio/asio       \
                  $(RESTBED_SRC)/dependency/kashmir/kashmir \
                  $(RESTBED_SRC)/dependency/catch/include

asio-dependency:
	@git clone https://github.com/Corvusoft/asio-dependency

catch-dependency:
	@git clone https://github.com/Corvusoft/catch-dependency

kashmir-dependency:
	@git clone https://github.com/Corvusoft/kashmir-dependency

$(RESTBED_SRC)/dependency/asio/asio: asio-dependency
	@cp -a asio-dependency/asio $(RESTBED_SRC)/dependency/asio
#	cd $(RESTBED_SRC)/dependency/asio/asio ; autogen.sh ; $(CONF_ENV) ./configure ; make -j 4

$(RESTBED_SRC)/dependency/kashmir/kashmir: kashmir-dependency
	@cp -a kashmir-dependency/kashmir $(RESTBED_SRC)/dependency/kashmir

$(RESTBED_SRC)/dependency/catch/include: catch-dependency
	@cp -a catch-dependency/include $(RESTBED_SRC)/dependency/catch

# restbed source
$(RESTBED_SRC):
	@echo "⬇️  Downloading $(SRC_URL)..."
	@wget -c $(SRC_URL) -O $(RESTBED_VER).tgz
	@tar zxf $(RESTBED_VER).tgz
	@cd $(RESTBED_SRC) ; patch -p1 < ../$(RESTBED_VER).patch

$(RESTBED_SRC)/build/Makefile:
	@mkdir -p $(RESTBED_SRC)/build
	@cd $(RESTBED_SRC)/build ; $(CONF_ENV) cmake .. -DBUILD_TESTS=NO

all:: $(RESTBED_SRC) $(RESTBED_DEPENDS) $(RESTBED_SRC)/build/Makefile
	-$(MAKE) -C $(RESTBED_SRC)/build -j 4

clean:
	-@$(MAKE) -C $(RESTBED_SRC)/build clean
	-rm -rf $(RESTBED_SRC)/build

# restbed install
$(RESTBED_DST):
	@mkdir $(RESTBED_DST)

install: $(RESTBED_DST)
	-mkdir -p $(RESTBED_DST)/include/corvusoft/restbed/detail
	-cp -a $(RESTBED_SRC)/source/restbed $(RESTBED_DST)/include
	-cp -a $(RESTBED_SRC)/source/corvusoft/restbed/*.hpp $(RESTBED_DST)/include/corvusoft/restbed/
	-cp -a $(RESTBED_SRC)/source/corvusoft/restbed/detail/*.hpp $(RESTBED_DST)/include/corvusoft/restbed/detail
	-mkdir -p $(RESTBED_DST)/$(OSTYPE)/lib
	-cp -a $(RESTBED_SRC)/build/*.a $(RESTBED_DST)/$(OSTYPE)/lib

# Makefile rules post processing
# include $(BUILD)/Makefile.post
