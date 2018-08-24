
#include "player.h"

Player::Player(char* servIp, int port){
	this->_socket.port = port;
	this->_socket.host = gethostbyname(servIp);
	this->_socket.clientSd = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char*)&this->_socket.sendSockAddr, 
		sizeof(this->_socket.sendSockAddr));
	this->_socket.sendSockAddr.sin_family = AF_INET;
	this->_socket.sendSockAddr.sin_addr.s_addr = 
		inet_addr(servIp);

	this->_socket.sendSockAddr.sin_port = htons(port);

}

Player::~Player(){
	close(this->_socket.clientSd);
}

void Player::run(){
	string data;
	char _data[64];
	while (1){
		memset(&_data, 0, sizeof(_data));
		recv(this->_socket.clientSd, 
			(char*)&_data, sizeof(_data), 0);
		cout << "rec:" << _data << endl;
		if (!strcmp(_data, "!")){
			break;
		}
		else if (!strcmp(_data, "?")){

			// send move here
			cout << ">";
			getline(cin, data);

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
			continue;
		}
		else{
			continue;
		}

	}
}

int main(int argc, char *argv[]){
	if (argc != 2){
		cerr << "Usage: <port>" << endl;
		exit(0);
	}

	// fetch port number
	char serverIp[] = "127.0.0.1";
	int port = atoi(argv[1]);

	// create player and try connecting to the server
	Player player(serverIp, port);

	player.status = connect(player._socket.clientSd, 
		(sockaddr*)&player._socket.sendSockAddr, 
		sizeof(player._socket.sendSockAddr));

	if (player.status < 0){
		cout << "Error connecting to _socket!" << endl;
	}
	cout << "Connected to the server!" << endl;


	player.run();

	return 0;
}
