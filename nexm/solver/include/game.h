#ifndef _GAME_

#define _GAME_

#define BLACK   'B'
#define WHITE   'W'
#define NEUTRAL '?'
#define EMPTY   '.'

#include <iostream>
#include <vector>
#include <map>
#include <stack>

using namespace std;

typedef unsigned int uint;

struct Cell{
	string key;
	char value;
	vector<string> adjKeys;

	Cell();
	Cell(string _key);
};

class State{
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

	private:
		void add_nbrs(uint row, uint col);
		uint num_nbrs(uint row, uint col);
		uint get_row(string key);
		uint get_col(string key);
		bool valid_pos(uint row, uint col);
};




#endif	// _GAME_