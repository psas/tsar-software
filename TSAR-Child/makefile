CC=g++
C_FLAGS=-std=c++11 -pthread -lncurses
D_FLAGS=-Wall -g

PROGS=TSAR-Child

ALLFILES= src/main.cpp src/command_control.cpp src/central_manager.cpp src/GPIO/GPIOManager.cpp src/GPIO/GPIOConst.cpp

run:
	$(CC) $(C_FLAGS) -o $(PROGS) $(ALLFILES)
debug:
	$(CC) $(C_FLAGS) $(D_FLAGS) -o $(PROGS) $(ALLFILES)
clean:
	rm -f *.o $(PROGS)
