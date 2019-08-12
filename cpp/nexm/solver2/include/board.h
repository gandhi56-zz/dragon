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
#define GAME_NOT_OVER	'?'

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <bitset>
#include <cstring>

using namespace std;

typedef bitset<VALBITS> Valtype;
typedef pair<Valtype, vector<uint16_t> > Cell;

class State{
	public:
		uint16_t numRows;
		uint16_t numColumns;
		vector< Cell > graph;

		uint16_t emptyCount;
		uint16_t neutralCount;
		uint16_t blackCount;
		uint16_t whiteCount;

		uint16_t movesCount;

		State();
		~State();
		void set_size(uint16_t rows, uint16_t cols);
		void create_graph();
		void set_nbrs(vector<uint16_t>& nbrs, uint16_t key);
		string get_value(uint16_t row, uint16_t col);
		void show();
		string get_key(uint16_t row, uint16_t col);
		void update(string move);
		bool connected(uint16_t key0, uint16_t end, bool blackConnect);
		char status();
		void revert(string move, char stone);

	private:
		uint16_t num_nbrs(uint16_t row, uint16_t col);
		uint16_t get_row(string pos);
		uint16_t get_col(string pos);
		bool valid_pos(uint16_t key);
		bool valid_pos(uint16_t row, uint16_t col);
};



#endif	// _GAME_
