CC=g++-4.9
C_FLAGS=-std=c++11 -pthread #-lncurses
N_FLAGS=-lncurses
D_FLAGS=-Wall -g3
PROGS=TSAR-Child

EXCLUDE=src/central_manager.cpp src/command_control.cpp src/GPIO src/GPIO/*.cpp
#ALLFILES=$(filter-out $(EXCLUDE), $(wildcard src/*.cpp) $(wildcard src/GPIO/*.cpp))
ALLFILES=src/main.cpp src/mcp342x.cpp src/tsar_lowrate_daq.cpp

run:
	$(CC) $(C_FLAGS) $(ALLFILES) $(N_FLAGS) -o $(PROGS)
debug:
	$(CC) $(C_FLAGS) $(ALLFILES) $(D_FLAGS) $(N_FLAGS) -o $(PROGS)
clean:
	rm -f *.o $(PROGS)
