#ifndef _GAME_

#define _GAME_

#define BLACK   	1
#define WHITE   	(1<<1)
#define NEUTRAL		((1<<1)|1)
#define EMPTY   	0
#define DRAW		'#'
#define KEYBITS		16		// number of bits for key
#define VALBITS		2		// number of bits for value

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <bitset>

using namespace std;

typedef unsigned int uint;
typedef	bitset<KEYBITS> Keytype;
typedef bitset<VALBITS> Valtype;

class State{
	public:
		uint numRows;
		uint numColumns;
		vector< Valtype > graph;
		
		uint emptyCount;
		uint neutralCount;
		uint blackCount;
		uint whiteCount;
		
		uint movesCount;

		State();
		~State();
		void set_size(uint rows, uint cols);
		void create_graph();
		string get_value(uint row, uint col);
		void show();
		string get_key(uint row, uint col);
		void update(string move);
		bool connected(string key0, string key1);
		char status();
		void revert(string move, char stone);

	private:
		void add_nbrs(uint row, uint col);
		uint num_nbrs(uint row, uint col);
		uint get_row(string pos);
		uint get_col(string pos);
		bool valid_pos(uint row, uint col);
};



#endif	// _GAME_
