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

class Board{
    public:
        uint numRows;
        uint numColumns;
        map<string, Cell> state;
    
        Board(uint numRows, uint numColumns);
        void show();
        string get_key(uint row, uint col);
        bool valid_pos_str(string key);
        bool valid_stone(char stone);
        bool valid_pos(uint row, uint col);

    private:
        void add_neighbours(uint row, uint col);
        uint count_neighbours(uint row, uint col);
        void add_col_edge();
        uint get_row(string key);
        uint get_col(string key);
};

class Game{
    public:
        Board* board;

        Game(uint numRows, uint numColumns);
        ~Game();
        void init(uint numRows, uint numColumns);
        bool is_valid(string move, char stone);
        void update(string move, char stone);
        char terminal_test();

    private:
        void update_cell(string movePos, char stone);
        bool is_connected(string key0, string key1);
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
