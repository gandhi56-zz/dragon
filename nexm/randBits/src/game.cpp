// game.cpp

#include "../include/game.h"

State::State(){
	numRows = 0;
	numColumns = 0;
	movesCount = 0;
	emptyCount = 0;
	blackCount = 0;
	whiteCount = 0;
	neutralCount = 0;
}

State::~State(){

}

void State::set_size(uint rows, uint cols){
	numRows = rows;
	numColumns = cols;
	graph.resize(numRows*numColumns);
}

void State::create_graph(){
	for (uint i = 0; i < numRows*numColumns; ++i){
		graph[i].first = EMPTY;
		set_nbrs(graph[i].second, i);
	}
}

bool State::valid_pos(uint key){
	return 0 <= key and key < numRows*numColumns;
}
void State::set_nbrs(vector<uint>& nbrs, uint key){
	if (valid_pos(key+numColumns))	nbrs.push_back(key+numColumns);
	if (valid_pos(key-numColumns))	nbrs.push_back(key-numColumns);
	if (valid_pos(key-1) and (key%numColumns > 0))
		nbrs.push_back(key-1);
	if (valid_pos(key+1) and (key%numColumns < numColumns-1))
		nbrs.push_back(key+1);
	if (valid_pos(key-numColumns+1)and(key%numColumns < numColumns-1))
		nbrs.push_back(key-numColumns+1);
	if (valid_pos(key+numColumns-1) and (key%numColumns > 0))
		nbrs.push_back(key+numColumns-1);
}

uint State::num_nbrs(uint row, uint col){
	if (row == 0)	return (col == 0 ? 4 : 5);
	else if (row == numRows - 1){	
		return (col == numColumns-1 ? 5 : 4);
	}
	else{
		return (col == 0 || col == numColumns-1 ? 5 : 6);
	}
}

string State::get_value(uint row, uint col){
	Valtype value = graph[row*numColumns+col].first;
	if (value == BLACK)	return "B";
	else if (value == WHITE)	return "W";
	else if (value == NEUTRAL)	return "?";
	else						return ".";
}

void State::show(){
	cout << endl << " ";

	for (uint col = 1; col <= numColumns; ++col){
		if (col < 10)	cout << "  " << col;
		else			cout << " " << col;
	}
	cout << endl;

	for (uint row = 0; row < numRows; ++row){
		for (uint padRow = 0; padRow < row; ++padRow){
			cout << " ";
		}
		cout << (char)('a'+row) << "\\";
		for (uint col = 0; col < numColumns; ++col){
			cout << "  " << get_value(row, col);
		}
		cout << "  \\" << (char)('a'+row) << endl;
	}
	for (uint row = 0; row < numRows+2; ++row){
		cout << " ";
	}

	for (uint col = 1; col <= numColumns; ++col){
		if (col < 10)	cout << "  " << col;
		else			cout << " " << col;
	}
	cout << endl << endl;
}

string State::get_key(uint row, uint col){
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


bool State::valid_pos(uint row, uint col){
	return (row>=0 && row<numRows && col>=0 && col<numColumns);
}

uint State::get_row(string pos){
	return (uint)(pos[0] - 'a');
}

uint State::get_col(string pos){
	return (uint)(pos[1] - '1');
}

void State::update(string move){
	uint i = 0;
	uint j = 1;
	string pos;
	int key;
	Valtype val;
	while (i < move.length()){
		if (move[j]=='B' || move[j]=='W' || move[j]=='?' 
				|| move[j]=='.'){
			
			pos = move.substr(i+1, j-i-1).c_str();
			key = get_row(pos) * numColumns + get_col(pos);

			if (graph[key].first != EMPTY){
				if (graph[key].first == BLACK)		blackCount--;
				else if (graph[key].first == WHITE)	whiteCount--;
				else if (graph[key].first == NEUTRAL)neutralCount--;
			}

			if (move[i] == '.'){
				if (graph[key].first == BLACK)	blackCount--;
				else if (graph[key].first == WHITE)	whiteCount--;
				else if (graph[key].first == NEUTRAL)neutralCount--;
				graph[key].first = EMPTY;
			}
			else{
				if (move[i] == 'B'){
					blackCount++;
					graph[key].first = BLACK;
				}
				else if (move[i] == 'W'){	
					whiteCount++;
					graph[key].first = WHITE;
				}
				else if (move[i] == '?'){	
					neutralCount++;
					graph[key].first = NEUTRAL;
				}
			}
			i = j;
		}
		j++;
	}


}

void State::revert(string move, char stone){
	uint numStones = 0;
	uint numNeutrals = 0;
	for (uint i = 0; i < move.length(); ++i){
		if (move[i] == stone)			numStones++;
		else if (move[i] == '?')	numNeutrals++;
	}

	if (numNeutrals != 1)	return;

	if (numStones == 1){
		for (uint i = 0; i < move.length(); ++i){
			if (move[i] == stone){
				if (stone == 'B')	blackCount--;
				else				whiteCount--;
				move[i] = '.';
			} 
			
			if (move[i] == '?'){
				neutralCount--;
				move[i] = '.';
			}
		}
	}
	else if (numStones == 2){
		for (uint i = 0; i < move.length(); ++i){
			if (move[i] == stone){
				move[i] = '?';
				if (stone == 'B')	blackCount--;
				else				whiteCount--;
				neutralCount++;	
			}
			else if (move[i] == '?'){
				if (stone == 'B'){
					move[i] = 'B';
					blackCount++;
				}
				else{
					move[i] = 'W';
					whiteCount++;
				}
				neutralCount--;
			}
		}
	}
	update(move);
}

char State::status(){
	// check if black has won
	for (uint col = 0; col < numColumns; ++col){
		if (graph[col].first == BLACK){
			// run dfs
			if (connected(col, numRows-1, true)){
				return BLACK_WIN;
			}
		}
	}


	// check if white has won yet
	for (uint row = 0; row < numRows; ++row){
		uint key = row * numColumns;
		if (graph[key].first == WHITE){
			if (connected(key, numColumns-1, false)){
				return WHITE_WIN;
			}
		}
	}

	uint emptyCount = numRows*numColumns - 
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
}

bool State::connected(uint key0, uint end, bool blackConnect){
	// construct a 'visited' array
	bool visited[numRows * numColumns];
	memset(visited, false, numRows*numColumns);

	stack<uint> keyStack;
	keyStack.push(key0);
	visited[key0] = true;
	while (!keyStack.empty()){
		uint curr = keyStack.top();
		keyStack.pop();


		uint currCol = curr%numColumns;
		uint currRow = (curr - currCol) / numColumns;
		
		if ((blackConnect and currRow == end) or 
			(!blackConnect and currCol == end)){
			return true;
		}

		for (uint adj : graph[curr].second){
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




