
#include "../include/player.h"

Player::Player(char* servIp, int port){
	/*
		Constructor: initialize player sockets
		and create a game instance once the
		necessary game settings are read from
		the connected server.
	*/


	// setup client-side socket
	this->_socket.port = port;
	this->_socket.host = gethostbyname(servIp);
	this->_socket.clientSd = socket(AF_INET, SOCK_STREAM, 0);
	bzero((char*)&this->_socket.sendSockAddr, 
		sizeof(this->_socket.sendSockAddr));
	this->_socket.sendSockAddr.sin_family = AF_INET;
	this->_socket.sendSockAddr.sin_addr.s_addr = 
		inet_addr(servIp);

	// connect to port
	this->_socket.sendSockAddr.sin_port = htons(port);
	int status = connect(_socket.clientSd, 
		(sockaddr*)&_socket.sendSockAddr, 
		sizeof(_socket.sendSockAddr));
	if (status < 0){
		cout << "Error connecting to _socket!" << endl;
	}
	cout << "Connected to the server!" << endl;

	// receive game settings
	movesCount = 0;

	char buffer[40];
	memset(buffer, 0, sizeof(buffer));
	recv(_socket.clientSd, (char*)&buffer, sizeof(buffer), 0);
	cout << "rec:" << buffer << endl;

	uint numRows;
	uint numColumns;
	read_settings(buffer, numRows, numColumns);

	// set random seed
	srand((uint)time(NULL));
	
	// create state instance
	state.set_size(numRows, numColumns);
	state.create_graph();
}

Player::~Player(){
	/*
		Destructor
	*/
	close(_socket.clientSd);
}

void Player::read_settings(char* buff, uint& rows, uint& cols){	
	/*
		read row and column dimensions from server.
	*/
	
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
	cout << "rows=" << rows << endl;
	cout << "cols=" << cols << endl;
	cout << "stone=" << myStone << endl;
}

// minimax implementation --------------------------------------
vector<string> Player::get_moves(State state, char stone){

	cout << "finding moves for the state:" << endl;
	state.show();

	// store all cells where a stone may be placed
	vector<string> emptyPos;
	vector<string> stonePos;
	vector<string> neutralPos;
	for (uint row = 0; row < state.numRows; ++row){
		for (uint col = 0; col < state.numColumns; ++col){
			string key = state.get_key(row,col);
			if (state.graph[key].value == EMPTY){
				emptyPos.push_back(key);
			}
			else if (state.graph[key].value == stone){
				stonePos.push_back(key);
			}
			else if (state.graph[key].value == NEUTRAL){
				neutralPos.push_back(key);
			}
		}
	}

	vector<string> moves;
	for (uint i = 0; i < emptyPos.size(); ++i){
		for (uint j = i+1; j < emptyPos.size(); ++j){
			string key;
			key.push_back(stone);
			key += emptyPos[i] + "?" + emptyPos[j];

			cout << key << endl;
			moves.push_back(key);
		}
	}

	cout << "---" << endl;

	for (uint i = 0; i < neutralPos.size(); ++i){
		for (uint j = i + 1; j < neutralPos.size(); ++j){
			for (uint k = 0; k < stonePos.size(); ++k){
				string key;
				key.push_back(stone);
				key += neutralPos[i];
				key.push_back(stone);
				key += neutralPos[j] + "?" + stonePos[k];

				cout << key << endl;
				moves.push_back(key);
			}
		}
	}

	cout << "Found " << moves.size() << " valid moves" << endl;
	return moves;
}

string Player::best_move(State state, char stone, uint depth){
	vector<string> moves = get_moves(state, stone);
	int bestValue = (stone == BLACK? -100 : 100);
	string bestMove = "";
	for (string move : moves){
		state.update(move);

		if (stone == BLACK){
			int value = minimax(state, WHITE, depth+1);
			if (value > bestValue){
				bestValue = value;
				bestMove = move;
			}
		}
		else{
			int value = minimax(state, BLACK, depth+1);
			if (value < bestValue){
				bestValue = value;
				bestMove = move;
			}
		}
	}

	return bestMove;
}

int Player::minimax(State state, char stone, uint depth){
	cout << "depth=" << depth << endl;
	char result = state.status();
	if (result != '?'){
		return evaluate(result);
	}
	else{
		if (stone == BLACK){
			return max_value(state, depth);
		}
		else if (stone == WHITE){
			return min_value(state, depth);
		}
	}
	
	return 0;
}

int Player::max_value(State state, uint depth){
	int bestValue = -100;
	vector<string> moves = get_moves(state, BLACK);
	for (string move : moves){
		state.update(move);
		bestValue = max(bestValue, minimax(state, WHITE, depth+1));
	}

	return bestValue;
}

int Player::min_value(State state, uint depth){
	int bestValue = 100;
	vector<string> moves = get_moves(state, WHITE);
	for (string move : moves){
		state.update(move);
		bestValue = min(bestValue, minimax(state, BLACK, depth+1));
	}

	return bestValue;
}

int Player::evaluate(char result){
	cout << "Evaluating state" << endl;
	if (result == BLACK)		return 1;
	else if (result == WHITE)	return -1;
	return 0;
}

// -------------------------------------------------------------

void Player::run(){
	/*
		Run game over server.
	*/

	string data;
	char _data[40];
	while (1){
		memset(_data, 0, sizeof(_data));
		recv(this->_socket.clientSd, 
			(char*)&_data, sizeof(_data), 0);
		cout << "rec:" << _data << endl;
		if (!strcmp(_data, "!")){
			exit(1);
		}
		else if (!strcmp(_data, "?")){

			// send move here


			cout << "curr state" << endl;
			state.show();
			data = best_move(state, myStone, state.movesCount);
			cout << "sending " << data.c_str() << endl;
			send(this->_socket.clientSd, data.c_str(), 
				data.size(), 0);
		}
		else if (!strcmp(_data, "+")){
			cout << "Yahoo, I won!!" << endl;
			break;
		}
		else if (!strcmp(_data, "-")){
			cout << "Ugh, I lost!" << endl;
			break;
		}
		else if (!strcmp(_data, "#")){
			cout << "Draw!" << endl;
			break;
		}
		else if(_data[0] == '>'){
			// to update state in memory
			data = string(_data);
			string move = data.substr(1, data.length() - 1);
			state.update(move);
			movesCount++;
		}
		else{
			continue;
		}

	}
}

