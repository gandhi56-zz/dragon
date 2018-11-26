//#include "../include/player.h"
#include "../include/game.h"
#include <algorithm>	// for random_shuffle

int main(int argc, char *argv[]){
/*
	if (argc != 2){
        cerr << "Usage: <port>" << endl;
        exit(0);
    }
    
    // fetch port number
	char serverIp[] = "127.0.0.1";
  	int port = atoi(argv[1]);

	// create player and try connecting to the server    
	Player player(serverIp, port);
	player.run();
*/


	State state;
	state.set_size(3, 3);
	state.create_graph();
	state.update("Bc3?c2");
	state.show();

	state.update("Wa1?a2");
	state.show();
	state.update("Bc2Ba2?c3");
	state.show();
	state.revert("Bc2Ba2?c3", 'B');
	state.show();

	return 0;
}
