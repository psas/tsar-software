CC=g++4.1
C_FLAGS=-std=c++11 -pthread -lncurses
D_FLAGS=-Wall -g

PROGS=TSAR-Child

EXCLUDE=src/central_manager.cpp src/command_control.cpp src/GPIO/*.cpp
ALLFILES=$(filter-out $(EXCLUDE), $(wildcard src/*.cpp) $(wildcard src/GPIO/*.cpp))

run:
	$(CC) $(C_FLAGS) -o $(PROGS) $(ALLFILES)
debug:
	$(CC) $(C_FLAGS) $(D_FLAGS) -o $(PROGS) $(ALLFILES)
clean:
	rm -f *.o $(PROGS)
