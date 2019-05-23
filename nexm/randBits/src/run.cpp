#include "../include/player.h"


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

	MCTNode node("None", NULL, player.gameState, true);
	MCTNode* nodeptr = &node;
	nodeptr->add_child("Ba1Bb1?a3");
	cout << "child added" << endl;
	nodeptr->childNodes[0]->state.show();
	return 0;
}
