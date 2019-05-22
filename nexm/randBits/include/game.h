#ifndef _GAME_

#define _GAME_

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

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <bitset>
#include <cstring>
#include <algorithm>

using namespace std;

typedef unsigned int uint;
typedef	bitset<KEYBITS> Keytype;
typedef bitset<VALBITS> Valtype;
typedef pair<Valtype, vector<uint> > Cell;

class State{
public:
	uint numRows;
	uint numColumns;
	vector< Cell > graph;
	
	uint emptyCount;
	uint neutralCount;
	uint blackCount;
	uint whiteCount;
	
	uint movesCount;

	State();
	~State();
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
	void operator=(State& state);

private:
	uint num_nbrs(uint row, uint col);
	uint get_row(string pos);
	uint get_col(string pos);
	bool valid_pos(uint key);
	bool valid_pos(uint row, uint col);
};



#endif	// _GAME_
