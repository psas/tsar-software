CC 	= g++
CFLAGS 	= -std=c++11
LDFLAGS = -lncurses -pthread
DFLAGS 	= -Wall -g3
SRC 	= $(wildcard ./src/*.cpp)
OBJS 	= $(SRC:%.cpp=%.o)
BIN 	= tsar-man

all: build

%.o: %.cpp
	$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

build: $(OBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $(LDFLAGS) $^ -o $(BIN)

run:
	make clean
	make build
	clear
	./$(BIN)

clean:
	rm -rf $(OBJS) $(BIN)
