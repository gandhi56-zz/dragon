#include "../include/server.h"


// ~~~~~~~~~~~~~~~~~~Config struct~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Config::read(){
	cout << "Fetching configurations from ../config.txt";
	cout << endl;
	file.open("config.txt");
	string line;
	if (file){
		while (!file.eof()){
			getline(file, line);

			if (line != "")
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
	if (in[0] == '#')	return;
	uint i = 0;
	while (in[i] != '=')	i++;
	if (in[i] == '=')
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
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~Server class~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

	configObj.read();
	import_settings();
	create_log();
	
	state.set_size(numRows, numColumns);
	state.create_graph();

	//cout << "#games = " << maxGames << endl;
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
	maxGames = 1;	// default max number of games
	numRows = 3;
	numColumns = 3;
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
	setsockopt(serverSd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT , 
		&opt, sizeof(opt));
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
	if(moves == "DEFAULT;"){ return;}
	while (i < moves.length()){
		if (state.valid_stone(moves[i])){
			j = i + 1;
			while (moves[j] != ';')	j++;
			cout << "config:playing move "<<moves.substr(i,j-i);
			cout << endl;
			state.update(moves.substr(i, j-i));
			i = j+1;
		}
		else{//if the move string is incorrect
			cout << ("Error parsing the state \n");
			break;
		}
	}
}

void Server::set_step(string value){
	step = value == "BLACK;"? 0 : 1;
}

void Server::create_log(){
	// generate a log file	
	
	string title = configObj.data["log-title"];
	//title = "./log/" + player1.name;
	//title += "-" + player2.name + ".log";
	logFile.open(title);
	cout << title << " created." << endl;
}

void Server::import_settings(){
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
	tmp = "";
	if (configObj.store_data("num-games", tmp)){
		maxGames = atoi(tmp.c_str());
	}
	if (configObj.store_data("set-state", tmp)){
		set_state(tmp);
	}
	if (configObj.store_data("turn", tmp)){
		set_step(tmp);
	}

}

void Server::run_games(){
	for (uint gameNum = 1; gameNum <= maxGames; ++gameNum){
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "Game #" << gameNum << endl;
		state.clear();
		run();
	}
}

void Server::run(){
	/*
		Run a game between two players. The players must be
		connected to this server via sockets.
	*/


	cout << "initial state >>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
	state.show();
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;


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
	
	//confirmation that players recieved game settings	
	read(player1.socket, (char *)&msg, sizeof(msg));
	read(player2.socket, (char *)&msg, sizeof(msg));

	configObj.store_data("set-state", _msg);
	send(player1.socket, (_msg).c_str(), 
		strlen((_msg).c_str()), 0);
    send(player2.socket, (_msg).c_str(), 
    	strlen((_msg).c_str()), 0);
    
	//confirmation that players recieved game state
	read(player1.socket, (char *)&msg, sizeof(msg));
	read(player2.socket, (char *)&msg, sizeof(msg));

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
			
			// start the decision time for player
			auto start = high_resolution_clock::now();

			cout << "S>?" << endl;
			memset(&msg, 0, sizeof(msg));
			
			// record time server takes to receive
			// a response from 'currPlayer'
			read(currPlayer.socket, (char*)&msg, 
				sizeof(msg));
			cout << "S<" << msg << endl;
			_msg = string(msg);
		
			// end the decision timer 
			auto stop =  high_resolution_clock::now();

			// output the time elapsed making the move
			duration<double> elapsed = 
				duration_cast<duration<double>>(stop - start);
			cout << "Time elapsed: " << elapsed.count();
			cout << " seconds" << endl;

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
				read(player1.socket, (char *)&moveRcv,
					sizeof(moveRcv));
				cout << "S>B: " << moveRcv << endl;
				memset(&moveRcv, 0, sizeof(moveRcv));
				read(player2.socket, (char *)&moveRcv, 
					sizeof(moveRcv));
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
				string win = "+";
				string loose = "-";
				send(player1.socket, win.c_str(), 
					strlen(win.c_str()), 0);
				send(player2.socket, loose.c_str(), 
					strlen(loose.c_str()), 0);
				cout << "S>+/-" << endl;
				logFile << result;
			}
			else if (result == WHITE){
				cout << "White wins!" << endl;
				send(player1.socket, "-", 
					strlen(string("-").c_str()), 0);
				send(player2.socket, "+", 
					strlen(string("+").c_str()), 0);
				cout << "S>+/-" << endl;
				logFile << result;
			}
			else{
				cout << "Draw!" << endl;
				send(player1.socket, "#", 
					strlen(string("#").c_str()), 0);
				send(player2.socket, "#", 
					strlen(string("#").c_str()), 0);
				cout << "S>#" << endl;
				logFile << result;
			}
			break;
		}

    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
