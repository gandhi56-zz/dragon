/*
	TODO
		- NexExpansionStrategy class methods
		- NexPlayoutStrategy class methods
 */








#include "../include/player.h"
#include <algorithm>	// for random_shuffle

// ###############################################################################
// ###############################################################################
// ###############################################################################
// ##																			##
// ##	State class																##
// ##																			##
// ###############################################################################
// ###############################################################################
// ###############################################################################

Board::Board(){
	numRows = 0;
	numColumns = 0;
	movesCount = 0;
	emptyCount = 0;
	blackCount = 0;
	whiteCount = 0;
	neutralCount = 0;
}

Board::~Board(){

}

void Board::set_size(uint rows, uint cols){
	numRows = rows;
	numColumns = cols;
	graph.resize(numRows*numColumns);
}

void Board::create_graph(){
	for (uint i = 0; i < numRows*numColumns; ++i){
		graph[i].first = EMPTY;
		set_nbrs(graph[i].second, i);
	}
}

bool Board::valid_pos(uint key){
	return 0 <= key and key < numRows*numColumns;
}
void Board::set_nbrs(vector<uint>& nbrs, uint key){
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

uint Board::num_nbrs(uint row, uint col){
	if (row == 0)	return (col == 0 ? 4 : 5);
	else if (row == numRows - 1){	
		return (col == numColumns-1 ? 5 : 4);
	}
	else{
		return (col == 0 || col == numColumns-1 ? 5 : 6);
	}
}

string Board::get_value(uint row, uint col){
	Valtype value = graph[row*numColumns+col].first;
	if (value == BLACK)	return "B";
	else if (value == WHITE)	return "W";
	else if (value == NEUTRAL)	return "?";
	else						return ".";
}

void Board::show(){
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

string Board::get_key(uint row, uint col){
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


bool Board::valid_pos(uint row, uint col){
	return (row>=0 && row<numRows && col>=0 && col<numColumns);
}

uint Board::get_row(string pos){
	return (uint)(pos[0] - 'a');
}

uint Board::get_col(string pos){
	return (uint)(pos[1] - '1');
}

void Board::update(string move){
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

void Board::revert(string move, char stone){
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

char Board::status(){
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

bool Board::connected(uint key0, uint end, bool blackConnect){
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




// ###############################################################################
// ###############################################################################
// ###############################################################################
// ##																			##
// ##	NexExpansionStrategy class												##
// ##																			##
// ###############################################################################
// ###############################################################################
// ###############################################################################

NexExpansionStrategy::NexExpansionStrategy(Board* board)	:
	ExpansionStrategy<Board, NexAction>(board){
}

NexAction* NexExpansionStrategy::generateNext(){
	NexAction* actionObj = new NexAction();
	generateNextMove(actionObj->action);
	return actionObj;
}

void NexExpansionStrategy::generateNextMove(string& move){

}

/*
void NexExpansionStrategy::generate_moves(Board board, bool isMax){
	string myStone = (isMax?"B":"W");

	// store all cells where a stone may be placed
	vector<string> emptyPos;
	vector<string> stonePos;
	vector<string> neutralPos;
	for (uint row = 0; row < board.numRows; ++row){
		for (uint col = 0; col < board.numColumns; ++col){
			uint key = row * board.numColumns + col;
			if (board.graph[key].first == EMPTY){
				emptyPos.push_back(board.get_key(row, col));
			}
			else if((board.graph[key].first==BLACK and isMax) or
				(board.graph[key].first == WHITE and !isMax)){
				stonePos.push_back(board.get_key(row, col));
			}
			else if (board.graph[key].first == NEUTRAL){
				neutralPos.push_back(board.get_key(row, col));
			}
		}
	}

	if (emptyPos.size() >= 2){
	
		for (uint i = 0; i < emptyPos.size(); ++i){
			for (uint j = i+1; j < emptyPos.size(); ++j){
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
				
		for (uint i = 0; i < neutralPos.size(); ++i){
			for (uint j = i + 1; j < neutralPos.size(); ++j){
				for (uint k = 0; k < stonePos.size(); ++k){
					string key = myStone;
					key += neutralPos[i]+myStone+neutralPos[j];
					key += "?" + stonePos[k];
					actions.push_back(NexAction(key));
				}
			}
		}
	}
}

*/



// ###############################################################################
// ###############################################################################
// ###############################################################################
// ##																			##
// ##	Player class															##
// ##																			##
// ###############################################################################
// ###############################################################################
// ###############################################################################


Player::Player(){
	init_vars();
}

Player::Player(char* servIp, int port){
	/*
	 * Constructor: initialize player sockets
	 * and create a game instance once the
	 * necessary game settings are read from
	 * the connected server.
	 */
	
	init_vars();
	attach_socket(servIp, port);
}

Player::~Player(){
	if (socketConnected)
		close(this->_socket.clientSd);
}

void Player::init_vars(){
	movesCount = 0;
	socketConnected = false;
}

void Player::attach_socket(char* servIp, int port){
	// setup client-side socket
	_socket.port = port;
	_socket.host = gethostbyname(servIp);
	_socket.clientSd = socket(AF_INET, SOCK_STREAM, 0);
	bzero((char*)&_socket.sendSockAddr, 
		sizeof(_socket.sendSockAddr));
	_socket.sendSockAddr.sin_family = AF_INET;
	_socket.sendSockAddr.sin_addr.s_addr = 
		inet_addr(servIp);

	// connect to port
	_socket.sendSockAddr.sin_port = htons(port);
	int status = connect(_socket.clientSd, 
		(sockaddr*)&_socket.sendSockAddr, 
		sizeof(_socket.sendSockAddr));
	if (status < 0){
		cout << "Error connecting to _socket!" << endl;
	}
	cout << "Connected to the server!" << endl;
	socketConnected = true;
}

void Player::set_state(string moves){
    uint i = 0;
    uint j = 1;
	if(moves == ";") return;
    while (i < moves.length()){
        if (moves[i]=='B'||moves[i]=='W'||moves[i]=='?'){
            j = i + 1;
            while (moves[j] != ';') j++;
	        board.update(moves.substr(i, j-i));
            i = j+1;
        }
		else{
			cout << "Error parsing start state" << endl;
			break;
		}
    }
}

void Player::read_settings(char* buff, uint& rows, uint& cols){	
	string _buffer = string(buff);
	uint i = 0;
	while (_buffer[i] != 'r')	i++;
	i++;
	if (_buffer[i+1] == '-'){
		rows = stoi(_buffer.substr(i, 1));
	}
	else{
		rows = stoi(_buffer.substr(i, 2));
	}

	i = 0;
	while (_buffer[i] != 'c')	i++;
	i++;
	if (_buffer[i+1] == '-'){
		cols = stoi(_buffer.substr(i, 1));
	}
	else{
		cols = stoi(_buffer.substr(i, 2));
	}

	myStone = _buffer[_buffer.length() - 2];
}

int Player::evaluate(Board board, bool isMax){
	char gameStatus = board.status();
	if (gameStatus == GAME_NOT_OVER)	return -100;
	int value;
	if (gameStatus == BLACK_WIN)		value = 1;
	else if (gameStatus == WHITE_WIN)	value = -1;
	else if (gameStatus == DRAW)		value = 0;
	if (!isMax)	value *= -1;
	return value;
}

// -------------------------------------------------------------------
void Player::run(bool disp){
	/*
		Run game over server.
	*/

	uint numRows;
	uint numColumns;	
	string data;
	char _data[40];
	while (1){
		memset(_data, 0, sizeof(_data));
		read(this->_socket.clientSd, 
			(char*)&_data, sizeof(_data));
		//cout << "rec:" << _data << endl;

		if (!strcmp(_data, "!")){
			exit(1);
		}
		else if (!strcmp(_data, "Move?")){

			// send move here
			data = "123123123123123123";	// #########################

			//cout << "sending " << data.c_str() << endl;
			send(this->_socket.clientSd, data.c_str(), 
				strlen(data.c_str()), 0);
		}
		else if (!strcmp(_data, "+")){
			//cout << "Yahoo, I won!!" << endl;
		}
		else if (!strcmp(_data, "-")){
			//cout << "Ugh, I lost!" << endl;
		}
		else if (!strcmp(_data, "#")){
			//cout << "Draw!" << endl;
		}
		else if(_data[0] == '>'){
			// to update state in memory
			data = string(_data);
			string move = data.substr(1, data.length() - 1);
			board.update(move);
			movesCount++;
			send(this->_socket.clientSd, "ok", 
				strlen("ok"), 0);
			continue;
		}

		else if (_data[0] == '$'){
			// done all games
			break;
		}
		else if (_data[0] == 'r'){
			
			// read game settings
			if (socketConnected){
				cout << "rec:" << _data << endl;
				send(_socket.clientSd, "ok", strlen("ok"), 0);
				
				read_settings(_data, numRows, numColumns);
				
				// create state instance
				board.set_size(numRows, numColumns);
				board.create_graph();

				// set initial position of board
				memset(&_data, 0, sizeof(_data));
				read(_socket.clientSd, (char *)&_data, 
					sizeof(_data));
				set_state(string(_data));
				send(_socket.clientSd, "ok", strlen("ok"), 0);
			}
		}

		else{
			send(_socket.clientSd, "exit", strlen("exit"), 0);
			cout << "breaking" << endl;
			break;
		}

	}
}

