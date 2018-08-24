
CC = g++
CCFLAGS = -Wall -std=c++17
SRC = ./
OBJ = ./
PORT = 16011

$(OBJ)player.o: $(SRC)player.cpp
	$(CC) $(SRC)player.cpp -c -o $(OBJ)player.o $(CCFLAGS)

$(OBJ)player : $(OBJ)player.o
	$(CC) $(OBJ)player.o -o $(OBJ)player

run: $(OBJ)player
	$(OBJ)player $(PORT)

clean:
	rm -f player player.o
