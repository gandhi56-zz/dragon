// game.cpp

#include "../include/ttt.h"

TTTState::TTTState(){
	numRows = 0;
	numColumns = 0;
	playerJustMoved = 2;
	status = '.';
}

TTTState::~TTTState(){

}

void TTTState::set_size(uint16_t rows, uint16_t cols){
	numRows = rows;
	numColumns = cols;
	graph.resize(numRows*numColumns);
}

void TTTState::create_graph(){
	playerJustMoved = 2;
	status = '.';
	for (uint16_t i = 0; i < numRows*numColumns; ++i){
		graph[i] = EMPTY;
	}
}

bool TTTState::valid_pos(uint16_t key){
	return 0 <= key and key < numRows*numColumns;
}

void TTTState::switch_turns(){
	playerJustMoved = 3 - playerJustMoved;
}

void TTTState::do_move(TTTAction action){
	update(action);
	switch_turns();
	status = check_win();
}

void TTTState::show(){
	cout << endl << "  ";
	for (int c = 1; c <= numColumns; ++c)
		cout << c << " ";
	cout << endl;
	for (int r = 0; r < numRows; ++r){
		cout << (char)('a'+r) << " ";
		for (int c = 0; c < numColumns; ++c){
			cout << graph[r*numRows+c] << " ";
		}
		cout << endl;
	}
	cout << endl << endl;
}

// pos to string
string TTTState::get_key(uint16_t row, uint16_t col){
	string key;
	key.push_back('a'+row);

	if (col < 9){
		key.push_back('1'+col);
	}
	else{
		key.push_back('1');
		key.push_back('0' + (col+1)%10);
	}
	return key;
}


bool TTTState::valid_pos(uint16_t row, uint16_t col){
	return (row>=0 && row<numRows && col>=0 && col<numColumns);
}

// string to pos
uint16_t TTTState::get_row(string pos){
	return (uint16_t)(pos[0] - 'a');
}

uint16_t TTTState::get_col(string pos){
	return (uint16_t)(pos[1] - '1');
}

bool TTTState::update(TTTAction action){
	//cout << "playing " << action.move << endl;
	uint16_t i = 0;
	uint16_t j = 1;
	string pos;
	int key;
	while (i < action.move.length()){
		if (action.move[j]==PLAYER_X || action.move[j]==PLAYER_O || action.move[j]==EMPTY){
			pos = action.move.substr(i+1, j-i-1);
			key = get_row(pos) * numColumns + get_col(pos);
			if (action.move[i]==EMPTY or action.move[i]==PLAYER_X or action.move[i]==PLAYER_O){
				graph[key] = action.move[i];
			}
			else{
				return false;
			}
			i = j;
		}
		j++;
	}
	return true;
}

void TTTState::revert(TTTAction action){
	for (uint16_t i = 0; i < action.move.length(); ++i){
		if (action.move[i] == PLAYER_X or action.move[i] == PLAYER_O){
			action.move[i] = '.';
		}
	}
	update(action);
}

int TTTState::next(){
	return 3 - playerJustMoved;
}

char TTTState::check_win(){
	for (int i = 0; i < numRows; ++i){
		if (graph[i*numRows] != EMPTY){
			bool winFound = true;
			for (int c = 1; c < numColumns; ++c){
				if (graph[i*numRows] != graph[i*numRows+c]){
					winFound = false;
				}
			}

			if (winFound){
				return graph[i*numRows];
			}
		}

		if (graph[i] != EMPTY){
			bool winFound = true;
			for (int r = 1; r < numRows; ++r){
				if (graph[i] != graph[r*numRows+i]){
					winFound = false;
				}
			}

			if (winFound){
				return graph[i];
			}
		}
	}

	bool winFound = true;
	for (int i = 1; i < numRows; ++i){
		if (graph[0] != graph[i*numRows+i]){
			winFound = false;
		}
	}
	if (winFound)
		return graph[0];

	winFound = true;
	for (int i = 1; i < numRows; ++i){
		if (graph[numColumns-1] != graph[i*numRows+(numColumns-1-i)]){
			winFound = false;
		}
	}
	if (winFound)
		return graph[numColumns-1];

	for (int i = 0; i < numRows*numColumns; ++i){
		if (graph[i] == EMPTY){
			return GAME_NOT_OVER;
		}
	}
	return DRAW;
}

void TTTState::get_moves(vector<TTTAction>& actions){
	string myStone = "x";
	if (playerJustMoved == 1)
		myStone = "o";
	for (int r = 0; r < numRows; ++r){
		for (int c = 0; c < numColumns; ++c){
			if (graph[r*numRows+c] == EMPTY){
				actions.push_back(TTTAction(myStone+get_key(r, c)));
			}
		}
	}
}

void TTTState::get_moves(vector<TTTAction>& actions, string myStone){
	for (int r = 0; r < numRows; ++r){
		for (int c = 0; c < numColumns; ++c){
			if (graph[r*numRows+c] == EMPTY){
				actions.push_back(TTTAction(myStone+get_key(r, c)));
			}
		}
	}
}

TTTState& TTTState::operator=(TTTState& s){
	numRows = s.numRows;
	numColumns = s.numColumns;
	graph = s.graph;
	playerJustMoved = s.playerJustMoved;
	status = s.status;
	return *this;
}

char TTTState::player1(){
	return PLAYER_X;
}

char TTTState::player2(){
	return PLAYER_O;
}

char TTTState::draw(){
	return DRAW;
}
