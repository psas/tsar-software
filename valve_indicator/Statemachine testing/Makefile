LDFLAGS = -g -Wall -std=c11
HEADERS = StateMachine.h

all: clean testbench.o StateMachine.o
	gcc -o testbench testbench.o StateMachine.o $(LDFLAGS)

testbench.o: testbench.c $(HEADERS)
	gcc -c  testbench.c $(LDFLAGS)

StateMachine.o: StateMachine.c
	gcc -c StateMachine.c $(LDFLAGS)

clean:
	rm -rf testbench *.o

