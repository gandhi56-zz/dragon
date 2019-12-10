#ifndef _GAME_

#define _GAME_
#define PLAYER_X   		'x'
#define PLAYER_O   		'o'
#define EMPTY   		'.'
#define DRAW			'#'
#define GAME_NOT_OVER	'.'

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <cstring>
using namespace std;

#include "../../solver/solver.h"

class TTTAction: public Action{
public:
	string move;
	TTTAction()	:	move("")	{}
	TTTAction(string m)	:	move(m)	{}
	TTTAction& operator=(const TTTAction& a){
		move = a.move;
		return *this;
	}
};

class TTTState: public State{
public:
	uint16_t numRows;
	uint16_t numColumns;
	vector< unsigned char > graph;
	uint16_t playerJustMoved;
	char status;

	TTTState();
	~TTTState();
	void set_size(uint16_t rows, uint16_t cols);
	void create_graph();
	void show();
	string get_key(uint16_t row, uint16_t col);
	bool update(TTTAction action);
	bool connected(uint16_t key0, uint16_t end, bool blackConnect);
	void revert(TTTAction action);
	int next();
	void get_moves(vector<TTTAction>& actions);
	void get_moves(vector<TTTAction>& actions, bool isMax);
	void switch_turns();
	void do_move(TTTAction action);
	TTTState& operator=(TTTState& s);
	char check_win();
	char player1();
	char player2();
	char draw();
	int evaluate(bool isMax);
	bool gameover();
private:
	uint16_t num_nbrs(uint16_t row, uint16_t col);
	uint16_t get_row(string pos);
	uint16_t get_col(string pos);
	bool valid_pos(uint16_t key);
	bool valid_pos(uint16_t row, uint16_t col);
};



#endif	// _GAME_
