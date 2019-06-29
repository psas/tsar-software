CC=g++-4.9
C_FLAGS=-std=c++11 -pthread
D_FLAGS=-Wall -g -li2c

PROGS=TSAR-Child

ALLFILES= main.cpp

run:
	$(CC) $(C_FLAGS) -o $(PROGS) $(ALLFILES)

e: execute
execute: run
	./$(PROGS)

debug:
	$(CC) $(C_FLAGS) $(D_FLAGS) -o $(PROGS) $(ALLFILES)

clean:
	rm -f *.csv *.o $(PROGS)
