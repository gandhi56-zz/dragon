#ifndef _PLAYER_
#define _PLAYER_


#define BLACK   		1
#define WHITE   		(1<<1)
#define NEUTRAL			((1<<1)|1)
#define EMPTY   		0
#define KEYBITS			16		// number of bits for key
#define VALBITS			2		// number of bits for value
#define DRAW			'#'
#define BLACK_WIN		'B'
#define WHITE_WIN		'W'
#define GAME_NOT_OVER	'?'
#define DEPTH_LIMIT 	100

#include <iostream>
#include <string>	// required?
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

#include <unordered_map>
#include <stack>
#include <bitset>
#include <cstring>	// required?

#include <mcts.h>

using namespace std;

typedef unsigned int uint;
typedef bitset<VALBITS> Valtype;
typedef pair<Valtype, vector<uint> > Cell;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class Board:public State{
public:
	uint numRows;
	uint numColumns;
	vector< Cell > graph;
	
	uint emptyCount;
	uint neutralCount;
	uint blackCount;
	uint whiteCount;
	
	uint movesCount;

	Board();
	~Board();
	void set_size(uint rows, uint cols);
	void create_graph();
	void set_nbrs(vector<uint>& nbrs, uint key);
	string get_value(uint row, uint col);
	void show();
	string get_key(uint row, uint col);
	void update(string move);
	bool connected(uint key0, uint end, bool blackConnect);
	char status();
	void revert(string move, char stone);

private:
	uint num_nbrs(uint row, uint col);
	uint get_row(string pos);
	uint get_col(string pos);
	bool valid_pos(uint key);
	bool valid_pos(uint row, uint col);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class NexAction:public Action<Board>{
public:
	string action;
	NexAction()	:	action("")	{}
	NexAction(string _action)	:	action(_action)	{}
	NexAction(const NexAction& actObj)	:	NexAction(actObj.action)	{}
	void execute(Board* board)	override	{
		board->update(action);
	}
	size_t hash()	override	{
		size_t val = 0;
		for (char c : action){
			val ^= c;
		}
		return val;
	}

	void print(ostream &strm)	override	{
		strm << "Playing " << action;
	}

	bool equals(Action* a)	override	{
		NexAction* _action = static_cast<NexAction*>(a);
		return *this == *_action;
	}

	bool operator==(const NexAction& actObj){
		return action == actObj.action;
	}

	bool operator!=(const NexAction& actionObj){
		return !operator==(actionObj);
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class NexExpansionStrategy:public ExpansionStrategy<Board, NexAction>{
	string nextMove;
public:
	NexExpansionStrategy(Board* board);
	NexAction* generateNext();
	~NexExpansionStrategy()	override	{}
private:
	void generateNextMove(string& move);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class NexPlayoutStrategy:public PlayoutStrategy<Board, NexAction>{
public:
	NexPlayoutStrategy(Board* board);
	void generateRandom(NexAction* action)	override;
	~NexPlayoutStrategy()	override	{}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

typedef MCTS<Board, 
	NexAction, NexExpansionStrategy, NexPlayoutStrategy> NexMcts;
class NexMCTSPlayer	{
	Board* board;
	NexMcts* mcts;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class NexBackpropagation:public Backpropagation<Board>	{

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

struct ClientSocket{
	int port;
	struct hostent* host;
	sockaddr_in sendSockAddr;
	int clientSd;
};
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
	Board board;

	Player();
	Player(char* servIp, int port);
	~Player();

	void set_state(string moves);
	void run(bool disp);
	int evaluate(Board board, bool isMax);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // _PLAYER_
