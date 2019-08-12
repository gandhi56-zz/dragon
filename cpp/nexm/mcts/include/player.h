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

#include "../include/board.h"

#define DEPTH_LIMIT 100

using namespace std;

struct ClientSocket{
	int port;
	struct hostent* host;
	sockaddr_in sendSockAddr;
	int clientSd;
};

class Node{
	State* state;
	Node* parent;
	vector<Node*> children;
	string* action;	// move played from parent to reach this node
	int numVisits;
	float score;
public:
	Node(State* _state, Node* _parent, string* _action)	:
		state(_state), parent(_parent), action(_action), numVisits(0), score(0.0)	{}

	State* get_state(){
		return state;
	}

	Node* get_parent(){
		return parent;
	}

	vector<Node*>& get_children(){
		return children;
	}

	string* get_action(){
		return action;
	}

	string* get_next_action(){
		// TODO define expansion strategy here
		return nullptr;
	}

	void add_child(Node* child){
		children.push_back(child);
	}

	bool expandable(){
		// TODO add a function that checks if there are any more moves generatable from here
		return children.empty();
	}

	void update(float s){
		score += s;
		numVisits++;
	}

	float get_score(){
		return score / numVisits;
	}

	int get_visits(){
		return numVisits;
	}

	~Node(){
		delete state;
		delete action;
		for (Node* child : children){
			delete child;
		}
	}
};

class Player{

private:
	ClientSocket _socket;
	uint16_t movesCount;
	bool socketConnected;

	void init_vars();
	void attach_socket(char* servIp, int port);
	void read_settings(char* buff, uint16_t& rows, uint16_t& cols);
	void connect_server();

public:
	char myStone;
	State gameState;

	Player();
	Player(char* servIp, int port);
	~Player();

	void set_state(string moves);
	void run(bool disp);
	void solve(State state, bool isMax, bool disp);

	vector<string> get_moves(State state, bool isMax);

	// negamax
	int evaluate(State state, bool isMax);
	string best_move(State state, int depth, bool isMax, bool disp);
	int negamax(State state, int depth, bool isMax, int alpha, int beta, bool disp);
};

#endif // _PLAYER_
