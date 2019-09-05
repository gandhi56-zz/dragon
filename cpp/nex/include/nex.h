#ifndef _GAME_

#define _GAME_

#define BLACK   		1
#define WHITE   		(1<<1)
#define NEUTRAL			((1<<1)|1)
#define EMPTY   		0
#define VALBITS			2		// number of bits for value
#define DRAW			'#'
#define BLACK_WIN		'B'
#define WHITE_WIN		'W'
#define GAME_NOT_OVER	'.'

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <bitset>
#include <cstring>
using namespace std;
typedef bitset<VALBITS> Valtype;
typedef pair<Valtype, vector<uint16_t> > Cell;

#include "../../solver/solver.h"

class NexAction: public Action{
public:
	string move;
	NexAction()	:	move("")	{}
	NexAction(string m)	:	move(m)	{}
	NexAction& operator=(const NexAction& a){
		move = a.move;
		return *this;
	}
};

class NexState: public State{
public:
	uint16_t numRows;
	uint16_t numColumns;
	vector< Cell > graph;
	uint16_t playerJustMoved;
	char status;

	NexState();
	~NexState();
	NexState& operator=(NexState& s);

	// board construction
	void set_size(uint16_t rows, uint16_t cols);
	void create_graph();
	void set_nbrs(vector<uint16_t>& nbrs, uint16_t key);
	
	// display
	void show();

	// transition
	bool update(NexAction action);
	void revert(NexAction action);
	int next();
	void switch_turns();
	void do_move(NexAction action);

	// get actions
	void get_moves(vector<NexAction>& actions);
	void get_moves(vector<NexAction>& actions, bool isMax);

	// terminal test
	bool connected(uint16_t key0, uint16_t end, bool blackConnect);
	char check_win();

	// terminal test accessors
	char player1();
	char player2();
	char draw();

private:
	uint16_t num_nbrs(uint16_t row, uint16_t col);
	uint16_t get_row(string pos);
	uint16_t get_col(string pos);
	bool valid_pos(uint16_t key);
	bool valid_pos(uint16_t row, uint16_t col);
	string get_value(uint16_t row, uint16_t col);
	string get_key(uint16_t row, uint16_t col);
};



#endif	// _GAME_
