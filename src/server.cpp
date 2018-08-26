#include "../include/server.h"

// ~~~~~~~~~~~~~~~~~~Config struct~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Config::read(){
	cout << "Fetching configurations from ../config.txt" << endl;
	file.open("config.txt");
	string line;
	if (file){
		while (!file.eof()){
			file >> line;
			parse_config(line);
		}
	}
	else{
		cout << "config.txt not found" << endl;
		exit(1);
	}
	file.close();
	cout << "Finished reading ../config.txt" << endl;
}

void Config::parse_config(string in){
	uint i = 0;
	while (in[i] != '=')	i++;
	data[in.substr(0,i)] = in.substr(i+1, in.length() - i - 1);
}

bool Config::store_data(string key, string& value){
	map<string,string>::iterator it = data.find(key);
	if (it == data.end()){
		return false;
	}
	else{
		value = it->second;
		return true;
	}
}

void Config::show(){
	map<string,string>::iterator it = data.begin();
	while (it != data.end()){
		cout << it->first << "=" << it->second << endl;
		it++;
	}
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~Server class~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Server::Server(){
	/*
		Initialize the member variables, set up
		server socket. Open a stream-oriented
		socket and keep track of the socket
		descriptor. Bind the socket to its
		local address. Receive client ack from
		player1 and player2. Read game settings
		from config.txt. Generate a log file.
		Create a game if no error is encountered.
	*/

    init_mem();
    create_socket();
    setup_socket();
    bind_socket();
	cout << "Port = " << port << endl;
	cout << "Waiting for players to connect..." << endl;
	receive_ack(player1.socket);
	player1.stone = BLACK;
    cout << "Connected with player 1!" << endl;
	receive_ack(player2.socket);
	player2.stone = WHITE;
    cout << "Connected with player 2!" << endl;

	uint numRows = 0;
	uint numColumns = 0;

	configObj.read();

	// import game settings here
	string tmp;
	if (configObj.store_data("rows", tmp)){
		numRows = atoi(tmp.c_str());
	}
	tmp = "";
	if (configObj.store_data("columns", tmp)){
		numColumns = atoi(tmp.c_str());
	}
	tmp = "";
	if (configObj.store_data("player1-name", tmp)){
		player1.name = tmp;
	}
	tmp = "";
	if (configObj.store_data("player2-name", tmp)){
		player2.name = tmp;
	}

	create_log();

	state.set_size(numRows, numColumns);
	state.create_graph();

	if (configObj.store_data("set-state", tmp)){
		set_state(tmp);
	}

	cout << "Game created successfully." << endl;
}

Server::~Server(){
	close(player1.socket);
	close(player2.socket);
    close(serverSd);
    logFile.close();
	cout << "Server destroyed" << endl;
}

void Server::init_mem(){
	/*
		initialize member variables
	*/
	step = 0;
    port = 16011;
}

void Server::setup_socket(){
    /*
		configure server socket settings
	*/
	bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
	
	cout << "Server socket has been set up." << endl;
}

void Server::create_socket(){ 
    /*
		create socket here
    	communication domain = AF_INET
    	communication type = TCP
	*/
    serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSd < 0){
        cerr << "Error establishing the server socket";
        cerr << endl;
        exit(0);
    }
	int opt=1;	
	setsockopt(serverSd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT , &opt, sizeof(opt));
	cout << "Server socket has been created." << endl;
}

void Server::bind_socket(){
	int bindStatus = bind(serverSd, 
    	(struct sockaddr*) &servAddr, sizeof(servAddr));
    if (bindStatus < 0){
        cerr << "Error binding socket to local";
        cerr << " address" << endl;
        exit(0);
    }

	cout << "Server socket binds successfully." << endl;
	//listen to maximum 2 players at a time;
	if(listen(serverSd, 2) < 0){
		cerr << "No connection received" << endl;
		exit(1);
	}
}

void Server::receive_ack(int& sock){
    // setup a new address to connect with the client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);


    // accept, create a new socket descriptor to handle the
    // new connection with client
    sock = accept(serverSd, 
            (sockaddr*)&newSockAddr, 
            &newSockAddrSize);
    if (sock < 0){
        cerr << "Error accepting request" << endl;
        exit(1);
    }
}

