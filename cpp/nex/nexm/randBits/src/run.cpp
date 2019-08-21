
#include <stdio.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <fstream>

#include "../include/search.h"
// #include <mcts.h>

using namespace std;

struct ClientSocket{
	int port;
	struct hostent* host;
	sockaddr_in sendSockAddr;
	int clientSd;
};

class Player{
	State* state;
	// nex_mcts* mcts;
};

class Client{
private:
	ClientSocket _socket;
	uint movesCount;
	bool socketConnected;

	void init_vars();
	void attach_socket(char* servIp, int port);
	void read_settings(char* buff, uint& rows, uint& cols);
	void connect_server();
	
public:
	char myStone;
	State gameState;

	Client();
	Client(char* servIp, int port);
	~Client();

	void set_state(string moves);
	void run(bool disp);
	void solve(State state, bool isMax, bool disp);
	
	void get_moves(State state, vector<string>& moves, bool isMax);
	
	// negamax
	int evaluate(State state, bool isMax);
	string best_neg_move(State state, int depth, bool isMax, bool disp);
	int negamax(State state, int depth, bool isMax, int alpha, int beta, bool disp);
};


/*
TODO: to be implemented:
	- ExpansionStrategy
	- PlayoutStrategy
	- Scoring
	- Backpropagation
	- TerminationCheck
*/



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
	Client player;
	player.gameState.set_size(3, 3);
	player.gameState.create_graph();
	player.set_state("?a1Ba3;Wa2?b1;Bb2?b3;Wc1?c2;");



	return 0;
}
