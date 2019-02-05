# CentralManager makefile

# make run will ignore all settings
# debug/test setttings
# 1 on, 0 off

# select which files to debug/test
TEST_SEQUENCER ?= 0		# this will turn in all others
TEST_HARDWARE ?= 1
TEST_LINK ?= 0		# this will turn on server too
TEST_SERVER ?= 0

# optional debug/test settings, this are only avaible in debug/test mode
PRINT_DATA ?= 0
HDW_LIVE_DATA ?= 1


##############################################################################
# no settings are below this


CC=g++
C_FLAGS=-std=c++14 -Wall
D_FLAGS=-pthread -lwiringPi

PROGS=CentralManager

HARDWAREFILES=hardware_controller.cpp hardware_library.cpp
SERVERFILES=server.cpp
LINKFILES=link_logger.cpp $(SERVERFILES)
SEQUENCERFILES=sequencer.cpp
MAIN=main.cpp
ALLFILES= $(MAIN) $(SEQUENCERFILES) $(LINKFILES) $(HARDWAREFILES)


# debug modes
ifeq ($(TEST_SEQUENCER), 1)					#debug/test system
TESTFILES= tests/sequencer_test_mains.cpp $(SEQUENCERFILES) $(LINKFILES) $(HARDWAREFILES)
TEST_FLAGS += -DSYSTEM_TEST
CPP_FLAGS += -pthread -lwiringPi

else ifeq ($(TEST_HARDWARE), 1)
ifeq ($(TEST_LINK), 1)						#debug/test hardware and link
TESTFILES= tests/hardware_test_mains.cpp $(HARDWAREFILES) $(LINKFILES)
TEST_FLAGS += -DHARDWARE_LINK_TEST
CPP_FLAGS += -pthread -lwiringPi
else								#debug/test only hardware
TESTFILES= tests/hardware_test_mains.cpp $(HARDWAREFILES)
TEST_FLAGS += -DHARDWARE_TEST -DLINK_OFF
CPP_FLAGS += -lwiringPi
endif

else ifeq ($(TEST_LINK), 1)					#debug/test only link
TESTFILES= tests/link_test_mains.cpp $(LINKFILES)
TEST_FLAGS += -DLINK_TEST

else ifeq ($(TEST_SERVER), 1)					#debug/test only server
TESTFILES += tests/link_test_mains.cpp $(SERVERFILES)
TEST_FLAGS += -DSERVER_TEST
endif


# optional setting(s)
ifeq ($(PRINT_DATA), 1)						#print data
TEST_FLAGS+= -DPRINT_DATA_FRAME
endif

ifeq ($(HDW_LIVE_DATA), 0)					#use live data or read from file
TEST_FLAGS+= -DLIVE_DATA_OFF
endif


run:
	$(CC) $(C_FLAGS) $(D_FLAGS) -o $(PROGS) $(ALLFILES)
debug_test:
	$(CC) $(C_FLAGS) $(TEST_FLAGS) -g -o $(PROGS) $(TESTFILES)
clean:
	rm -f *.o $(PROGS)