void Server::set_state(string moves){
	uint i = 0;
	uint j = 1;
	while (i < moves.length()){
		if (state.valid_stone(moves[i])){
			j = i + 1;
			while (moves[j] != ';')	j++;
			state.update(moves.substr(i, j-i-1));
			i = j+1;

			step++;
		}
	}
	state.update(moves.substr(i, j-i-1));
}

void Server::create_log(){
	// generate a log file	
	
	string title = configObj.data["log-title"];
	//title = "./log/" + player1.name;
	//title += "-" + player2.name + ".log";
	logFile.open(title);
	cout << title << " created." << endl;
}

void Server::run(){
	/*
		Run a game between two players. The players must be
		connected to this server via sockets.
	*/

    char result;
    string _msg;
    char msg[40];
	struct Player currPlayer;

	// send game settings to each player
	_msg = "r" + configObj.data["rows"] + "-";
	_msg += "c" + configObj.data["columns"] + "-";
	_msg += "B#";

	cout << "S>" << _msg << endl;
	send(player1.socket, _msg.c_str(), (_msg).length(), 0);
    
	_msg[_msg.length() - 2] = 'W';
	cout << "S>" << _msg << endl;
	send(player2.socket, _msg.c_str(), _msg.length(), 0);
	while (true){
		// check if the game terminates yet
		// print board state to standard output
		result = state.status();
		state.show();	
		if (result == '?'){
			// decide the player yet to move,
			// send a move request to that player
			// wait for response
			currPlayer = step%2 == 0? player1:player2;
			send(currPlayer.socket, "Move?", 
				string("Move?").length(), 0);
			
			cout << "S>?" << endl;
			memset(&msg, 0, sizeof(msg));
			
			// record time server takes to receive
			// a response from 'currPlayer'
			read(currPlayer.socket, (char*)&msg, 
				sizeof(msg));
			cout << "S<" << msg << endl;
			_msg = string(msg);
			if (state.is_valid(_msg, currPlayer.stone)){
				// if move is valid, update game state
				// append move to the logfile
				// increment moves count
				state.update(_msg);
				logFile << _msg << ";";
				step++;

				// send move to each player
				send(player1.socket, (">"+_msg).c_str(),
					_msg.length() + 1, 0);
				send(player2.socket, (">"+_msg).c_str(),
					_msg.length() + 1, 0);
				char moveRcv[40];
				memset(&moveRcv, 0, sizeof(moveRcv));
				read(player1.socket, (char *)&moveRcv, sizeof(moveRcv));
				cout << "S>B: " << moveRcv << endl;
				memset(&moveRcv, 0, sizeof(moveRcv));
				read(player2.socket, (char *)&moveRcv, sizeof(moveRcv));
				cout << "S>W: " << moveRcv << endl;
				cout << "S>" << _msg << endl;
			}
			else{
				// report error 
				send(player1.socket, "!", sizeof("!"), 0);
				send(player2.socket, "!", sizeof("!"), 0);
				cout << "S>!" << endl;
				logFile << "!";
				break;
			}
		}
		else{
			if (result == BLACK){
				cout << "Black wins!" << endl;
				string win = "Result+";
				string loose = "Result-";
				send(player1.socket, win.c_str(), strlen(win.c_str()), 0);
				send(player2.socket, loose.c_str(), strlen(loose.c_str()), 0);
				cout << "S>+/-" << endl;
				logFile << result;
			}
			else if (result == WHITE){
				cout << "White wins!" << endl;
				send(player1.socket, "Result-", strlen(string("Result-").c_str()), 0);
				send(player2.socket, "Result+", strlen(string("Result+").c_str()), 0);
				cout << "S>+/-" << endl;
				logFile << result;
			}
			else{
				cout << "Draw!" << endl;
				send(player1.socket, "Result#", strlen(string("Result#").c_str()), 0);
				send(player2.socket, "Result#", strlen(string("Result#").c_str()), 0);
				cout << "S>#" << endl;
				logFile << result;
			}
			break;
		}

    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

