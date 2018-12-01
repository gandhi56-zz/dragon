#include "../include/player.h"


int main(int argc, char *argv[]){
    
/*
    // fetch port number
	char serverIp[] = "127.0.0.1";
  	int port;
	bool dispBoard = false;

	// TODO flag settings for bash commands
	for (int i = 0; i < argc; ++i){
		cout << argv[i] << endl;
		if (argv[i] == (char*)"--print-board"){
			dispBoard = true;
		}
		else{
			port = atoi(argv[1]);
		}
	}


	// create player and try connecting to the server    
	Player player(serverIp, port);
	player.run(false);

*/
	Player player;
	player.gameState.set_size(2, 2);
	player.gameState.create_graph();

	cout << player.best_neg_move(player.gameState, 100, true, true) << endl;

	player.gameState.create_graph();
	player.gameState.update("?b1Bb2");
	player.gameState.show();

	return 0;
}
