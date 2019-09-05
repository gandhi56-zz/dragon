
#include "../include/nex.h"

NexState::NexState(){
	numRows = 0;
	numColumns = 0;
	playerJustMoved = 2;
	status = '.';
}

NexState::~NexState(){

}

void NexState::set_size(uint16_t rows, uint16_t cols){
	numRows = rows;
	numColumns = cols;
	graph.resize(numRows*numColumns);
}

void NexState::create_graph(){
	for (uint16_t i = 0; i < numRows*numColumns; ++i){
		graph[i].first = EMPTY;
		set_nbrs(graph[i].second, i);
	}
}

bool NexState::valid_pos(uint16_t key){
	return 0 <= key and key < numRows*numColumns;
}
void NexState::set_nbrs(vector<uint16_t>& nbrs, uint16_t key){
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

string NexState::get_value(uint16_t row, uint16_t col){
	Valtype value = graph[row*numColumns+col].first;
	if 		(value == BLACK)	return "B";
	else if (value == WHITE)	return "W";
	else if (value == NEUTRAL)	return "?";
	else						return ".";
}

void NexState::switch_turns(){
	playerJustMoved = 3 - playerJustMoved;
}

void NexState::show(){
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
string NexState::get_key(uint16_t row, uint16_t col){
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


bool NexState::valid_pos(uint16_t row, uint16_t col){
	return (row>=0 && row<numRows && col>=0 && col<numColumns);
}

// string to pos
uint16_t NexState::get_row(string pos){
	return (uint16_t)(pos[0] - 'a');
}

uint16_t NexState::get_col(string pos){
	return (uint16_t)(pos[1] - '1');
}

bool NexState::update(NexAction action){
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

			if (action.move[i] == '.'){
				graph[key].first = EMPTY;
			}
			else{
				if (action.move[i] == 'B'){
					graph[key].first = BLACK;
				}
				else if (action.move[i] == 'W'){
					graph[key].first = WHITE;
				}
				else if (action.move[i] == '?'){
					graph[key].first = NEUTRAL;
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

void NexState::revert(NexAction action){
	uint16_t numStones = 0;
	uint16_t numNeutrals = 0;
	char stone;
	for (auto c : action.move){
		if (c == 'B' or c == 'W'){
			stone = c;
			break;
		}
	}

	for (uint16_t i = 0; i < action.move.length(); ++i){
		if 		(action.move[i] == stone)	numStones++;
		else if (action.move[i] == '?')		numNeutrals++;
	}

	if (numNeutrals != 1)	return;

	if (numStones == 1){
		for (uint16_t i = 0; i < action.move.length(); ++i){
			if (action.move[i] == stone){
				action.move[i] = '.';
			}

			if (action.move[i] == '?'){
				action.move[i] = '.';
			}
		}
	}
	else if (numStones == 2){
		for (uint16_t i = 0; i < action.move.length(); ++i){
			if (action.move[i] == stone){
				action.move[i] = '?';
			}
			else if (action.move[i] == '?'){
				if (stone == 'B'){
					action.move[i] = 'B';
				}
				else{
					action.move[i] = 'W';
				}
			}
		}
	}
	update(action);
}

char NexState::check_win(){
	// check if black has won
	for (uint16_t col = 0; col < numColumns; ++col){
		if (graph[col].first == BLACK){
			if (connected(col, numRows-1, true)){
				return player1();
			}
		}
	}

	for (uint16_t row = 0; row < numRows; ++row){
		uint16_t key = row * numColumns;
		if (graph[key].first == WHITE){
			if (connected(key, numColumns-1, false)){
				return player2();
			}
		}
	}

	// count stones
	uint16_t blackCount = 0;
	uint16_t whiteCount = 0;
	uint16_t neutralCount = 0;
	for (uint16_t row = 0; row < numRows; ++row){
		for (uint16_t col = 0; col < numColumns; ++col){
			if (graph[row*numColumns+col].first == BLACK){
				blackCount++;
			}
			else if (graph[row*numColumns+col].first == WHITE){
				whiteCount++;
			}
			else if (graph[row*numColumns+col].first == NEUTRAL){
				neutralCount++;
			}
		}
	}

	int emptyCount = numRows*numColumns - (blackCount+whiteCount+neutralCount);
	if (emptyCount > 1){
		return GAME_NOT_OVER;
	}
	else{
		if (neutralCount >= 2){
			return GAME_NOT_OVER;
		}
		else{
			return draw();
		}
	}
	return GAME_NOT_OVER;
}

int NexState::next(){
	return 3 - playerJustMoved;
}

bool NexState::connected(uint16_t key0, uint16_t end, bool blackConnect){
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

void NexState::get_moves(vector<NexAction>& actions){

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
				actions.push_back(NexAction(key0));

				string key1 = "?";
				key1 += emptyPos[i] + myStone  + emptyPos[j];
				actions.push_back(NexAction(key1));
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
					actions.push_back(NexAction(key));
				}
			}
		}
	}
}


void NexState::get_moves(vector<NexAction>& actions, bool isMax){

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
				actions.push_back(NexAction(key0));

				string key1 = "?";
				key1 += emptyPos[i] + myStone  + emptyPos[j];
				actions.push_back(NexAction(key1));
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
					actions.push_back(NexAction(key));
				}
			}
		}
	}
}

NexState& NexState::operator=(NexState& s){
	numRows = s.numRows;
	numColumns = s.numColumns;
	playerJustMoved = s.playerJustMoved;
	status = s.status;
	graph = s.graph;
	return *this;
}


char NexState::player1(){
	return 'B';
}

char NexState::player2(){
	return 'W';
}

char NexState::draw(){
	return DRAW;
}
