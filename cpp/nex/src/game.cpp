// game.cpp

#include "../include/game.h"

// Action class methods ------------------------------------
// ---------------------------------------------------------

// Cell class methods --------------------------------------
Cell::Cell(){
	key = "";
	value = EMPTY;
}

Cell::Cell(string _key){
	key = _key;
	value = EMPTY;
}

// ---------------------------------------------------------


State::State(){
	numRows = 0;
	numColumns = 0;
	count[BLACK] = 0;
	count[WHITE] = 0;
	count[NEUTRAL] = 0;
	playerJustMoved = 2;
}

State::~State(){

}

void State::reset(){
	count[BLACK] = 0;
	count[WHITE] = 0;
	count[NEUTRAL] = 0;
	playerJustMoved = 2;
	for (auto it = graph.begin(); it != graph.end(); ++it){
		it->second.value = EMPTY;
	}
}

int State::next(){
	return 3 - playerJustMoved;
}

void State::set_size(uint rows, uint cols){
	numRows = rows;
	numColumns = cols;
	reset();
	create_graph();
}

void State::create_graph(){
	/*
		Construct a graph corresponding to the board.
		Assumes numRows and numColumns are assigned.
	*/

    string winStones[] = {"B0", "B1", "W0", "W1"};
	for (string stone : winStones){
		graph[stone] = Cell(stone);
		graph[stone].value = stone[0];
	}

	// add interior cells to 'state' and their neighbours.
	for (uint row = 0; row < numRows; ++row){
		for (uint col = 0; col < numColumns; ++col){
			string key = get_key(row, col);
			graph[key] = Cell(key);
			add_nbrs(row, col);
		}
	}

	// connect each edge cell to its respective win 
	// determining cells.
	for (uint row = 0; row < numRows; ++row){
		graph[get_key(row, 0)].adjKeys.push_back("W0");
		graph["W0"].adjKeys.push_back(get_key(row, 0));
		graph[get_key(row, numColumns-1)].adjKeys.push_back("W1");
		graph["W1"].adjKeys.push_back(get_key(row, numColumns-1));
	}
	for (uint col = 0; col < numColumns; ++col){
		graph[get_key(0, col)].adjKeys.push_back("B0");
		graph["B0"].adjKeys.push_back(get_key(0, col));
		graph[get_key(numRows-1, col)].adjKeys.push_back("B1");
		graph["B1"].adjKeys.push_back(get_key(numRows-1, col));
	}
}


void State::add_nbrs(uint row, uint col){
	/*
		add neighbours to the cell positioned at 'row' 
		and 'col'.	
	*/

	if (valid_pos(row+1, col))
		graph[get_key(row,col)].
			adjKeys.push_back(get_key(row+1,col));

    if (valid_pos(row-1, col))
    	graph[get_key(row,col)].
			adjKeys.push_back(get_key(row-1,col));

    if (valid_pos(row, col-1))
    	graph[get_key(row,col)].
			adjKeys.push_back(get_key(row,col-1));

    if (valid_pos(row, col+1))
        graph[get_key(row,col)].
			adjKeys.push_back(get_key(row,col+1));

    if (valid_pos(row-1, col+1))
        graph[get_key(row,col)].
			adjKeys.push_back(get_key(row-1,col+1));

    if (valid_pos(row+1, col-1))
        graph[get_key(row,col)].
			adjKeys.push_back(get_key(row+1,col-1));
}

uint State::num_nbrs(uint row, uint col){
	/*
		return the number of neighbouring cells to the 
		cell at 'row' and 'col'.
	*/
	if (row == 0)	return (col == 0 ? 4 : 5);
	else if (row == numRows - 1){	
		return (col == numColumns-1 ? 5 : 4);
	}
	else{
		return (col == 0 || col == numColumns-1 ? 5 : 6);
	}
}

