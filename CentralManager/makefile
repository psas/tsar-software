#CentralManager makefile


# debug setttings
# 1 on, 0 off

# select which files to debug
DEBUG_ALL ?= 1# this will turn in all others
DEBUG_HARDWARE ?= 1
DEBUG_LINK ?= 1# this will turn on server too
DEBUG_SERVER ?= 1

# optional debug settings, this are only avaible in debug mode
LIVE_DATA ?= 0
PRINT_DATA ?= 0


##############################################################################
# no settings are below this


CC=g++
CPP_FLAGS=-std=c++14 -pthread -Wall -lwiringPi

PROGS=CentralManager
HARDWAREFILES=hardware_controller.cpp hardware_library.cpp
SERVERFILES=server.cpp string_queue.cpp
LINKFILES=link_logger.cpp send_data_queue.cpp $(SERVERFILES)
SEQUENCERFILES=sequencer.cpp
CMFILES=central_manager.cpp
MAIN=main.cpp
ALLFILES= $(MAIN) $(CMFILES) $(SEQUENCERFILES) $(LINKFILES) $(HARDWAREFILES)


# debug modes
ifeq ($(DEBUG_ALL), 1)
DEBUGFILES= $(CMFILES) $(SEQUENCERFILES) $(LINKFILES) $(HARDWAREFILES)
DEBUG_FLAGS += -DALL_TEST -DLINK_ON

else ifeq ($(DEBUG_HARDWARE), 1)
ifeq ($(DEBUG_LINK), 1)
DEBUGFILES= $(HARDWAREFILES) $(LINKFILES)
DEBUG_FLAGS += -DHARDWARE_LINK_TEST -DLINK_ON
else
DEBUGFILES= $(HARDWAREFILES)
DEBUG_FLAGS += -DHARDWARE_TEST
endif

else ifeq ($(DEBUG_LINK), 1)
DEBUGFILES= $(LINKFILES)
DEBUG_FLAGS += -DLINK_TEST

else ifeq ($(DEBUG_SERVER), 1)
DEBUGFILES += $(SERVERFILES)
DEBUG_FLAGS += -DSERVER_TEST
endif


#optional setting
ifeq ($(LIVE_DATA), 1)
DEBUG_FLAGS += -DLIVE_DATA
endif
ifeq ($(PRINT_DATA), 1)
DEBUG_FLAGS+= -DPRINT_DATA_FRAME
endif


run:
	$(CC) $(CPP_FLAGS) -o $(PROGS) $(ALLFILES) -DLINK_ON
debug:
	$(CC) $(CPP_FLAGS) -g -o $(PROGS) $(MAIN) $(DEBUGFILES) $(DEBUG_FLAGS)
clean:
	rm -f *.o $(PROGS)
