#CentralManager makefile


# debug setttings
# 1 on, 0 off

# select which files to debug
DEBUG_ALL ?= 0# this will turn in all others
DEBUG_HARDWARE ?= 0
DEBUG_LINK ?= 0# this will turn on server too
DEBUG_SERVER ?= 1

# optional debug settings, this are only avaible in debug mode
LIVE_DATA ?= 0
PRINT_DATA ?= 0


##############################################################################
# no settings are below this


CC=g++
CPP_FLAGS=-std=c++17 -Wall

PROGS=CentralManager

HARDWAREFILES=hardware_controller.cpp hardware_library.cpp
SERVERFILES=server.cpp
LINKFILES=link_logger.cpp $(SERVERFILES)
SEQUENCERFILES=sequencer.cpp
CMFILES=central_manager.cpp
MAIN=main.cpp
ALLFILES= $(MAIN) $(CMFILES) $(SEQUENCERFILES) $(LINKFILES) $(HARDWAREFILES)


# debug modes
ifeq ($(DEBUG_ALL), 1)			#debug system
DEBUGFILES= $(CMFILES) $(SEQUENCERFILES) $(LINKFILES) $(HARDWAREFILES)
DEBUG_FLAGS += -DALL_TEST -DLINK_ON
CPP_FLAGS += -pthread -lwiringPi

else ifeq ($(DEBUG_HARDWARE), 1)
ifeq ($(DEBUG_LINK), 1)			#debug hardware and link
DEBUGFILES= $(HARDWAREFILES) $(LINKFILES)
DEBUG_FLAGS += -DHARDWARE_LINK_TEST -DLINK_ON
CPP_FLAGS += -pthread -lwiringPi
else
DEBUGFILES= $(HARDWAREFILES)		#debug only hardware
DEBUG_FLAGS += -DHARDWARE_TEST
CPP_FLAGS += -lwiringPi
endif

else ifeq ($(DEBUG_LINK), 1)		#debug only link
DEBUGFILES= $(LINKFILES)
DEBUG_FLAGS += -DLINK_TEST

else ifeq ($(DEBUG_SERVER), 1)		#debug only server
DEBUGFILES += $(SERVERFILES)
DEBUG_FLAGS += -DSERVER_TEST
endif


#optional setting
ifeq ($(LIVE_DATA), 1)			#turn live data (data from sensor NOT rand)
DEBUG_FLAGS += -DLIVE_DATA
endif
ifeq ($(PRINT_DATA), 1)			#print data
DEBUG_FLAGS+= -DPRINT_DATA_FRAME
endif


run:
	$(CC) $(CPP_FLAGS) -o $(PROGS) $(ALLFILES) -DLINK_ON -pthread -lwiringPi
debug:
	$(CC) $(CPP_FLAGS) -g -o $(PROGS) $(MAIN) $(DEBUGFILES) $(DEBUG_FLAGS)
clean:
	rm -f *.o $(PROGS)
