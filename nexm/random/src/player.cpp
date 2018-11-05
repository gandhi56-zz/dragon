
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
        if (moves[i]==BLACK || moves[i]==WHITE || moves[i]==NEUTRAL){
            j = i + 1;
            while (moves[j] != ';') j++;
	        gameState.update(moves.substr(i, j-i));
            i = j+1;

        }
		else{
			cout << "Error parsing start state:" << moves << endl;
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
			random_move(myStone, data);
			cout << "sending " << data << endl;
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

void Player::random_move(char stone, string& move){
	vector<string> emptyPosVect;
	vector<string> neutralPosVect;
	vector<string> stonePosVect;
	
	for (uint row = 0; row < gameState.numRows; ++row){
		for (uint col = 0; col < gameState.numColumns; ++col){
			string key = gameState.get_key(row, col);
			if (gameState.graph[key].value == '.'){
				emptyPosVect.push_back(key);
			}
			else if (gameState.graph[key].value == NEUTRAL){
				neutralPosVect.push_back(key);
			}
			else if (gameState.graph[key].value == stone){
				stonePosVect.push_back(key);
			}
		}
	}

	srand((uint)time(NULL));
	uint randNum = 0;
	uint index;

	if (emptyPosVect.size() >= 2){
		if (neutralPosVect.size()>=2&&stonePosVect.size()>=1){
			randNum = rand() % 2;
		}
		else{
			randNum = 0;
		}
	}
	else{
		if (neutralPosVect.size()>=2&&stonePosVect.size()>=1){
			randNum = 1;
		}
	}

	if (randNum == 0){
		index = rand()%emptyPosVect.size();
		string stonePos = emptyPosVect[index];
		emptyPosVect.erase(emptyPosVect.begin() + index);
		string neutralPos = 
			emptyPosVect[rand()%emptyPosVect.size()];

		move = stone + stonePos + "?" + neutralPos;
	}
	else if (randNum == 1){
		index = rand()%neutralPosVect.size();
		string stonePos1 = neutralPosVect[index];
		neutralPosVect.erase(neutralPosVect.begin()+index);
		string stonePos2 = 
			neutralPosVect[rand()%neutralPosVect.size()];
		string neutralPos =
			stonePosVect[rand()%stonePosVect.size()];
		move = stone+stonePos1+stone+stonePos2+"?"+neutralPos;
	}

}

