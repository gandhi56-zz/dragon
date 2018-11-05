
#include "../include/player.h"

Player::Player(){
	init_vars();
}

Player::Player(char* servIp, int port){
	/*
		Constructor: initialize player sockets
		and create a game instance once the
		necessary game settings are read from
		the connected server.
	*/
	
	// initialize member variables
	uint numRows;
	uint numColumns;	
	char buffer[40];
	
	init_vars();
	attach_socket(servIp, port);

	// read game settings
	if (socketConnected){
		memset(buffer, 0, sizeof(buffer));
		read(_socket.clientSd, (char*)&buffer, 
			sizeof(buffer));
		cout << "rec:" << buffer << endl;
		send(_socket.clientSd, "ok", strlen("ok"), 0);
		
		read_settings(buffer, numRows, numColumns);
		
		// create state instance
		gameState.set_size(numRows, numColumns);
		gameState.create_graph();

		// set initial position of gameState
		memset(&buffer, 0, sizeof(buffer));
		read(_socket.clientSd, (char *)&buffer, 
			sizeof(buffer));
		set_state(string(buffer));
		send(_socket.clientSd, "ok", strlen("ok"), 0);
	}
}

Player::~Player(){
	/*
		Destructor
	*/
	
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
        if (moves[i]==BLACK||moves[i]==WHITE||
        	moves[i]==NEUTRAL){
            j = i + 1;
            while (moves[j] != ';') j++;
	        gameState.update(moves.substr(i, j-i));
            i = j+1;
        }
		else{
			cout << "Error parsing start state" << endl;
			break;
		}
    }
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

// alpha-beta minimax implementation ------------------------
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

	if (emptyPos.size() >= 2){
	
		for (uint i = 0; i < emptyPos.size(); ++i){
			for (uint j = i+1; j < emptyPos.size(); ++j){
				string key;
				key.push_back(stone);
				key += emptyPos[i] + "?" + emptyPos[j];
	
				moves.push_back(key);
				
				key = "?"+emptyPos[i];
				key.push_back(stone);
				key += emptyPos[j];
				
				moves.push_back(key);
			}
		}
	}

	if (neutralPos.size() >= 2 && stonePos.size() >= 1){
				
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
	}


	return moves;
}

string Player::best_move(State state, char stone, uint depth){

	cout << "initial state>>>>>>>>>>>>>>>>>>>>>>" << endl;
	state.show();
	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;

	// initialize minimax values
	int alpha = -100;
	int beta = 100;
	string bestMove;
	int bestValue;

	if (stone == BLACK){
		bestValue = -100;
		
		// store a vector with all legal moves from state
		vector<string> moves = get_moves(state, BLACK);
		
		for (uint i = 0; i < moves.size(); ++i){
		
			// for every valid legal move from the
			// current state, update state
			state.update(moves[i]);

			// get the minimax value
			int value = minimax(state, WHITE, depth+1, alpha, beta);
			
			if (value >= bestValue){
				bestValue = value;
				bestMove = moves[i];
			}
			alpha = max(alpha, bestValue);
			state.revert(moves[i], BLACK);
		}
	}


	else if (stone == WHITE){
	
		// minimize minimax value
		bestValue = 100;
		
		// store a vector with all legal moves from state
		vector<string> moves = get_moves(state, stone);
		for (uint i = 0; i < moves.size(); ++i){
			state.update(moves[i]);
			int value = minimax(state, BLACK, depth+1, alpha, beta);
			if (value < bestValue){
				bestValue = value;
				bestMove = moves[i];
			}
			beta = min(beta, bestValue);
			state.revert(moves[i], WHITE);
		}


	}

	state.show();

	cout << "alpha=" << alpha << endl;
	cout << "beta=" << beta << endl;
	cout << "value=" << bestValue << endl;
	cout << "bestMove=" << bestMove << endl;
	return bestMove;
}

int Player::minimax(State state, char stone, uint depth, int& alpha, int& beta){

	vector<string> moves = get_moves(state, stone);
	int value = 100;
	
	if (state.connected("B0", "B1")){
		value = 1;
	}
	else if (state.connected("W0", "W1")){
		value = -1;
	}
	else if (moves.size() == 0){
		value = 0;
	}

	
	if (value == 0 || value == 1 || value == -1){
		return value;
	}
	else{
	}

	
	if (stone == BLACK){
		// maximize minimax value
		value = -100;
		for (uint i = 0; i < moves.size(); ++i){	
			state.update(moves[i]);
			value = max(value, minimax(state, WHITE, depth+1, alpha, beta));
			alpha = max(alpha, value);
			if (beta <= alpha){
				break;
			}
			
			state.revert(moves[i], BLACK);
		}
	}
	else if (stone == WHITE){
		// minimize minimax value
		value = 100;
		for (uint i = 0; i < moves.size(); ++i){
			state.update(moves[i]);
			value = min(value, minimax(state, BLACK, depth+1, alpha, beta));
			beta = min(beta, value);
			if (beta <= alpha){
				break;
			}
			state.revert(moves[i], WHITE);
		}
	}
	return value;
}

void Player::solve(State state, char stone){
	int alpha = -100;
	int beta = 100;
	int value = minimax(state, stone, 0, alpha, beta);
	cout << "alpha=" << alpha << endl;
	cout << "beta="  << beta << endl;
	cout << "value=" << value << endl;
}

// -----------------------------------------------------------

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
			data =best_move(gameState, myStone, 0);
			cout << "sending " << data.c_str() << endl;
			send(this->_socket.clientSd, data.c_str(), 
				strlen(data.c_str()), 0);
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
			gameState.update(move);
			movesCount++;
			send(this->_socket.clientSd, "ok", 
				strlen("ok"), 0);
			continue;
			
		}
		else{
			cout << "breaking" << endl;
			break;
		}

	}
}

