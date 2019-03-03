# CentralManager makefile

# make run will ignore all settings
# debug setttings
# 1 on, 0 off

# optional debug settings, this are only avaible in debug mode
HDW_LIVE_DATA ?= 1
HDW_NO_LL ?= 1
PRINT_DATA ?= 0


##############################################################################
# no settings are below this

CC=g++
C_FLAGS=-std=c++14 -Wall -pthread
PI_FLAGS=-lwiringPi

PROGS=CentralManager

SERVERFILES=server.cpp main_class.cpp
LINKFILES=link_logger.cpp $(SERVERFILES) hardware_data_frame.cpp client_command.cpp sequencer_status.cpp
HARDWAREFILES=hardware_controller.cpp i2c_library.cpp pi_uart.cpp $(LINKFILES)
SEQUENCERFILES=sequencer.cpp $(HARDWAREFILES)
ALLFILES= governor.cpp $(SEQUENCERFILES)

########## optional setting(s) ####################
ifeq ($(PRINT_DATA), 1)			#print data
TEST_FLAGS += -DPRINT_DATA_FRAME
endif

ifeq ($(HDW_LIVE_DATA), 0)		#live data
TEST_FLAGS += -DSENSOR_DATA_OFF
endif

ifeq ($(HDW_NO_LL), 1)			#no link on hdw test
TEST_FLAGS += -DLL_OFF
endif

run:
	$(CC) $(C_FLAGS) $(PI_FLAGS) -o $(PROGS) $(ALLFILES)
hardware_test_debug:
	$(CC) $(C_FLAGS) $(PI_FLAGS) -g -o $(PROGS) tests/hardware_test_main.cpp $(HARDWAREFILES) $(TEST_FLAGS)
server_test_debug:
	$(CC) $(C_FLAGS) -g -o $(PROGS) tests/server_test_main.cpp $(SERVERFILES) $(TEST_FLAGS)
clean:
	rm -f *.o hardware/*.o $(PROGS)
