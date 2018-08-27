
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
	read(_socket.clientSd, (char*)&buffer, sizeof(buffer));
	cout << "rec:" << buffer << endl;
	send(this->_socket.clientSd, "Recieved game details", strlen("Recieved game details"), 0);

	uint numRows;
	uint numColumns;
	read_settings(buffer, numRows, numColumns);

	// set random seed
	srand((uint)time(NULL));
	
	// create state instance
	state.set_size(numRows, numColumns);
	state.create_graph();

	memset(&buffer, 0, sizeof(buffer));
	read(this->_socket.clientSd, (char *)&buffer, sizeof(buffer));
	this->set_state(string(buffer));
	send(this->_socket.clientSd, "Recieved game state", strlen("Reciev    ed game state"), 0);
}

void Player::set_state(string moves){
    uint i = 0;
    uint j = 1;
	if(moves == "DEFAULT;") return;
    while (i < moves.length()){
        if (moves[i] == BLACK || moves[i] == WHITE || moves[i] == NEUTRAL){
            j = i + 1;
            while (moves[j] != ';') j++;
	        state.update(moves.substr(i, j-i));
            i = j+1;

        }
		else{
			cout << "Error parsing start state\n";
			break;
		}
    }
}


Player::~Player(){
	/*
		Destructor
	*/
	close(this->_socket.clientSd);
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

			moves.push_back(key);
		}
	}

	for (uint i = 0; i < neutralPos.size(); ++i){
		for (uint j = i + 1; j < neutralPos.size(); ++j){
			for (uint k = 0; k < stonePos.size(); ++k){
				string key;
				key.push_back(stone);
				key += neutralPos[i];
				key.push_back(stone);
				key += neutralPos[j] + "?" + stonePos[k];

				moves.push_back(key);
			}
		}
	}

	return moves;
}

string Player::best_move(State state, char stone, uint depth){
	state.show();
	
	// store a vector with all legal moves from state
	vector<string> moves = get_moves(state, stone);

	// initialize minimax values
	int bestValue = (stone == BLACK? -100 : 100);
	int alpha = -100;
	int beta = 100;

	string bestMove = "";
	for (string move : moves){
		state.update(move);

		if (stone == BLACK){
			int value = minimax(state, WHITE, depth+1, alpha, beta);
			/*if(value == 1){
				bestMove = move;
				break;
			}*/
			if (value > bestValue){
				bestValue = value;
				bestMove = move;
			}
		}
		else{
			int value = minimax(state, BLACK, depth+1, alpha, beta);
			/*if(value == -1){
				bestMove = move;
				break;
			}*/
			if (value < bestValue){
				bestValue = value;
				bestMove = move;
			}
		}
		state.revert(move, stone);
	}
	return bestMove;
}

int Player::minimax(State state, char stone, uint depth, int& alpha, int& beta){
	char result = state.status();
	if (result != '?'){
		return evaluate(result);
	}
	else{
		if (stone == BLACK){
			return max_value(state, depth, alpha, beta);
		}
		else if (stone == WHITE){
			return min_value(state, depth, alpha, beta);
		}
	}
	
	return 0;
}

int Player::max_value(State state, uint depth, int& alpha, int& beta){
	int bestValue = -100;
	vector<string> moves = get_moves(state, BLACK);
	for (string move : moves){
		state.update(move);
		bestValue = max(bestValue, minimax(state, WHITE, depth+1, 
						alpha, beta));
		alpha = max(alpha, bestValue);
		
		state.revert(move, BLACK);
		
		if (alpha >= beta)	break;
	}

	return bestValue;
}

int Player::min_value(State state, uint depth, int& alpha, int& beta){
	int bestValue = 100;
	vector<string> moves = get_moves(state, WHITE);
	for (string move : moves){
		state.update(move);
		bestValue = min(bestValue, minimax(state, BLACK, depth+1, 
						alpha, beta));
		beta = min(beta, bestValue);
		state.revert(move, WHITE);
		if (alpha <= beta)	break;
	}

	return bestValue;
}

int Player::evaluate(char result){
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
		read(this->_socket.clientSd, 
			(char*)&_data, sizeof(_data));
		cout << "rec:" << _data << endl;

		if (!strcmp(_data, "!")){
			exit(1);
		}
		else if (!strcmp(_data, "Move?")){

			// send move here
			data = best_move(state, myStone, state.movesCount);
			cout << "sending " << data.c_str() << endl;
			send(this->_socket.clientSd, data.c_str(), 
				strlen(data.c_str()), 0);
		}
		else if (!strcmp(_data, "Result+")){
			cout << "Yahoo, I won!!" << endl;
			break;
		}
		else if (!strcmp(_data, "Result-")){
			cout << "Ugh, I lost!" << endl;
			break;
		}
		else if (!strcmp(_data, "Result#")){
			cout << "Draw!" << endl;
			break;
		}
		else if(_data[0] == '>'){
			// to update state in memory
			data = string(_data);
			string move = data.substr(1, data.length() - 1);
			state.update(move);
			movesCount++;
			send(this->_socket.clientSd, "move recieved", strlen("move recieved"), 0);
			continue;
			
		}
		else{
			printf("breaking\n");
			break;
		}

	}
}

