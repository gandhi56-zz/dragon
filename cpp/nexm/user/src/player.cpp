
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
	
	init_vars();
	attach_socket(servIp, port);
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
			cout << "Error parsing start state\n";
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
			cout << "?";
			cin >> data;
			cout << "sending " << data << endl;
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
			gameState.update(move);
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

				//cout << "rec:" << _data << endl;
				send(_socket.clientSd, "ok", strlen("ok"), 0);
				
				read_settings(_data, numRows, numColumns);
				
				// create state instance
				gameState.set_size(numRows, numColumns);
				gameState.create_graph();

				// set initial position of gameState
				memset(&_data, 0, sizeof(_data));
				read(_socket.clientSd, (char *)&_data, 
					sizeof(_data));
				set_state(string(_data));
				send(_socket.clientSd, "ok", strlen("ok"), 0);
				//gameState.show();
			}
		}

		else{
			send(_socket.clientSd, "exit", strlen("exit"), 0);
			cout << "breaking" << endl;
			break;
		}

	}
}

