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

// Board class methods -------------------------------------
Board::Board(uint numRows, uint numColumns){
	/*
		Construct the game graph. Add stones for each edge,
		representing the color of that edge.
	*/

	cout << "Creating board of size " << numRows << " x ";
	cout << numColumns << endl;
    this->numRows = numRows;
    this->numColumns = numColumns;

    string winStones[] = {"B0", "B1", "W0", "W1"};
	for (string stone : winStones){
		state[stone] = Cell(stone);
		state[stone].value = stone[0];
	}

	// add interior cells to 'state' and their neighbours.
	for (uint row = 0; row < numRows; ++row){
		for (uint col = 0; col < numColumns; ++col){
			string key = get_key(row, col);
			state[key] = Cell(key);
			add_neighbours(row, col);
		}
	}

	// connect each edge cell to its respective win 
	// determining cells.
	for (uint row = 0; row < numRows; ++row){
		state[get_key(row, 0)].adjKeys.push_back("W0");
		state["W0"].adjKeys.push_back(get_key(row, 0));
		state[get_key(row, numColumns-1)].adjKeys.push_back("W1");
		state["W1"].adjKeys.push_back(get_key(row, numColumns-1));
	}
	for (uint col = 0; col < numColumns; ++col){
		state[get_key(0, col)].adjKeys.push_back("B0");
		state["B0"].adjKeys.push_back(get_key(0, col));
		state[get_key(numRows-1, col)].adjKeys.push_back("B1");
		state["B1"].adjKeys.push_back(get_key(numRows-1, col));
	}
}

void Board::add_neighbours(uint row, uint col){
	/*
		add neighbours to the cell positioned at 'row' 
		and 'col'.	
	*/

	if (valid_pos(row+1, col))
		state[get_key(row,col)].
			adjKeys.push_back(get_key(row+1,col));

    if (valid_pos(row-1, col))
    	state[get_key(row,col)].
			adjKeys.push_back(get_key(row-1,col));

    if (valid_pos(row, col-1))
    	state[get_key(row,col)].
			adjKeys.push_back(get_key(row,col-1));

    if (valid_pos(row, col+1))
        state[get_key(row,col)].
			adjKeys.push_back(get_key(row,col+1));

    if (valid_pos(row-1, col+1))
        state[get_key(row,col)].
			adjKeys.push_back(get_key(row-1,col+1));

    if (valid_pos(row+1, col-1))
        state[get_key(row,col)].
			adjKeys.push_back(get_key(row+1,col-1));
}

