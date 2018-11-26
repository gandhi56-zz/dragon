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
}

void State::create_graph(){
	Valtype val(0);
	for (uint i = 0; i < numRows*numColumns; ++i){
		graph.push_back(val);
	}
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
	Valtype value = graph[row*numRows+col];
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
	cout << move << endl;
	uint i = 0;
	uint j = 1;
	string pos;
	int key;
	Valtype val;
	while (i < move.length()){
		if (move[j]=='B' || move[j]=='W' || move[j]=='?' 
				|| move[j]=='.'){
			
			pos = move.substr(i+1, j-i-1).c_str();
			key = get_row(pos) * numRows + get_col(pos);

			if (graph[key] != '.'){
				if (graph[key] == BLACK)		blackCount--;
				else if (graph[key] == WHITE)	whiteCount--;
				else if (graph[key] == NEUTRAL)	neutralCount--;
			}

			if (move[i] == '.'){
				if (graph[key] == BLACK)	blackCount--;
				else if (graph[key] == WHITE)	whiteCount--;
				else if (graph[key] == NEUTRAL)	neutralCount--;
				graph[key] = EMPTY;
			}
			else{
				if (move[i] == 'B'){
					blackCount++;
					graph[key] = BLACK;
				}
				else if (move[i] == 'W'){	
					whiteCount++;
					graph[key] = WHITE;
				}
				else if (move[i] == '?'){	
					neutralCount++;
					graph[key] = NEUTRAL;
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
	/*
	 * Returns the status of the program:
	 * - 'B' if black has won, 'W' if white has won
	 * - '?' if the game has not finished yet
	 * - '#' if the game has ended in a draw
	 */

	// To check if black has won, run dfs from
	// every black stone in row 0. If an iteration
	// reaches the last row of the state, return black
	// has won.

	for (uint col = 0; col < numColumns; ++col){
		if (graph[col] == BLACK){
			// run dfs
		}
	}


    if 		(connected("B0", "B1"))	return BLACK;
	else if (connected("W0", "W1"))	return WHITE;
	
	uint emptyCount = numRows*numColumns - 
		(count[BLACK]+count[WHITE]+count[NEUTRAL]);
		
	if (emptyCount > 1){
		return '?';
	}
	else{
		if (count[NEUTRAL] >= 2){
			return '?';
		}
		else{
			return DRAW;
		}
	}
}

bool State::connected(uint key0, uint endRow, uint endCol){

	// construct a 'visited' array
	bool visited[numRows * numColumns];
	memset(visited, false, numRows*numColumns);

	stack<uint> keyStack;
	keyStack.push(key0);
	visited[key0] = true;
	while (!keyStack.empty()){
		uint curr = keyStack.top();
		keyStack.pop();

		uint currRow = curr/numRows;
		uint currCol = curr%numRows;
		if (currRow == endRow or currCol == endCol){
			return true;
		}

		for (uint adj : graph[curr].adjKeys){	// how to get adjKeys?
			if (graph[adj] == graph[curr]){
				if (!visited[adj]){
					keyStack.push(adj);
					visited[adj] = true;
				}
			}
		}
	}
	return false;
}
