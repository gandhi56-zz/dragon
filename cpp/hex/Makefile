
CC = g++
CCFLAGS = -Wall -std=c++17
SRC = ./src/
OBJ = ./obj/
HEADER = ./include/
GAME = hex

$(OBJ)$(GAME): $(OBJ)$(GAME).o $(OBJ)server.o $(OBJ)main.o 
	$(CC) $(CCFLAGS) $^ -o $@

$(OBJ)main.o: $(SRC)main.cpp
	$(CC) $(CCFLAGS) $^ -c -o $@

$(OBJ)server.o: $(SRC)server.cpp
	$(CC) $(CCFLAGS) $^ -c -o $@

$(OBJ)$(GAME).o: $(SRC)$(GAME).cpp
	mkdir -p $(@D)
	$(CC) $(CCFLAGS) $^ -c -o $@

run: $(OBJ)$(GAME)
	@$(OBJ)$(GAME)

clean:
	@rm -f $(OBJ)*.o $(OBJ)$(GAME)
	@rm -rf $(SRC).*.*.*
	@rm -rf $(HEADER).*.*.*
	@rm -rf .*.*.*

