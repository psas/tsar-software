CC=g++
CFLAGS=-std=c++11
LDFLAGS=-lncurses -pthread
DFLAGS=-Wall -g3


TSAR_SRC = ./src
HARDWARE_DRIVERS_SRC = ./src/hardware_drivers
GPIO_SRC = ./src/BeagleBoneBlack-GPIO/GPIO
RAPIDJSON_SRC = ./src/rapidjson/include/rapidjson


INCLUDE_DIRS =	-I$(TSAR_SRC)                   \
		-I$(HARDWARE_DRIVERS_SRC)       \
		-I$(GPIO_SRC)                   \
		-I$(RAPIDJSON_SRC)


SOURCES =	$(GPIO_SRC)/GPIOConst.cpp 	\
		$(GPIO_SRC)/GPIOManager.cpp 	\
		$(TSAR_SRC)/command_control.cpp	\
		$(TSAR_SRC)/central_manager.cpp \
		$(TSAR_SRC)/main.cpp


OBJS =		$(SOURCES:%.cpp=%.o)


%.o: %.cpp
	$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDE_DIRS) -c $< -o $@


central_manager: $(OBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $(LDFLAGS) $^ -o $@ 

clean:
	rm -f $(OBJS) central_manager

