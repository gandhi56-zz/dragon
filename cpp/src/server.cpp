#include "../include/server.h"

// ~~~~~~~~~~~~~~~~~~~~Server class~~~~~~~~~~~~~~~~~~~~~~~~~~~
Server::Server(){
	// initialize state here 
	state.set_size(3, 3);	// default size is 3x3
	state.create_graph();
	maxGames = 1;	// default max number of games
	numBlackWin = 0;
	numWhiteWin = 0;
	numDraw = 0;
	cout << "Game created successfully." << endl;
}

Server::~Server(){
	cout << "Server destroyed" << endl;
}

void Server::set_state(string moves){
	uint i = 0;
	uint j = 1;
	if(moves == ";"){ return;}
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
			cout << "Error parsing the state:" << moves[i] << endl;
			break;
		}
	}
	cout << "State initialized." << endl;
}


void Server::run(){

}

void Server::report_result(int gameId, char result){
	string win = "+";
	string lose = "-";
	
	if (result == BLACK){
		numBlackWin++;
		cout << "*************************************************";
		cout << endl;
		cout << "Game #" << gameId << endl;
		cout << "*\tB\tl\ta\tc\tk\t*" << endl;
		cout << "*************************************************";
		cout << endl;
		cout << "S>+/-" << endl;
	}
	else if (result == WHITE){
		numWhiteWin++;
		cout << "*************************************************";
		cout << endl;
		cout << "*\tW\th\ti\tt\te\t*" << endl;
		cout << "*************************************************";
		cout << endl;
		cout << "S>+/-" << endl;
	}
	else{
		numDraw++;
		cout << "*****************************************" << endl;
		cout << "*\tD\tR\tA\tW\t*" << endl;
		cout << "*****************************************" << endl;
		cout << "S>#" << endl;
	}

}


