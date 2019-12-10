
#include "../include/go.h"

GoState::GoState(){
	numRows = 0;
	numColumns = 0;
	playerJustMoved = 2;
	status = '.';
}

GoState::~GoState(){

}

void GoState::set_size(uint16_t rows, uint16_t cols){
	numRows = rows;
	numColumns = cols;
	graph.resize(numRows*numColumns);
}

void GoState::create_graph(){
	playerJustMoved = 2;
	status = '.';
	for (uint16_t i = 0; i < numRows*numColumns; ++i){
		graph[i].first = EMPTY;
		set_nbrs(graph[i].second, i);
	}
}

bool GoState::valid_pos(uint16_t key){
	return 0 <= key and key < numRows*numColumns;
}
void GoState::set_nbrs(vector<uint16_t>& nbrs, uint16_t key){
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

string GoState::get_value(uint16_t row, uint16_t col){
	Valtype value = graph[row*numColumns+col].first;
	if 		(value == BLACK)	return "B";
	else if (value == WHITE)	return "W";
	else						return ".";
}

void GoState::switch_turns(){
	playerJustMoved = 3 - playerJustMoved;
}

void GoState::show(){
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
string GoState::get_key(uint16_t row, uint16_t col){
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


bool GoState::valid_pos(uint16_t row, uint16_t col){
	return (row>=0 && row<numRows && col>=0 && col<numColumns);
}

// string to pos
uint16_t GoState::get_row(string pos){
	return (uint16_t)(pos[0] - 'a');
}

uint16_t GoState::get_col(string pos){
	return (uint16_t)(pos[1] - '1');
}

bool GoState::update(GoAction action){
	//cout << "playing " << action.move << endl;
	uint16_t i = 0;
	uint16_t j = 1;
	string pos;
	int key;
	Valtype val;
	while (i < action.move.length()){
		if (action.move[j]=='B' || action.move[j]=='W' || action.move[j]=='.'){
			pos = action.move.substr(i+1, j-i-1).c_str();
			key = get_row(pos) * numColumns + get_col(pos);
			if (action.move[i] == '.'){
				//if (graph[key].first == BLACK)	blackCount--;
				//else if (graph[key].first == WHITE)	whiteCount--;
				graph[key].first = EMPTY;
			}
			else{
				if (action.move[i] == BLACK_STONE){
					//blackCount++;
					graph[key].first = BLACK;
				}
				else if (action.move[i] == WHITE_STONE){
					//whiteCount++;
					graph[key].first = WHITE;
				}
				else{
					return false;
				}
			}
			i = j;
		}
		j++;
	}
	return true;
}

void GoState::revert(GoAction& action){
	for (uint16_t i = 0; i < action.move.length(); ++i){
		if (action.move[i] == BLACK_STONE or action.move[i] == WHITE_STONE){
			action.move[i] = '.';
		}
	}
	update(action);
}

int GoState::next(){
	return 3 - playerJustMoved;
}

bool GoState::connected(uint16_t key0, uint16_t key1, bool blackConnect){
	
	bool visited[numRows * numColumns];
	memset(visited, false, numRows*numColumns);
	stack<uint16_t> keyStack;
	keyStack.push(key0);
	visited[key0] = true;
	while (!keyStack.empty()){
		uint16_t curr = keyStack.top();
		keyStack.pop();

		uint16_t currRow = curr/numRows;
		uint16_t currCol = curr%numRows;

		if ((blackConnect and currRow == key1) or
			(!blackConnect and currCol == key1)){
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

char GoState::check_win(){
	// check if black has won
	for (uint16_t col = 0; col < numColumns; ++col){
		if (graph[col].first == BLACK){
			if (connected(col, numRows-1, true)){
				return BLACK_STONE;
			}
		}
	}


	// check if white has won
	for (uint16_t row = 0; row < numRows; ++row){
		uint16_t key = row * numRows;
		if (graph[key].first == WHITE){
			if (connected(key, numColumns-1, false)){
				return WHITE_STONE;
			}
		}
	}

	return GAME_NOT_OVER;

}

void GoState::get_moves(vector<GoAction>& actions){
	string myStone = "B";
	if (playerJustMoved == 1)
		myStone = "W";

	// store all cells where a stone may be placed
	vector<string> emptyPos;
	vector<string> stonePos;
	vector<string> neutralPos;
	for (uint16_t row = 0; row < numRows; ++row){
		for (uint16_t col = 0; col < numColumns; ++col){
			uint16_t key = row * numColumns + col;
			if (graph[key].first == EMPTY){
				actions.push_back(GoAction(myStone+get_key(row, col)));
			}
		}
	}
}

void GoState::get_moves(vector<GoAction>& actions, bool isMax){

	string myStone = "B";
	if (!isMax)
		myStone = "W";

	// store all cells where a stone may be placed
	vector<string> emptyPos;
	vector<string> stonePos;
	vector<string> neutralPos;
	for (uint16_t row = 0; row < numRows; ++row){
		for (uint16_t col = 0; col < numColumns; ++col){
			uint16_t key = row * numColumns + col;
			if (graph[key].first == EMPTY){
				actions.push_back(GoAction(myStone+get_key(row, col)));
			}
		}
	}
}

GoState& GoState::operator=(GoState& s){
	numRows = s.numRows;
	numColumns = s.numColumns;
	//blackCount = s.blackCount;
	//whiteCount = s.whiteCount;
	playerJustMoved = s.playerJustMoved;
	status = s.status;
	graph = s.graph;
	return *this;
}

char GoState::player1(){
	return BLACK_STONE;
}

char GoState::player2(){
	return WHITE_STONE;
}

char GoState::draw(){
	return DRAW;
}


