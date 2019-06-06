#include "../include/player.h"

MCTNode tree_policy(MCTNode node){
	string bestMove;
	double bestValue = 0.0;
	for (string move : node.moves){
		State tmpState = node.state;
	}
}

void mcts(State state){
	MCTNode root("None", NULL, state, true);
	MCTNode node;
	State currState;

	while (1){

		node = root;
		currState = state;
		while (!node.is_leaf()){
			node = tree_policy(node);
			currState.update(node.move);
		}

	}
}

int main(int argc, char *argv[]){

	
/*
	// fetch port number
	char serverIp[] = "127.0.0.1";
  	int port;
	//bool dispBoard = false;

	// TODO flag settings for bash commands
	for (int i = 0; i < argc; ++i){
		cout << argv[i] << endl;
		if (argv[i] == (char*)"--print-board"){
	//		dispBoard = true;
		}
		else{
			port = atoi(argv[1]);
		}
	}


	// create player and try connecting to the server    
	Player player(serverIp, port);
	player.run(false);

*/

	cout << "Hello, Random Bits." << endl;
	Player player;
	player.gameState.set_size(3, 3);
	player.gameState.create_graph();
	player.set_state("?a1Ba3;Wa2?b1;Bb2?b3;Wc1?c2;");

	State state = player.gameState;
	state.update("Ba1Bb?a3");
	player.gameState.show();
	state.show();

	return 0;
}
