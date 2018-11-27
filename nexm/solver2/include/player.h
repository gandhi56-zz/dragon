#ifndef _PLAYER_
#define _PLAYER_

#include <iostream>
#include <string>
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

#include <vector>

#include "../include/game.h"

using namespace std;

struct ClientSocket{
	int port;
	struct hostent* host;
	sockaddr_in sendSockAddr;
	int clientSd;
};

class Player{

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

	Player();
	Player(char* servIp, int port);
	~Player();

	void set_state(string moves);
	void run();
	void solve(State state, char stone);
	
	// minimax
	vector<string> get_moves(State state, char stone);
	string best_move(State state, char stone, uint depth);
	int minimax(State state, char stone, uint depth, int& alpha, 
		int& beta);
	int max_value(State state, uint depth, int& alpha, int& beta);
	int min_value(State state, uint depth, int& alpha, int& beta);

	// negamax
	int evaluate(State state, bool isMax);
	string best_neg_move(State state, int depth, bool isMax);
	int negamax(State state, int depth, bool isMax, int alpha, int beta);
};

#endif // _PLAYER_
