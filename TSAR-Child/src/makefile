CC=g++
C_FLAGS=-std=c++11 -pthread -lncurses
D_FLAGS=-Wall -g

PROGS=TSAR-Child

ALLFILES= main.cpp command_control.cpp central_manager.cpp GPIO/GPIOManager.cpp GPIO/GPIOConst.cpp

run:
	$(CC) $(C_FLAGS) -o $(PROGS) $(ALLFILES)
debug:
	$(CC) $(C_FLAGS) $(D_FLAGS) -o $(PROGS) $(ALLFILES)
clean:
	rm -f *.o $(PROGS)
