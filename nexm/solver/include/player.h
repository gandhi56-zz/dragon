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
	State state;
	uint movesCount;
	char myStone;

	void read_settings(char* buff, uint& rows, uint& cols);
	void connect_server();

	// minimax
	vector<string> get_moves(State state, char stone);
	string best_move(State state, char stone, uint depth);
	int minimax(State state, char stone, uint depth, int& alpha, int& beta);
	int max_value(State state, uint depth, int& alpha, int& beta);
	int min_value(State state, uint depth, int& alpha, int& beta);
	int evaluate(char result);
	void set_state(string moves);
public:
	Player(char* servIp, int port);
	~Player();
	void run();
};

#endif // _PLAYER_
