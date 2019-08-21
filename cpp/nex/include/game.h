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
private:
public:
	string move;
	Action();
	Action(string m)	:	move(m)	{}
	Action& operator=(const Action& a){
		move = a.move;
		return *this;
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
	// change graph to be like solver2 graph
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
	void reset();
	string get_key(uint row, uint col);
	void update(Action a);
	bool connected(string key0, string key1);
	char status();
	bool valid_stone(char stone);
	bool is_valid(Action a, char stone);
	void clear();
	int next();
	void get_moves(vector<Action>& actions, string& myStone);
	State clone();

private:
	void add_nbrs(uint row, uint col);
	uint num_nbrs(uint row, uint col);
	uint get_row(string key);
	uint get_col(string key);
	bool valid_pos(uint row, uint col);

	bool valid_pos(string key);
};




#endif	// _GAME_
