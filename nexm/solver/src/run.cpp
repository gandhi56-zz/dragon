#include "../include/player.h"

int main(int argc, char *argv[]){
    if (argc != 2){
        cerr << "Usage: <port>" << endl;
        exit(0);
    }
    
    // fetch port number
    //char serverIp[] = "127.0.0.1";
    //int port = atoi(argv[1]);
    
	// create player and try connecting to the server    
    //player.run();
    //player.solve(player.gameState, player.myStone);
	
	uint rows = 3;
	uint cols = 3;
	
	Player player;
	player.gameState.set_size(rows, cols);
	player.gameState.create_graph();
	player.set_state("Bb3Wa1?a2;");
	
	player.gameState.show();
	
	return 0;
}
