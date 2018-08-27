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

#include "game.h"

using namespace std;

struct ClientSocket{
	int port;
	struct hostent* host;
	sockaddr_in sendSockAddr;
	int clientSd;
};

class Player{
public:

	State state;
	uint movesCount;
	
	void connect_server();
	void read_settings(char* buff, uint& rows, uint& cols);
	void user_move(char* move);
	void set_state(string move);

	int status;
	ClientSocket _socket;
	Player(char* servIp, int port);
	~Player();
	void run();
	int send_data(const char* data);
	int recieve_data(char* data);
};

#endif // _PLAYER_
