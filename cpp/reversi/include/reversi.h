#ifndef _GAME_

#define _GAME_

#define BLACK_COIN		'B'
#define WHITE_COIN		'W'
#define EMPTY 			'-'

#define DRAW			'#'
#define GAME_NOT_OVER	'.'

#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

#include "../../solver/solver.h"

typedef pair<char, bool> Cell;

class ReversiAction: public Action{
public:
	string move;
	ReversiAction()	:	move("")	{}
	ReversiAction(string m)	:	move(m)	{}
	ReversiAction& operator=(const ReversiAction& a){
		move = a.move;
		return *this;
	}
};

struct Pos{
	int row, col;
	void add(Pos p){
		row += p.row;
		col += p.col;
	}
};

class ReversiState: public State{
public:
	uint16_t numRows;
	uint16_t numColumns;
	vector<Cell> graph;
	uint16_t blackCount;
	uint16_t whiteCount;
	uint16_t playerJustMoved;
	char status;

	ReversiState();
	~ReversiState();
	ReversiState& operator=(ReversiState& s);

	// board construction
	void set_size(uint16_t rows, uint16_t cols);
	void create_graph();
	
	// display
	void show();

	// transition
	// bool update(ReversiAction action);
	// void revert(ReversiAction& action);
	bool update(ReversiAction action);
	int process_move(struct Pos startPos, struct Pos changePos, char coin, char oppcoin);
	int flip_coins(Pos startPos, Pos changePos, char coin, char oppcoin);
	void update_scores(int numFlips, char coin);
	void switch_turns();
	int next();

	// get actions
	// void get_moves(vector<ReversiAction>& actions);
	// void get_moves(vector<ReversiAction>& actions, bool isMax);
	int validate_moves(bool isMax);
	void get_moves(vector<ReversiAction>& actions, bool isMax);

	bool posInBounds(struct Pos p);
	bool posInBounds(int row, int col);
	int generate_moves(struct Pos selectPos, bool isMax);
	// // terminal test
	// char check_win();
	// bool connected(uint16_t key0, uint16_t end, bool blackConnect);
	// int evaluate(bool isMax);

	char player1();
	char player2();
	char draw();

	int evaluate(bool isMax);

	bool gameover();

// private: TODO
	bool valid_pos(uint16_t key);
	bool valid_pos(uint16_t row, uint16_t col);
	string get_key(uint16_t row, uint16_t col);
	uint16_t get_row(string pos);
	uint16_t get_col(string pos);

	string get_value(uint16_t row, uint16_t col);
	uint16_t num_nbrs(uint16_t row, uint16_t col);
};



#endif	// _GAME_
