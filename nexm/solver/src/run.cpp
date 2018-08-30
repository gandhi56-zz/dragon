#include "../include/player.h"

int main(int argc, char *argv[]){
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
    player.solve(player.gameState, player.myStone);


/*
	uint rows = 3;
	uint cols = 3;

	while (true){
		Player player;
		player.gameState.set_size(rows, cols);
		player.gameState.create_graph();
		string init;
		cin >> init;
		player.set_state(init);

		//player.solve(player.gameState, BLACK);
		
		player.gameState.show();
		cout << player.gameState.connected("B0", "B1") << endl;
	
	}

*/
	return 0;
}
