
CC = g++
CCFLAGS = -Wall -std=c++17
SRC = ./src/
OBJ = ./obj/
HEADER = ./include/

$(OBJ)main: $(OBJ)main.o $(OBJ)game.o $(OBJ)server.o
	$(CC) $(OBJ)*.o -o $(OBJ)main

$(OBJ)main.o: $(OBJ)server.o $(OBJ)game.o
	$(CC) $(SRC)main.cpp -c -o $(OBJ)main.o $(CCFLAGS)

$(OBJ)game.o: $(SRC)game.cpp
	$(CC) $(SRC)game.cpp -c -o $(OBJ)game.o $(CCFLAGS)

$(OBJ)server.o: $(SRC)server.cpp
	$(CC) $(SRC)server.cpp -c -o $(OBJ)server.o $(CCFLAGS)

run: $(OBJ)main
	$(OBJ)main

clean:
	rm -f $(OBJ)*.o $(OBJ)main
	rm -rf $(SRC).*.*.*
	rm -rf $(HEADER).*.*.*
