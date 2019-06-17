#include "../include/player.h"

int main(int argc, char *argv[]){

	
	// fetch port number
	// char serverIp[] = "127.0.0.1";
  	// int port = 21299;
	//bool dispBoard = false;

	// TODO flag settings for bash commands
	// for (int i = 0; i < argc; ++i){
	// 	cout << argv[i] << endl;
	// 	if (argv[i] == (char*)"--print-board"){
	// //		dispBoard = true;
	// 	}
	// 	else{
	// 		port = atoi(argv[1]);
	// 	}
	// }


	// create player and try connecting to the server    
	// Player player(serverIp, port);
	// player.run(false);

	Player player;
	player.board.set_size(3, 3);
	player.board.create_graph();
	player.board.update("Ba1?b2");
	player.board.show();
	return 0;
}
