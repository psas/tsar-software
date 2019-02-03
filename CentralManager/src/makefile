#CentralManager makefile


# debug setttings
# 1 on, 0 off

# select which files to debug
TEST_ALL ?= 0# this will turn in all others
TEST_HARDWARE ?= 0
TEST_LINK ?= 0# this will turn on server too
TEST_SERVER ?= 1

# optional debug settings, this are only avaible in debug mode
PRINT_DATA ?= 0


##############################################################################
# no settings are below this


CC=g++
CPP_FLAGS=-std=c++1z -Wall

PROGS=CentralManager

HARDWAREFILES=hardware_controller.cpp hardware_library.cpp
SERVERFILES=server.cpp
LINKFILES=link_logger.cpp $(SERVERFILES)
SEQUENCERFILES=sequencer.cpp
CMFILES=central_manager.cpp
MAIN=main.cpp
ALLFILES= $(MAIN) $(CMFILES) $(SEQUENCERFILES) $(LINKFILES) $(HARDWAREFILES)


# debug modes
ifeq ($(TEST_ALL), 1)			#debug system
TESTFILES= $(CMFILES) $(SEQUENCERFILES) $(LINKFILES) $(HARDWAREFILES)
TEST_FLAGS += -DALL_TEST
CPP_FLAGS += -pthread -lwiringPi

else ifeq ($(TEST_HARDWARE), 1)
ifeq ($(TEST_LINK), 1)			#debug hardware and link
TESTFILES= $(HARDWAREFILES) $(LINKFILES)
TEST_FLAGS += -DHARDWARE_LINK_TEST
CPP_FLAGS += -pthread -lwiringPi
else
TESTFILES= $(HARDWAREFILES)		#debug only hardware
TEST_FLAGS += -DHARDWARE_TEST -DLINK_OFF
CPP_FLAGS += -lwiringPi
endif

else ifeq ($(TEST_LINK), 1)		#debug only link
TESTFILES= $(LINKFILES)
TEST_FLAGS += -DLINK_TEST

else ifeq ($(TEST_SERVER), 1)		#debug only server
TESTFILES += $(SERVERFILES)
TEST_FLAGS += -DSERVER_TEST
endif


#optional setting(s)
ifeq ($(PRINT_DATA), 1)			#print data
TEST_FLAGS+= -DPRINT_DATA_FRAME
endif


run:
	$(CC) $(CPP_FLAGS) -o $(PROGS) $(ALLFILES) -DLINK_ON -pthread -lwiringPi
test:
	$(CC) $(CPP_FLAGS) -g -o $(PROGS) $(MAIN) $(TESTFILES) $(TEST_FLAGS)
clean:
	rm -f *.o $(PROGS)
