#ifndef _GAME_

#define _GAME_
#define BLACK   		1
#define WHITE   		(1<<1)
#define NEUTRAL			((1<<1)|1)
#define EMPTY   		0
#define VALBITS			2		// number of bits for value
#define DRAW			'#'
#define BLACK_STONE		'B'
#define WHITE_STONE		'W'
#define GAME_NOT_OVER	'.'

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <bitset>
#include <cstring>
using namespace std;
typedef bitset<VALBITS> Valtype;
typedef pair<Valtype, vector<int16_t> > Cell;

#include "../../solver/solver.h"

class HexAction: public Action{
public:
	string move;
	HexAction()	:	move("")	{}
	HexAction(string m)	:	move(m)	{}
	HexAction& operator=(const HexAction& a){
		move = a.move;
		return *this;
	}
};

class HexState: public State{
public:
	int16_t numRows;
	int16_t numColumns;
	vector< Cell > graph;
	int16_t emptyCount;
	int16_t blackCount;
	int16_t whiteCount;
	int16_t playerJustMoved;
	char status;

	HexState();
	~HexState();
	HexState& operator=(HexState& s);

	// board construction
	void set_size(int16_t rows, int16_t cols);
	void create_graph();
	void set_nbrs(vector<int16_t>& nbrs, int16_t key);
	
	// display
	void show();

	// transition
	bool update(HexAction action);
	void revert(HexAction& action);
	int next();
	void switch_turns();
	
	// get actions
	void get_moves(vector<HexAction>& actions);
	void get_moves(vector<HexAction>& actions, bool isMax);
	
	// terminal test
	char check_win();
	bool connected(int16_t key0, int16_t end, bool blackConnect);
	int evaluate(bool isMax);
	bool gameover();

	char player1();
	char player2();
	char draw();
private:
	string get_key(int16_t row, int16_t col);
	string get_value(int16_t row, int16_t col);
	int16_t num_nbrs(int16_t row, int16_t col);
	int16_t get_row(string pos);
	int16_t get_col(string pos);
	bool valid_pos(int16_t key);
	bool valid_pos(int16_t row, int16_t col);
};



#endif	// _GAME_
