#include "../include/server.h"

// ~~~~~~~~~~~~~~~~~~~~Server class~~~~~~~~~~~~~~~~~~~~~~~~~~~
Server::Server(){
	// initialize state here 
	state.set_size(3, 3);	// default size is 3x3
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
			cout << "playing move "<<moves.substr(i,j-i);
			cout << endl;
			state.update(moves.substr(i, j-i));
			i = j+1;
		}
		else{//if the move string is incorrect
			cout << "Error parsing the state:" << moves[i] << endl;
			break;
		}
	}
}


void Server::run(){
	string cmd;
	while (1){
		cout << "? ";
		cin >> cmd;

		if (cmd == "show"){
			show();
		}
		else if (cmd == "new"){
			int rows, cols;
			cin >> rows >> cols;
			state.set_size(rows, cols);
		}
		else if (cmd == "run"){
		
		}
		else if (cmd == "search"){
		
		}
		else if (cmd == "play"){
			string move;
			cin >> move;

			Action a(move);
		}
		else if (cmd == "config"){
		
		}
		else if (cmd == "quit"){
			break;
		}
		else{
			cout << "\t\t\tdragon 1.0 manual" << endl;
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "showboard\t\t\t\tprints the current state of the board" << endl;
			cout << "new <R> <C>\t\t\t\tcreates an empty state of size R X C" << endl;
			cout << "run <N> [-state] [-estimate]\t\truns N games between players" << endl;
			cout << "\t\t\t\t\tshowing the state and estimate if the flags are provided" << endl;
			cout << "search <b/w>[N]\t\t\t\tsearching for the best move in N simulations, 1000 simulations by default" << endl;
			cout << "play <M>\t\t\t\tplays the given move M over the current state" << endl;
			cout << "config <b/w> <AI>\t\tconfigure the player modes" << endl;
			cout << "quit\t\t\t\t\tquit the program" << endl;
		}
	}
	cout << "Terminating dragon." << endl;
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

void Server::show(){
	state.show();
	cout << "Player to move: " << state.next() << endl;
}