void State::show(){
	/*
		print board to standard output.
	*/
	cout << endl;
	cout << " ";
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
			cout << "  " << graph[get_key(row, col)].value;
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
	/*
		return key corresponding the cell at 'row' and 
		'col'.
	*/
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

uint State::get_row(string key){
	return key[0] - 'a';
}

uint State::get_col(string key){
	return stoi(key.substr(1, key.length() - 1))-1;
}

bool State::valid_pos(uint row, uint col){
	return (row>=0 && row<numRows && col>=0 && col<numColumns);
}

void State::update(Action a){
	uint i = 0;
	uint j = 1;
	while (j < a.move.length()){
		if (a.move[j]==BLACK or a.move[j]==WHITE or a.move[j]==NEUTRAL){
			string pos = a.move.substr(i+1, j-i-1);

			if (graph[pos].value != EMPTY){
				count[graph[pos].value]--;
			}
			count[a.move[i]]++;

			graph[pos].value = a.move[i];
			i = j;
		}
		j++;
	}

	string pos = a.move.substr(i+1, j-i-1);
	if (graph[pos].value != EMPTY){
		count[graph[pos].value]--;
	}
	count[a.move[i]]++;
	graph[a.move.substr(i+1, j-i-1)].value = a.move[i];

}

char State::status(){
	/*
		Returns the stone corresponding to the winner if
		the game is over. If the game draws, '#' is returned.

		If a move can still be played, '?' is returned.
	*/

    if 		(connected("B0", "B1"))	return BLACK;
	else if (connected("W0", "W1"))	return WHITE;
	else{
		uint emptyCount = numRows*numColumns - count[BLACK]
			- count[WHITE] - count[NEUTRAL];
		if (emptyCount <= 1){
			if (count[NEUTRAL] <= 1)		return '#';
			else						return '?';
		}
	}

	return NEUTRAL;
}

bool State::connected(string key0, string key1){
	/*
		Depth-first Search implementation to check if
		cells 'key0' and 'key1' are connected. Returns 
		true if they are, else returns false.
	*/
	map<string, bool> visited;
	map<string, Cell>::iterator it = graph.begin();
	stack<string> keyStack;

	// initialize 'visited' to all false
	while (it != graph.end()){
		visited[it->first] = false;
		it++;
	}

	keyStack.push(key0);
	visited[key0] = true;
	while (!keyStack.empty()){
		string curr = keyStack.top();
		keyStack.pop();
		if (curr == key1){
			return true;
		}

		for (string adj : graph[curr].adjKeys){
			if (graph[adj].value == graph[curr].value){
				if (!visited[adj]){
					keyStack.push(adj);
					visited[adj] = true;
				}
			}
		}
	}
	return false;
}

bool State::valid_pos(string key){
	if (key.length() == 0)	return false;
	return valid_pos(get_row(key), get_col(key));
}

bool State::valid_stone(char stone){
	return ((stone == BLACK) || 
			(stone == WHITE) || (stone == NEUTRAL));
}

bool State::is_valid(Action a, char stone){
	uint i = 0;
	uint count = 0;
	vector<uint> ind;
	while (i < a.move.length()){
		if (valid_stone(a.move[i])){
			ind.push_back(i);
			i++;
			uint j = i + 1;
			while (j < a.move.length() && !valid_stone(a.move[j])){
				j++;
			}

			string pos = a.move.substr(i, j-i);
			if (!valid_pos(pos)){
				return false;
			}

			if (a.move[i-1] == stone)			count++;
			else if (a.move[i-1] == NEUTRAL)	count--;
			else							return false;

			i = j;
		}
		else{
			return false;
		}
	}

	string pos;
	if (count == 0){
		pos = a.move.substr(ind[0]+1, ind[1] - ind[0] - 1);
		if (graph[pos].value != EMPTY){
			return false;
		}

		pos = a.move.substr(ind[1]+1, a.move.size() - ind[1] - 1);
		if (graph[pos].value != EMPTY){
			return false;
		}
	}
	else if (count == 1){
		for (i = 0; i < ind.size(); ++i){
			pos = a.move.substr(ind[0]+1, ind[1]-ind[0]-1);
			char moveStone = a.move[ind[0]];
			char cellStone = graph[pos].value;

			if (moveStone == NEUTRAL && cellStone == stone){
				continue;
			}
			else if (moveStone == stone && cellStone == NEUTRAL){
				continue;
			}
			else{
				return false;
			}
		}
	}
	else{
		return false;
	}
	return true;
}

void State::clear(){
	graph.clear();
	count[BLACK] = 0;
	count[WHITE] = 0;
	count[NEUTRAL] = 0;

	create_graph();
}

void State::get_moves(vector<Action>& actions, string& myStone){

	// TODO change graph to the way solver 2 implements

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
			else if((graph[key].first==BLACK and isMax) or
				(graph[key].first == WHITE and !isMax)){
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

State State::clone(){
	return *this;
}



