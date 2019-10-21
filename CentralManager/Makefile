CC=g++
CFLAGS=-std=c++11
LDFLAGS=-lncurses -pthread
DFLAGS=-Wall -g3


TSAR_SRC = ./src
GPIO_SRC = ./src/GPIO


INCLUDE_DIRS =	-I$(TSAR_SRC)	\
		-I$(GPIO_SRC)


SOURCES =	$(GPIO_SRC)/GPIOConst.cpp 	\
		$(GPIO_SRC)/GPIOManager.cpp 	\
		$(TSAR_SRC)/command_control.cpp	\
		$(TSAR_SRC)/central_manager.cpp \
		$(TSAR_SRC)/main.cpp


OBJS =		$(SOURCES:%.cpp=%.o)


%.o: %.cpp
	$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@


tsar-child: $(OBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $(LDFLAGS) $^ -o $@ 

clean:
	rm -f $(OBJS) tsar-child


