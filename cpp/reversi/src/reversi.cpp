
#include "../include/reversi.h"

ReversiState::ReversiState(){
	numRows = 0;
	numColumns = 0;
	blackCount = 0;
	whiteCount = 0;
	playerJustMoved = 2;
	status = GAME_NOT_OVER;
}

ReversiState::~ReversiState(){

}


ReversiState& ReversiState::operator=(ReversiState& s){
	numRows = s.numRows;
	numColumns = s.numColumns;
	blackCount = s.blackCount;
	whiteCount = s.whiteCount;
	playerJustMoved = s.playerJustMoved;
	graph = s.graph;
	status = s.status;
	return *this;
}

void ReversiState::set_size(uint16_t rows, uint16_t cols){
	numRows = rows;
	numColumns = cols;
	graph.resize(numRows*numColumns);
}

void ReversiState::create_graph(){
	playerJustMoved = 2;
	status = GAME_NOT_OVER;
	for (uint16_t i = 0; i < numRows*numColumns; ++i){
		graph[i] = {EMPTY, false};
	}

	graph[(numRows/2)*numColumns + numColumns/2].first = WHITE_COIN;
    graph[(numRows/2-1)*numColumns + numColumns/2-1].first = WHITE_COIN;
    graph[(numRows/2 )*numColumns + numColumns/2-1].first = BLACK_COIN;
    graph[(numRows/2-1)*numColumns + numColumns/2].first = BLACK_COIN;

	blackCount = 2;
	whiteCount = 2;
}

bool ReversiState::valid_pos(uint16_t key){
	return 0 <= key and key < numRows*numColumns;
}

void ReversiState::switch_turns(){
	playerJustMoved = 3 - playerJustMoved;
}

void ReversiState::show(){
	// Print column indices above the board
    cout << endl << "  ";
    for (int i = 1; i <= numColumns; ++i){
        cout << i << " ";
    }
    cout << endl;

	// Print the board with row indices on the left
    // Print the values of each cell. If a move
    // can be played at a cell, print '*'
    for (int i = 0; i < numRows; ++i){
        cout << (char)('a'+i) << " ";
        for (int j = 0; j < numColumns; ++j){
			if (graph[i*numColumns+j].first == EMPTY and graph[i*numColumns+j].second){
				cout << "* ";
			}
			else{
				cout << graph[i*numColumns+j].first << " ";
			}
        }
        cout << (char)('a'+i) << endl;
    }
	cout << "  ";
    for (int i = 1; i <= numColumns; ++i){
        cout << i << " ";
    }

	cout << endl;
	cout << "# black coins " << blackCount << endl;
	cout << "# white coins " << whiteCount << endl;

	cout << endl << endl;
}

// actions vector ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ReversiState::validate_moves(bool isMax){
	// reset all cells to unplayable
	for (int i = 0; i < numRows * numColumns; ++i){
		graph[i].second = false;
	}

	char coin = BLACK_COIN;
	if (!isMax)
		coin = WHITE_COIN;

	Pos move;
	for (int r = 0; r < numRows; ++r){
		for (int c = 0; c < numColumns; ++c){
			if (graph[r*numColumns+c].first == coin){
				move.row = r;
				move.col = c;
				generate_moves(move, isMax);
			}
		}
	}
}

bool ReversiState::posInBounds(Pos p){
	return p.row >= 0 and p.col >= 0 and p.row < numRows and p.col < numColumns;
}

bool ReversiState::posInBounds(int row, int col){
	return row >= 0 and col >= 0 and row < numRows and col < numColumns;
}

void ReversiState::generate_moves(Pos selectPos, bool isMax){
	Pos pos;
    // store neighbour positions
    int nbrs[] = {selectPos.row-1, selectPos.col-1, 
    			  selectPos.row-1, selectPos.col, 
    			  selectPos.row-1, selectPos.col+1, 
    			  selectPos.row  , selectPos.col-1, 
    			  selectPos.row  , selectPos.col+1, 
    			  selectPos.row+1, selectPos.col-1, 
    			  selectPos.row+1, selectPos.col  , 
    			  selectPos.row+1, selectPos.col+1};

	char oppCoin = BLACK_COIN;
	if (isMax){
		oppCoin = WHITE_COIN;
	}
	int nbrsCount = 8;
	for (int i = 0; i < 2*nbrsCount; i += 2){
		pos.row = nbrs[i];
		pos.col = nbrs[i+1];

		if (graph[pos.row*numColumns+pos.col].first == oppCoin){
			Pos changePos, startPos;
			changePos.row = pos.row - selectPos.row;
			changePos.col = pos.col - selectPos.col;
			startPos.row = pos.row;
			startPos.col = pos.col;
			while (graph[startPos.row*numColumns+startPos.col].first == oppCoin){
				startPos.add(changePos);
			}

			if (posInBounds(startPos) and graph[startPos.row*numColumns+startPos.col].first == EMPTY){
				graph[startPos.row*numColumns+startPos.col].second = true;
			}

		}

	}
}

