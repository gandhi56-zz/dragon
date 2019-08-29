#include "../include/player.h"
#include <set>

int main(int argc, char *argv[]){


	// fetch port number
	// char serverIp[] = "127.0.0.1";
  	// int port;
	// //bool dispBoard = false;
	//
	// // TODO flag settings for bash commands
	// for (int i = 0; i < argc; ++i){
	// 	cout << argv[i] << endl;
	// 	if (argv[i] == (char*)"--print-board"){
	// //		dispBoard = true;
	// 	}
	// 	else{
	// 		port = atoi(argv[1]);
	// 	}
	// }
	//
	//
	// // create player and try connecting to the server
	// Player player(serverIp, port);
	// player.run(false);

	Player player;
	player.gameState.set_size(3, 3);
	player.gameState.create_graph();

	set<string> winOpenings;

	// for (int i =0; i < 100; ++i){
	// 	// string move = player.best_nesg_move(player.gameState, 100, true, false);
	// 	winOpenings.insert(move);
	// 	cout << "winning opening: " << move << " winning openings found so far : " << winOpenings.size() << endl;
	// }
	return 0;
}
