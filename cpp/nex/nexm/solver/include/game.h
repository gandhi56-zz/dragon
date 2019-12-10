#ifndef _GAME_

#define _GAME_

#define BLACK   'B'
#define WHITE   'W'
#define NEUTRAL '?'
#define EMPTY   '.'
#define DRAW	'#'

#include <iostream>
#include <vector>
#include <map>
#include <stack>

using namespace std;

typedef unsigned int uint;

struct Cell{

	/*
	 * vars
	 * - key: cell id
	 * - value: state of the cell
	 * - adjKeys: vector containing 
	 *   the keys of each nbr cell
	 *
	 * methods:
	 * - constructors
	 *
	 * space:
	 * - ~25 bytes
	 */

	char value;
	vector<string> adjKeys;

	Cell();
};

class State{

	/*
	 *	vars
	 *	- numRows, numColumns : dimensions of the board
	 *	- graph : state configuration of the board
	 *	- count : #cells of each type on the board,
	 *			BLACK, WHITE, NEUTRAL
	 *	- movesCount : #moves processed so far
	 *
	 */


	public:
		uint numRows;
		uint numColumns;
		map<string, Cell> graph;
		map<char, uint> count;
		uint movesCount;

		State();
		~State();
		void set_size(uint rows, uint cols);
		void create_graph();
		void show();
		string get_key(uint row, uint col);
		void update(string move);
		bool connected(string key0, string key1);
		char status();
		void revert(string move, char stone);

	private:
		void add_nbrs(uint row, uint col);
		uint num_nbrs(uint row, uint col);
		uint get_row(string key);
		uint get_col(string key);
		bool valid_pos(uint row, uint col);
};




#endif	// _GAME_
