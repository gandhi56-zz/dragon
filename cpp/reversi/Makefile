reversi_main: reversi_main.o game.o player.o
	g++ reversi_main.o game.o player.o -o reversi_main

reversi_main.o: game.o
	g++ reversi_main.cpp -c -o reversi_main.o

game.o: game.cpp
	g++ game.cpp -c -o game.o

player.o: player.cpp
	g++ player.cpp -c -o player.o

run: reversi_main
	./reversi_main

clean:
	rm -f *.o
	rm -f reversi_main
