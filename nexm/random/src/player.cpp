
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

	cout << "rows=" << rows << endl;
	cout << "cols=" << cols << endl;
}

void Player::random_move(char stone, string& move){
	vector<string> emptyPosVect;
	vector<string> neutralPosVect;
	vector<string> stonePosVect;
	
	for (uint row = 0; row < state.numRows; ++row){
		for (uint col = 0; col < state.numColumns; ++col){
			string key = state.get_key(row, col);
			if (state.graph[key].value == '.'){
				emptyPosVect.push_back(key);
			}
			else if (state.graph[key].value == NEUTRAL){
				neutralPosVect.push_back(key);
			}
			else if (state.graph[key].value == stone){
				stonePosVect.push_back(key);
			}
		}
	}

	srand((uint)time(NULL));
	uint randNum = 0;
	uint index;

	if (emptyPosVect.size() >= 2){
		if (neutralPosVect.size()>=2 && stonePosVect.size()>=1){
			randNum = rand() % 2;
		}
		else{
			randNum = 0;
		}
	}
	else{
		if (neutralPosVect.size()>=2 && stonePosVect.size()>=1){
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

void Player::run(){
	/*
		Run game over server.
	*/

	string data;
	char _data[40];
	char stone = BLACK;
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
			cout << ">";
			random_move(stone, data);
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
			state.show();
			movesCount++;
			stone = (movesCount%2 == 0? BLACK : WHITE);
		}
		else{
			continue;
		}

	}
}

