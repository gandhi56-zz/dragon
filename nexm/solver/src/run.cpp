#include "../include/player.h"

#include <algorithm>	// for random_shuffle

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

/*
	Player player;
	player.gameState.set_size(2, 2);
	player.gameState.create_graph();


	vector<string> moves = player.get_moves(player.gameState, BLACK);

	srand(unsigned (time(0)));
	random_shuffle(moves.begin(), moves.end());
	int alpha = -100;
	int beta = 100;

	int bestValue = -100;
	string bestMove;
	for (uint i = 0; i < moves.size(); ++i){

		cout << "current best move = " << bestMove << endl;
		cout << i << " : trying " << moves[i] << endl;
		player.gameState.update(moves[i]);
		int value = player.negamax(player.gameState, 100, true, alpha, beta);
		if (value >= bestValue){
			bestValue = value;
			bestMove = moves[i];
		}

		cout << "value = " << value << endl;
		cout << "bestValue = " << bestValue << endl;
		player.gameState.revert(moves[i], BLACK);
	
	}
*/


	return 0;
}
