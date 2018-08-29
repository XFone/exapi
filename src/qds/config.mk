# For linux only
ifeq (linux, $(OSTYPE))
  LIBNAMES += uuid rt
  CXXFLAGS += -Wno-unused-variable
  LD_PREFIX = LD_LIBRARY_PATH=$(LDPATHS)
endif

# For MacOS only
ifeq (darwin, $(OSTYPE))
  CXXFLAGS += -Wno-infinite-recursion      # warning in boost
  CXXFLAGS += -Wno-deprecated-declarations # warning in boost (OSMemoryBarrier)
  CXXFLAGS += -Wno-unused-private-field    # not show warning for unused field
  LD_PREFIX = DYLD_LIBRARY_PATH=$(LDPATHS)
endif

# for UNIT_TEST and LOGFILE: dont print filename and lineno
STDCCFLAGS += -DNO_FILE_LINE

# Test Case
TRACEFILE=$(shell date +"trace-$(TARGET_PRG)-%Y%m%d-%H%M.txt")
QDATAFILE=$(shell date +"qdata-$(TARGET_PRG)-%Y%m%d-%H%M.bin")

ldpath:
	@echo $(LD_PREFIX)

test: all
	@echo ""
	@echo "🌞  Testing..."
	@echo "💡  Trace and log is saved to $(TRACEFILE)"
	@$(LD_PREFIX) $(BINDIR)/$(TARGET_PRG) -t 7 -v 7 -c ./etc/qds.conf -q $(QDATAFILE) 2>&1  | tee -i $(TRACEFILE)

data: all
	@echo ""
	@echo "🌞  Replaying pcap packets in $(INPUT) ..."
	@echo "💡  Trace and log is saved to $(TRACEFILE)"
	@$(LD_PREFIX) $(BINDIR)/$(TARGET_PRG) -t 7 -v 7 -c ./etc/qds.conf -l $(INPUT) 2>&1  | tee -i $(TRACEFILE)

convert: all
	@echo ""
	@echo "🌞  Converting qdata in $(INPUT) to .pcap ..."
	@$(LD_PREFIX) $(BINDIR)/$(TARGET_PRG) -t 7 -v 7 -c ./etc/qds.conf -x $(INPUT)

