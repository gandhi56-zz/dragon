
CC = g++
CCFLAGS = -Wall -std=c++17
SRC = ./src/
OBJ = ./obj/
HEADER = ./include/

$(OBJ)main: $(OBJ)hex.o $(OBJ)server.o $(OBJ)main.o 
	$(CC) $(CCFLAGS) $^ -o $@

$(OBJ)main.o: $(SRC)main.cpp
	$(CC) $(CCFLAGS) $^ -c -o $@

$(OBJ)server.o: $(SRC)server.cpp
	$(CC) $(CCFLAGS) $^ -c -o $@

$(OBJ)hex.o: $(SRC)hex.cpp
	mkdir -p $(@D)
	$(CC) $(CCFLAGS) $^ -c -o $@

run: $(OBJ)main
	@$(OBJ)main

clean:
	@rm -f $(OBJ)*.o $(OBJ)main
	@rm -rf $(SRC).*.*.*
	@rm -rf $(HEADER).*.*.*
	@rm -rf .*.*.*

