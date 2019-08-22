// game.cpp

#include "../include/game.h"

State::State(){
	numRows = 0;
	numColumns = 0;
	emptyCount = 0;
	blackCount = 0;
	whiteCount = 0;
	neutralCount = 0;
	playerJustMoved = 2;
}

State::~State(){

}

void State::set_size(uint16_t rows, uint16_t cols){
	numRows = rows;
	numColumns = cols;
	graph.resize(numRows*numColumns);
}

void State::create_graph(){
	for (uint16_t i = 0; i < numRows*numColumns; ++i){
		graph[i].first = EMPTY;
		set_nbrs(graph[i].second, i);
	}
}

bool State::valid_pos(uint16_t key){
	return 0 <= key and key < numRows*numColumns;
}
void State::set_nbrs(vector<uint16_t>& nbrs, uint16_t key){
	if (valid_pos(key+numColumns))	nbrs.push_back(key+numColumns);
	if (valid_pos(key-numColumns))	nbrs.push_back(key-numColumns);
	if (valid_pos(key-1) and (key%numColumns > 0))nbrs.push_back(key-1);
	if (valid_pos(key+1) and (key%numColumns < numColumns-1))
		nbrs.push_back(key+1);
	if (valid_pos(key-numColumns+1)and(key%numColumns < numColumns-1))
		nbrs.push_back(key-numColumns+1);

	if (valid_pos(key+numColumns-1) and (key%numColumns > 0))
		nbrs.push_back(key+numColumns-1);
}

uint16_t State::num_nbrs(uint16_t row, uint16_t col){
	if (row == 0)	return (col == 0 ? 4 : 5);
	else if (row == numRows - 1){
		return (col == numColumns-1 ? 5 : 4);
	}
	else{
		return (col == 0 || col == numColumns-1 ? 5 : 6);
	}
}

string State::get_value(uint16_t row, uint16_t col){
	Valtype value = graph[row*numColumns+col].first;
	if (value == BLACK)	return "B";
	else if (value == WHITE)	return "W";
	else if (value == NEUTRAL)	return "?";
	else						return ".";
}

void State::switch_turns(){
	playerJustMoved = 3 - playerJustMoved;
}

void State::do_move(Action action){
	update(action);
	switch_turns();
}

void State::show(){
	cout << endl << " ";

	for (uint16_t col = 1; col <= numColumns; ++col){
		if (col < 10)	cout << "  " << col;
		else			cout << " " << col;
	}
	cout << endl;

	for (uint16_t row = 0; row < numRows; ++row){
		for (uint16_t padRow = 0; padRow < row; ++padRow){
			cout << " ";
		}
		cout << (char)('a'+row) << "\\";
		for (uint16_t col = 0; col < numColumns; ++col){
			cout << "  " << get_value(row, col);
		}
		cout << "  \\" << (char)('a'+row) << endl;
	}
	for (uint16_t row = 0; row < numRows+2; ++row){
		cout << " ";
	}

	for (uint16_t col = 1; col <= numColumns; ++col){
		if (col < 10)	cout << "  " << col;
		else			cout << " " << col;
	}
	cout << endl << endl;
}

