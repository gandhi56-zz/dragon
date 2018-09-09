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
//	player.solve(player.gameState, player.myStone);
	
/*
	Player player;
	player.gameState.set_size(3, 3);
	player.gameState.create_graph();


	vector<string> moves = player.get_moves(player.gameState, BLACK);

	moves[0] = "?a3Bc3";

	int bestValue = -100;
	string bestMove;
	int alpha = -100;
	int beta = 100;
	for (int i = 0; i < moves.size(); ++i){

		cout << "current best move = " << bestMove << endl;
		cout << "trying " << moves[i] << endl;
		player.gameState.update(moves[i]);
		int value = player.minimax(player.gameState, WHITE, 1, alpha, beta);
		cout << "value = " << value << endl;
		cout << "bestValue = " << bestValue << endl;
		if (value > bestValue){
			bestValue = value;
			bestMove = moves[i];
		}

		alpha = max(alpha, bestValue);
		player.gameState.revert(moves[i], BLACK);
	
	}

	cout << "best opening move = " << bestMove << endl;
*/
	return 0;
}
