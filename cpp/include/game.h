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

class Action{
	string move;
	Action();
	Action(string m)	:	move(m)	{}
	Action& operator=(const Action& a){
		move = a.move;
		return *this;
	}

	friend ostream &operator<<(ostream &output){
	
	}
};

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
		uint playerJustMoved;

		State();
		~State();

		void set_size(uint rows, uint cols);
		void create_graph();
		void show();
		string get_key(uint row, uint col);
		void update(string move);
		bool connected(string key0, string key1);
		char status();
		bool valid_stone(char stone);
		bool is_valid(string move, char stone);
		void clear();

		void do_move();

	private:
		void add_nbrs(uint row, uint col);
		uint num_nbrs(uint row, uint col);
		uint get_row(string key);
		uint get_col(string key);
		bool valid_pos(uint row, uint col);

		bool valid_pos(string key);
};




#endif	// _GAME_
