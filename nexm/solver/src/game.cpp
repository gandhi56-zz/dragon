// game.cpp

#include "../include/game.h"

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
	movesCount = 0;
}

State::~State(){

}

void State::set_size(uint rows, uint cols){
	numRows = rows;
	numColumns = cols;
	count[BLACK] = 0;
	count[WHITE] = 0;
	count[NEUTRAL] = 0;
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
			add_nbr(row, col);
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


void State::add_nbr(uint row, uint col){
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

void State::update(string move){
	uint i = 0;
	uint j = 1;
	while (j < move.length()){
		if (move[j]==BLACK || move[j]==WHITE || move[j]==NEUTRAL){
			string pos = move.substr(i+1, j-i-1);

			if (graph[pos].value != EMPTY){
				count[graph[pos].value]--;
			}
			count[move[i]]++;

			graph[pos].value = move[i];
			i = j;
		}
		j++;
	}

	string pos = move.substr(i+1, j-i-1);
	if (graph[pos].value != EMPTY){
		count[graph[pos].value]--;
	}
	count[move[i]]++;
	graph[move.substr(i+1, j-i-1)].value = move[i];

	movesCount++;
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














