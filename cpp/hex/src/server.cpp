#include "../include/server.h"

// ~~~~~~~~~~~~~~~~~~~~Server class~~~~~~~~~~~~~~~~~~~~~~~~~~~
Server::Server(){
	state.set_size(3, 3);
	state.create_graph();
	solver.set_state(state);
	maxGames = 1;
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
		if (moves[i] == 'B' or moves[i] == 'W'){
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
		cout << "> ";
		cin >> cmd;

		if (cmd == "show"){
			state.show();
		}
		else if (cmd == "new"){
			int rows, cols;
			cin >> rows >> cols;
			state.set_size(rows, cols);
			state.create_graph();
			solver.set_state(state);
		}
		else if (cmd == "run"){

			//while (1){
			//	HexAction action = solver.solve();
			//	state.update(action);
			//	state.switch_turns();
			//	state.check_win();
			//	solver.set_state(state);
			//	cout << state.status << endl;
			//	if (state.status != '.')
			//		break;
			//}

		}
		else if (cmd == "play"){
			//cout << "status = " << state.status << endl;
			if (state.status == '.'){
				string move;
				cin >> move;
				if (state.update(HexAction(move))){
					state.switch_turns();
					state.status = state.check_win();
					solver.set_state(state);
				}
			}
			else{
				cout << state.status << " has already won." << endl;
				continue;
			}

			if (state.status == state.player1()){
				cout << state.player1() << " wins!" << endl;
			}
			else if (state.status == state.player2()){
				cout << state.player2() << " wins!" << endl;
			}
		}
		else if (cmd == "search"){
			int neg = solver.negamax(solver.state, -100, 100, state.playerJustMoved==2, 0);
			cout << "negamax value = " << neg << endl;
		}
		else if (cmd == "config"){
		
		}
		else if (cmd == "quit"){
			break;
		}
		else{
			cout << "\t\t\tdragon 1.0 manual" << endl;
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "show\t\t\t\t\tprints the current state of the board" << endl;
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
