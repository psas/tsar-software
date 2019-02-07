# CentralManager makefile

# make run will ignore all settings
# debug setttings
# 1 on, 0 off

# optional debug settings, this are only avaible in debug mode
HDW_LIVE_DATA ?= 1
PRINT_DATA ?= 0


##############################################################################
# no settings are below this

CC=g++
C_FLAGS=-std=c++14 -Wall -pthread
PI_PLAG=-lwiringPi

PROGS=CentralManager

SERVERFILES=server.cpp
LINKFILES=link_logger.cpp $(SERVERFILES) sensor_data_frame.cpp client_command.cpp sequence_status.cpp
HARDWAREFILES=hardware_controller.cpp hardware_library.cpp $(LINKFILES)
SEQUENCERFILES=sequencer.cpp $(HARDWAREFILES)
ALLFILES= governor.cpp $(SEQUENCERFILES)

# optional setting(s)
ifeq ($(PRINT_DATA), 1)			#print data
TEST_FLAGS+= -DPRINT_DATA_FRAME
endif

ifeq ($(HDW_LIVE_DATA), 0)		#live data
TEST_FLAGS+= -DSENSOR_DATA_OFF
endif

run:
	$(CC) $(C_FLAGS) $(PI_FLAGS) -o $(PROGS) $(ALLFILES)
sequencer_test_debug:
	$(CC) $(C_FLAGS) $(PI_FLAGS) -g -o $(PROGS) tests/sequencer_test_mains.cpp $(SEQUENCERFILES) $(TEST_FLAGS) -DSEQUENCER_TEST
hardware_test_debug:
	$(CC) $(C_FLAGS) $(PI_FLAGS) -g -o $(PROGS) tests/hardware_test_mains.cpp $(HARDWAREFILES) $(TEST_FLAGS) -DHARDWARE_TEST
hardware_link_test_debug:
	$(CC) $(C_FLAGS) $(PI_FLAGS) -g -o $(PROGS) tests/hardware_test_mains.cpp $(HARDWAREFILES) $(TEST_FLAGS) -DHARDWARE_LINK_TEST
link_test_debug:
	$(CC) $(C_FLAGS) -g -o $(PROGS) tests/link_test_mains.cpp $(LINKFILES) $(TEST_FLAGS) -DLINK_TEST
server_test_debug:
	$(CC) $(C_FLAGS) -g -o $(PROGS) tests/link_test_mains.cpp $(SERVERFILES) $(TEST_FLAGS) -DSERVER_TEST
clean:
	rm -f *.o $(PROGS)
