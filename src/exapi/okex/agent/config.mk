# For linux
ifeq (linux, $(OSTYPE))
  CXXFLAGS += -Wno-unused-variable
  LD_PREFIX = LD_LIBRARY_PATH=$(LDPATHS)
endif

# For MacOS
ifeq (darwin, $(OSTYPE))
  CXXFLAGS += -Wno-unused-private-field    # not show warning for unused field
  LD_PREFIX = DYLD_LIBRARY_PATH=$(LDPATHS)
endif

# for UNIT_TEST and LOGFILE: dont print filename and lineno
STDCCFLAGS += -DNO_FILE_LINE

# Test Case
TRACE_FILE  ?= $(shell date +"trace-$(TARGET_PRG)-%Y%m%d-%H%M.txt")
CLEAN_FILES += trace-$(TARGET_PRG)-*
KEY_FILE = my.key

test: all
	@echo "🌞  Testing..."
	@echo "Trace and log is saved to $(TRACE_FILE)"
	@$(LD_PREFIX) $(BINDIR)/$(TARGET_PRG) -c my.key -t 7 -v 7 2>&1  | tee -i $(TRACE_FILE)
