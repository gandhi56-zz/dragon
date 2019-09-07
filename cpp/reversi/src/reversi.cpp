
#include "../include/reversi.h"

ReversiState::ReversiState(){
	numRows = 0;
	numColumns = 0;
	emptyCount = 0;
	//blackCount = 0;
	//whiteCount = 0;
	playerJustMoved = 2;
	status = '.';
}

ReversiState::~ReversiState(){

}


ReversiState& ReversiState::operator=(ReversiState& s){
	numRows = s.numRows;
	numColumns = s.numColumns;
	//blackCount = s.blackCount;
	//whiteCount = s.whiteCount;
	playerJustMoved = s.playerJustMoved;
	status = s.status;
	graph = s.graph;
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

	graph[(numRows/2)*numColumns + numColumns/2].first = BLACK_STONE;
    graph[(numRows/2-1)*numColumns + numColumns/2-1].first = BLACK_STONE;
    graph[(numRows/2 )*numColumns + numColumns/2-1].first = WHITE_STONE;
    graph[(numRows/2-1)*numColumns + numColumns/2].first = WHITE_STONE;

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
	cout << endl << endl;
}

// actions vector ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ReversiState::validate_moves(bool isMax){
	// reset all cells to unplayable
	for (int i = 0; i < numRows * numColumns; ++i){
		graph[i].second = false;
	}

	char coin = BLACK_STONE;
	if (!isMax)
		coin = WHITE_STONE;

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

	char oppCoin = BLACK_STONE;
	if (isMax){
		oppCoin = WHITE_STONE;
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

void ReversiState::get_moves(vector<ReversiAction>& actions){
	string coin = "B";
	if (playerJustMoved == 1){
		coin = "W";
	}
	validate_moves(coin=="B");
	for (int i = 0; i < numRows * numColumns; ++i){
		if (graph[i].second){
			actions.push_back(ReversiAction(coin+get_key(i/numColumns, i%numColumns)));
		}
	}

	cout << "in ReversiState::get_moves()..." << endl;
	for (auto action : actions){
		cout << action.move << endl;
	}

}

// state transitions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool ReversiState::update(ReversiAction action){

}

// void ReversiState::revert(ReversiAction& action){
// 	for (uint16_t i = 0; i < action.move.length(); ++i){
// 		if (action.move[i] == BLACK_STONE or action.move[i] == WHITE_STONE){
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
	return BLACK_STONE;
}

char ReversiState::player2(){
	return WHITE_STONE;
}

char ReversiState::draw(){
	return DRAW;
}

int ReversiState::evaluate(bool isMax){
	// char result = check_win();
	// int value;
	// switch(result){
	// 	case GAME_NOT_OVER:
	// 		return -100;
	// 	case BLACK_STONE:
	// 		value = 1;
	// 		break;
	// 	case WHITE_STONE:
	// 		value = -1;
	// 		break;
	// 	case DRAW:
	// 		value = 0;
	// };
	// if (!isMax)
	// 	value *= -1;
	// // cout << "result = " << result << " isMax = " << isMax << " value = " << value << endl;
	// return value;
	return 0;
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