uint Board::count_neighbours(uint row, uint col){
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

void Board::show(){
	/*
		print board to standard output.
	*/
	printf("\n ");
	add_col_edge();
	printf("\n");

	for (uint row = 0; row < numRows; ++row){
		for (uint padRow = 0; padRow < row; ++padRow){
			printf(" ");
		}
		printf("%c\\", (char)('a'+row));
		for (uint col = 0; col < numColumns; ++col){
			printf("  %c", state[get_key(row, col)].value);
		}
		printf("  \\%c\n", (char)('a'+row));
	}
	for (uint row = 0; row < numRows+2; ++row){
		printf(" ");
	}
	add_col_edge();
	printf("\n\n");
}

void Board::add_col_edge(){
	/*
		print the column labels, used by 'show()'.
	*/

	for (uint col = 1; col <= numColumns; ++col){
		if (col < 10)	printf("  %d", col);
		else			printf(" %d", col);
	}
}

string Board::get_key(uint row, uint col){
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

uint Board::get_row(string key){
	return key[0] - 'a';
}

uint Board::get_col(string key){
	return stoi(key.substr(1, key.length() - 1))-1;
}

bool Board::valid_pos(uint row, uint col){
	return ((col >= 0 && col < numColumns) && 
		(row >= 0 && row < numRows));
}

bool Board::valid_pos_str(string key){
	if (key.length() == 0)	return false;
	return valid_pos(get_row(key), get_col(key));
}

bool Board::valid_stone(char stone){
	return ((stone == BLACK) || 
			(stone == WHITE) || 
			(stone == NEUTRAL));
}

// ---------------------------------------------------------

// Game class methods --------------------------------------

Game::Game(uint numRows, uint numColumns){
    board = new Board(numRows, numColumns);
}

Game::~Game(){
	delete board;
}

bool Game::is_valid(string move, char stone){
	/*
		returns true if 'move' is a valid playable move,
		false otherwise.
	*/
	uint i = 0;
	uint count = 0;
	vector<int> ind;
	while (i < move.size()){
		if (board->valid_stone(move[i])){
			ind.push_back(i);
			i++;
			uint j = i + 1;
			while (j < move.size()){
				if (board->valid_stone(move[j]))
					break;
				j++;
			}

			string pos = move.substr(i, j-i);
			if (!board->valid_pos_str(pos))
				return false;

			if 		(move[i-1] == stone)	count++;
			else if (move[i-1] == NEUTRAL)	count--;
			else						return false;
			i = j;
		}
		else{
			return false;
		}
	}

	string pos;
	if (count == 0){
		// check if the cells are empty
		pos = move.substr(ind[0]+1, ind[1]-ind[0]-1);
		if (board->state[pos].value != EMPTY)
			return false;
			
		pos = move.substr(ind[1]+1, move.size()-ind[1]-1);
		if (board->state[pos].value != EMPTY)
			return false;
	}
	else if (count == 1){
		// check if the type 2 move is denoted correctly
		for (i = 0; i < ind.size(); ++i){
			pos = move.substr(ind[0]+1, ind[1]-ind[0]-1);
			char moveStone = move[ind[0]];
			char cellStone = board->state[pos].value;
			
			if (moveStone == NEUTRAL && cellStone == stone){
				continue;
			}
			else if ((moveStone == stone) 
				&& (cellStone == NEUTRAL)){
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

void Game::update(string move, char stone){
	/*
		updates game state, playing 'move'; 
		assumes 'move' is valid string.
	*/

	uint i = 0;
	uint j = 1;
	while (j < move.length()){
		if (board->valid_stone(move[j])){
			string _move = move.substr(i+1, j-i-1);
			update_cell(_move, move[i]);
			i = j;
		}
		j++;
	}
	update_cell(move.substr(i+1, j-i-1), move[i]);
}

void Game::update_cell(string movePos, char stone){
	/*
		used by void Game::update(string move)
	*/
	cout << "playing "<<stone<<" at "<<movePos<<endl;
	this->board->state[movePos].value = stone;
}

char Game::terminal_test(){
	/*
		Returns the stone corresponding to the winner if
		the game is over. If the game draws, '#' is returned.

		If a move can still be played, '?' is returned.
	*/

    if 		(is_connected("B0", "B1"))	return BLACK;
	else if (is_connected("W0", "W1"))	return WHITE;
	else{
		uint neutralCount = 0;
		uint emptyCount = 0;
        for (uint row = 0; row < board->numRows; ++row){
			for (uint col = 0; col < board->numColumns; ++col){
				string key = board->get_key(row, col);

				if (board->state[key].value == NEUTRAL){
					neutralCount++;
				}
				else if (board->state[key].value == EMPTY){
					emptyCount++;
				}
			}
		}
		if (emptyCount <= 1){
			if (neutralCount <= 1)		return '#';
			else						return '?';
		}
	}

	return NEUTRAL;
}

bool Game::is_connected(string key0, string key1){
	/*
		Depth-first Search implementation to check if
		cells 'key0' and 'key1' are connected. Returns 
		true if they are, else returns false.
	*/
	map<string, bool> visited;
	map<string, Cell>::iterator it = this->board->state.begin();
	stack<string> keyStack;

	// initialize 'visited' to all false
	while (it != this->board->state.end()){
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

		for (string adj : board->state[curr].adjKeys){
			if (board->state[adj].value == 
				board->state[curr].value){
				if (!visited[adj]){
					keyStack.push(adj);
					visited[adj] = true;
				}
			}
		}
	}
	return false;
}
// ---------------------------------------------------------
