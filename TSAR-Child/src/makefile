CC=g++
C_FLAGS=-std=c++11 -Wall -pthread

PROGS=TSAR-Child

ALLFILES= main.cpp command_control.cpp central_manager.cpp

run:
	$(CC) $(C_FLAGS) -o $(PROGS) $(ALLFILES)
clean:
	rm -f *.o $(PROGS)