// pos to string
string State::get_key(uint16_t row, uint16_t col){
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


bool State::valid_pos(uint16_t row, uint16_t col){
	return (row>=0 && row<numRows && col>=0 && col<numColumns);
}

// string to pos
uint16_t State::get_row(string pos){
	return (uint16_t)(pos[0] - 'a');
}

uint16_t State::get_col(string pos){
	return (uint16_t)(pos[1] - '1');
}

void State::update(Action action){
	uint16_t i = 0;
	uint16_t j = 1;
	string pos;
	int key;
	Valtype val;
	while (i < action.move.length()){
		if (action.move[j]=='B' || action.move[j]=='W' || action.move[j]=='?'
				|| action.move[j]=='.'){

			pos = action.move.substr(i+1, j-i-1).c_str();
			key = get_row(pos) * numColumns + get_col(pos);

			if (graph[key].first != EMPTY){
				if (graph[key].first == BLACK)		blackCount--;
				else if (graph[key].first == WHITE)	whiteCount--;
				else if (graph[key].first == NEUTRAL)neutralCount--;
			}

			if (action.move[i] == '.'){
				if (graph[key].first == BLACK)	blackCount--;
				else if (graph[key].first == WHITE)	whiteCount--;
				else if (graph[key].first == NEUTRAL)neutralCount--;
				graph[key].first = EMPTY;
			}
			else{
				if (action.move[i] == 'B'){
					blackCount++;
					graph[key].first = BLACK;
				}
				else if (action.move[i] == 'W'){
					whiteCount++;
					graph[key].first = WHITE;
				}
				else if (action.move[i] == '?'){
					neutralCount++;
					graph[key].first = NEUTRAL;
				}
			}
			i = j;
		}
		j++;
	}


}

void State::revert(Action action, char stone){
	uint16_t numStones = 0;
	uint16_t numNeutrals = 0;
	for (uint16_t i = 0; i < action.move.length(); ++i){
		if (action.move[i] == stone)			numStones++;
		else if (action.move[i] == '?')	numNeutrals++;
	}

	if (numNeutrals != 1)	return;

	if (numStones == 1){
		for (uint16_t i = 0; i < action.move.length(); ++i){
			if (action.move[i] == stone){
				if (stone == 'B')	blackCount--;
				else				whiteCount--;
				action.move[i] = '.';
			}

			if (action.move[i] == '?'){
				neutralCount--;
				action.move[i] = '.';
			}
		}
	}
	else if (numStones == 2){
		for (uint16_t i = 0; i < action.move.length(); ++i){
			if (action.move[i] == stone){
				action.move[i] = '?';
				if (stone == 'B')	blackCount--;
				else				whiteCount--;
				neutralCount++;
			}
			else if (action.move[i] == '?'){
				if (stone == 'B'){
					action.move[i] = 'B';
					blackCount++;
				}
				else{
					action.move[i] = 'W';
					whiteCount++;
				}
				neutralCount--;
			}
		}
	}
	update(action);
}

char State::status(){
	// check if black has won
	for (uint16_t col = 0; col < numColumns; ++col){
		if (graph[col].first == BLACK){
			// run dfs
			if (connected(col, numRows-1, true)){
				cout << "black won" << endl;
				return BLACK_WIN;
			}
		}
	}


	// check if white has won yet
	for (uint16_t row = 0; row < numRows; ++row){
		uint16_t key = row * numColumns;
		if (graph[key].first == WHITE){
			if (connected(key, numColumns-1, false)){
				cout << "white won" << endl;
				return WHITE_WIN;
			}
		}
	}

	uint16_t emptyCount = numRows*numColumns -
		(blackCount+whiteCount+neutralCount);

	if (emptyCount > 1){
		return GAME_NOT_OVER;
	}
	else{
		if (neutralCount >= 2){
			return GAME_NOT_OVER;
		}
		else{
			return DRAW;
		}
	}
	cout << "game not over" << endl;
	return GAME_NOT_OVER;
}

int State::next(){
	return 3 - playerJustMoved;
}

bool State::connected(uint16_t key0, uint16_t end, bool blackConnect){
	// construct a 'visited' array
	bool visited[numRows * numColumns];
	memset(visited, false, numRows*numColumns);

	stack<uint16_t> keyStack;
	keyStack.push(key0);
	visited[key0] = true;
	while (!keyStack.empty()){
		uint16_t curr = keyStack.top();
		keyStack.pop();


		uint16_t currCol = curr%numColumns;
		uint16_t currRow = (curr - currCol) / numColumns;

		if ((blackConnect and currRow == end) or
			(!blackConnect and currCol == end)){
			return true;
		}

		for (uint16_t adj : graph[curr].second){
			if (graph[adj].first == graph[curr].first){
				if (!visited[adj]){
					keyStack.push(adj);
					visited[adj] = true;
				}
			}
		}
	}
	return false;
}


void State::get_moves(vector<Action>& actions, string myStone){

	// store all cells where a stone may be placed
	vector<string> emptyPos;
	vector<string> stonePos;
	vector<string> neutralPos;
	for (uint16_t row = 0; row < numRows; ++row){
		for (uint16_t col = 0; col < numColumns; ++col){
			uint16_t key = row * numColumns + col;
			if (graph[key].first == EMPTY){
				emptyPos.push_back(get_key(row, col));
			}
			else if((graph[key].first==BLACK and myStone == "B") or
				(graph[key].first == WHITE and myStone == "W")){
				stonePos.push_back(get_key(row, col));
			}
			else if (graph[key].first == NEUTRAL){
				neutralPos.push_back(get_key(row, col));
			}
		}
	}

	if (emptyPos.size() >= 2){

		for (uint16_t i = 0; i < emptyPos.size(); ++i){
			for (uint16_t j = i+1; j < emptyPos.size(); ++j){
				string key0 = myStone;
				key0 += emptyPos[i] + "?" + emptyPos[j];
				actions.push_back(Action(key0));

				string key1 = "?";
				key1 += emptyPos[i] + myStone  + emptyPos[j];
				actions.push_back(Action(key1));
			}
		}
	}

	if (neutralPos.size() >= 2 && stonePos.size() >= 1){

		for (uint16_t i = 0; i < neutralPos.size(); ++i){
			for (uint16_t j = i + 1; j < neutralPos.size(); ++j){
				for (uint16_t k = 0; k < stonePos.size(); ++k){
					string key = myStone;
					key += neutralPos[i]+myStone+neutralPos[j];
					key += "?" + stonePos[k];
					actions.push_back(Action(key));
				}
			}
		}
	}
}

State& State::operator=(State& s){
	numRows = s.numRows;
	numColumns = s.numColumns;
	emptyCount = s.emptyCount;
	
}