void ReversiState::get_moves(vector<ReversiAction>& actions, bool isMax){
	actions.clear();
	for (int i = 0; i < numRows*numColumns; ++i){
		graph[i].second = false;
	}

	string coin = "B";
	if (!isMax){
		coin = "W";
	}
	validate_moves(coin=="B");
	for (int i = 0; i < numRows * numColumns; ++i){
		if (graph[i].second){
			actions.push_back(ReversiAction(coin+get_key(i/numColumns, i%numColumns)));
		}
	}

	// cout << "in ReversiState::get_moves()..." << endl;
	// for (auto action : actions){
	// 	cout << action.move << endl;
	// }
}

// state transitions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool ReversiState::update(ReversiAction action){
	char coin = action.move[0];
	char oppcoin = (coin==BLACK_COIN?WHITE_COIN:BLACK_COIN);
	int row = get_row(action.move.substr(1, action.move.size()-1));
	int col = get_col(action.move.substr(1, action.move.size()-1));
	// cout << "playing " << coin << " " << row << " " << col << endl;
	
	validate_moves(coin == BLACK_COIN);

	if (graph[row*numColumns+col].second and posInBounds(row, col)){
		int numFlips = 0;

        // Store 'pos' of each neighbour cell
        int nbrs[] = {row-1, col-1, row-1, col, row-1, col+1, row  , col-1, 
			row  , col+1, row+1, col-1, row+1, col  , row+1, col+1};

		const int nbrsCount = 8;
		// For each neighbour
        for (int i = 0; i < 2*nbrsCount; i += 2){
            // check if the neighbour cell holds an opponent coin
            if ( graph[nbrs[i]*numColumns+nbrs[i+1]].first == oppcoin ){

                Pos startPos;
                Pos changePos;

                // Set startPos to the neighbour cell pos
                startPos.row = nbrs[i];
                startPos.col = nbrs[i+1];

                // Set increment values
                changePos.row = startPos.row - row;
                changePos.col = startPos.col - col;

                numFlips += process_move(startPos, changePos, coin, oppcoin);
            }     
        }

		// Finally, place a coin at the intended position
		graph[row*numColumns+col].first = coin;
		update_scores(numFlips, coin);
	}
	return true;
}

// returns the number of coins flipped. 
int ReversiState::process_move(Pos startPos, Pos changePos, char coin, char oppcoin){
    int numFlips = 0;

    // Iterate over opponent coins
    while ( graph[startPos.row*numColumns+startPos.col].first == oppcoin ){
        startPos.add(changePos);
    }

    // If the array of opponent coins is surrounded by a self coin, 
    // flip each opponent coin to a self coin. Count the number of 
    // flips.
    if (graph[startPos.row*numColumns+startPos.col].first == coin){
        numFlips = flip_coins(startPos, changePos, coin, oppcoin);
    }
    return numFlips;
}

// return the number of flips
int ReversiState::flip_coins(Pos startPos, Pos changePos, char coin, char oppcoin){
    // cout << "changepos " << changePos.row << ", " << changePos.col << endl;
	int numFlips = 0;

    // reverse the signs
    changePos.row = -changePos.row;
    changePos.col = -changePos.col;

    startPos.add(changePos);

    // Flip each coin until 'play0' coin is found
    while (graph[startPos.row*numColumns+startPos.col].first == oppcoin and posInBounds(startPos)){
		// cout << "(" << startPos.row << " " << startPos.col << ")" << endl;
        graph[startPos.row*numColumns+startPos.col].first = coin;
        startPos.add(changePos);
        numFlips++;
    }
    return numFlips;
}

void ReversiState::update_scores(int numFlips, char coin){
    switch(coin){
        case BLACK_COIN:
            blackCount++;
            blackCount += numFlips;
            whiteCount -= numFlips;
            break;
        case WHITE_COIN:
            whiteCount++;    
            blackCount -= numFlips;
            whiteCount += numFlips;
            break;
        default:
            break;
    }
}

// TODO implement action reverter

// void ReversiState::revert(ReversiAction& action){
// 	for (uint16_t i = 0; i < action.move.length(); ++i){
// 		if (action.move[i] == BLACK_COIN or action.move[i] == WHITE_COIN){
// 			action.move[i] = '.';
// 		}
// 	}
// 	update(action);
// }

int ReversiState::next(){
	return 3 - playerJustMoved;
}

// terminal test ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char ReversiState::player1(){
	return BLACK_COIN;
}

char ReversiState::player2(){
	return WHITE_COIN;
}

char ReversiState::draw(){
	return DRAW;
}

int ReversiState::evaluate(bool isMax){
	// assumes there are no moves to play
	int value = blackCount - whiteCount;
	if (!isMax)
		value *= -1;
	// cout << "result = " << result << " isMax = " << isMax << " value = " << value << endl;
	return value;
}

// pos to string
string ReversiState::get_key(uint16_t row, uint16_t col){
	string key;
	key.push_back('a'+row);

	if (col < 9){
		key.push_back('1'+col);
	}
	else{
		key.push_back('1');
		key.push_back('0' + (col+1)%10);
	}
	return key;
}


bool ReversiState::valid_pos(uint16_t row, uint16_t col){
	return (row>=0 && row<numRows && col>=0 && col<numColumns);
}

// string to pos
uint16_t ReversiState::get_row(string pos){
	return (uint16_t)(pos[0] - 'a');
}

uint16_t ReversiState::get_col(string pos){
	return (uint16_t)(pos[1] - '1');
}
