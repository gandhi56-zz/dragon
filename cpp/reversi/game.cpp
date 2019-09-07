
// game.cpp

#include <iostream>
#include "string.h"
#include "game.h"

using namespace std;

// 'Pos' struct definitions --------------------------------------------------
void Pos::add(Pos pos1){
    row += pos1.row;
    col += pos1.col;
}
// ---------------------------------------------------------------------------
// Initialize 'state0' to the starting position of the game
void initState(char (&state0)[ROWS][COLS]){
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            state0[i][j] = '-';
        }
    }

    state0[ROWS/2]  [COLS/2] = 'R';
    state0[ROWS/2-1][COLS/2-1] = 'R';
    state0[ROWS/2]  [COLS/2-1] = 'G';
    state0[ROWS/2-1][COLS/2] = 'G';
    
    memset(&playable, false, ROWS*COLS);
    
    redScore = 2;
    greenScore = 2;

}

// Display the game board
void displayState(char (&state0)[ROWS][COLS]){

    // Print column indices above the board
    cout << "  ";
    for (int i = 0; i < COLS; ++i){
        cout << i << " ";
    }
    cout << endl;

    // Print the board with row indices on the left
    // Print the values of each cell. If a move
    // can be played at a cell, print '*'
    for (int i = 0; i < ROWS; ++i){
        cout << i << " ";
        for (int j = 0; j < COLS; ++j){
        	if (playable[i*ROWS+j]){
        		cout << "*" << " ";
        	}
        	else{
        		cout << state0[i][j] << " ";
        	}
        }
        cout << endl;
    }
    
    cout << "Green = " << greenScore << " Red = " << redScore << endl;
    
}

// set value at 'selectPos' in 'state0'
void setValue(char (&state0)[ROWS][COLS], Pos selectPos, char val){
    state0[selectPos.row][selectPos.col] = val;
}

// return value at 'selectPos' in 'state0'
char getValue(char (&state0)[ROWS][COLS], Pos selectPos){
    return state0[selectPos.row][selectPos.col];
}

// Make a move. Return if move was successfully made.
void updateState(char (&state0)[ROWS][COLS], Pos selectPos){

    if ( playable[selectPos.row*ROWS+selectPos.col] && posInBounds(selectPos) ){

        int numFlips = 0;
		Pos pos;
		
        // Store 'pos' of each neighbour cell
        int nbrs[] = {selectPos.row-1, selectPos.col-1, 
            selectPos.row-1, selectPos.col, 
            selectPos.row-1, selectPos.col+1, 
            selectPos.row  , selectPos.col-1, 
            selectPos.row  , selectPos.col+1, 
            selectPos.row+1, selectPos.col-1, 
            selectPos.row+1, selectPos.col  , 
            selectPos.row+1, selectPos.col+1};

        // For each neighbour
        for (int i = 0; i < 2*NUM_NBRS; i += 2){

			pos.row = nbrs[i];
			pos.col = nbrs[i+1];

            // check if the neighbour cell holds an opponent coin
            if ( getValue(state0, pos) == play1){

                Pos startPos;
                Pos changePos;

                // Set startPos to the neighbour cell pos
                startPos.row = pos.row;
                startPos.col = pos.col;

                // Set increment values
                changePos.row = startPos.row - selectPos.row;
                changePos.col = startPos.col - selectPos.col;

                numFlips += processMove(state0, startPos, changePos);

            }     
        }
        
        // Finally, place a coin at the intended position
        setValue(state0, selectPos, play0);
        updateScores(state0, numFlips);            
		moveNum++;
		
		cout << play0 << " >> (" << selectPos.row << "," << selectPos.col << ")\n"; 

    }
    else{
        cout << "(" << selectPos.row << "," << selectPos.col << ") is an invalid move.\n";
    }
}

// returns the number of coins flipped. 
unsigned char processMove(char (&state0)[ROWS][COLS], Pos startPos, Pos changePos){
    unsigned char numFlips = 0;

    // Iterate over opponent coins
    while ( getValue(state0, startPos) == play1 ){
        startPos.add(changePos);
    }

    // If the array of opponent coins is surrounded by a 'play0' coin, 
    // flip each opponent coin to a 'play0' coin. Count the number of 
    // flips.
    if ( getValue(state0, startPos) == play0 ){

        numFlips = flipCoins(state0, startPos, changePos);

    }
    return numFlips;
}

// return the number of flips
unsigned char flipCoins(char (&state0)[ROWS][COLS], Pos startPos, Pos changePos){
    unsigned char numFlips = 0;

    // reverse the signs
    changePos.row = -changePos.row;
    changePos.col = -changePos.col;

    startPos.add(changePos);

    // Flip each coin until 'play0' coin is found
    while ( getValue(state0, startPos) == play1 ){
        setValue(state0, startPos, play0);
        startPos.add(changePos);
        numFlips++;
    }

    return numFlips;
}

void updateScores(char (&state0)[ROWS][COLS], unsigned char numFlips){
    cout << "#coins flipped = " << (int)numFlips << endl;
    // Update scores
    switch(play0){
        case 'R':
            redScore++;
            redScore += numFlips;
            greenScore -= numFlips;
            break;
        case 'G':
            greenScore++;    
            redScore -= numFlips;
            greenScore += numFlips;
            break;
        default:
            break;
    }
}

// set the value for 'playable' if
// a valid move exists at that cell
void movesList(char (&state0)[ROWS][COLS]){
	Pos move;

    // Reset #moves found so far
    memset(playable, false, ROWS*COLS);

    cout << "Searching moves for " << play0 << endl;

    // For each cell on the board
    //  If 'self' coin is found,
    //  call generateMoves() on that cell
    for (int r = 0; r < ROWS; ++r){
        for (int c = 0; c < COLS; ++c){
        	move.row = r;
        	move.col = c;

            if (getValue(state0, move) == play0 && !playable[r*ROWS+c]){
            	move.row = r;
            	move.col = c;
                generateMoves(state0, move);
            }
        
        }
    }

}

// For each neighbour to (r, c) check if at least one coin
// can be flipped.
void generateMoves(char (&state0)[ROWS][COLS], Pos selectPos){

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

    // For each neighbour, if an opponent's coin is found
    for (int i = 0; i < 2*NUM_NBRS; i += 2){
    
    	pos.row = nbrs[i];
    	pos.col = nbrs[i+1];
    
        if (getValue(state0, pos) == play1){

            Pos changePos, startPos;

            // Set the increment pos
            changePos.row = pos.row - selectPos.row;
            changePos.col = pos.col - selectPos.col;

            // Store the adjacent cell that is considered.
            startPos.row = pos.row;
            startPos.col = pos.col;
            
            // Increment 'startPos' as long as an opponent's coin is found
            while (getValue(state0, startPos) == play1){
                startPos.add(changePos);
            }

            // If the startPos is within the range of cells and
            // it is an empty cell, then a move can be played on the
            // cell. Increment the number of moves found.
            if (posInBounds(startPos) && getValue(state0, startPos) == '-' ){
                playable[startPos.row*ROWS + startPos.col] = true;
                validActions++;
            }

        }
    }

}

// Store all valid actions from 'stat0' into 'actionsList'
void getActions(char (&state0)[ROWS][COLS], Pos actionsList[MAX_ACTIONS]){
    movesList(state0);
    Pos move;
    int idx = 0;
    for (int i = 0; i < ROWS*COLS; ++i){
        if (playable[i]){
            move.col = i % COLS;
            move.row = i / COLS;

            actionsList[idx].row = i / COLS;
            actionsList[idx].col = i % COLS;
        }
    }
    cout << "called getActions()" << validActions << endl;
}

// Return if 'pos' is valid
bool posInBounds(Pos pos){
    return (pos.row >= 0 && pos.col >= 0 && pos.row < ROWS && pos.col < COLS);
}

void switchTurns(){
	switch(play0){
		case 'R':
			play0 = 'G';
			play1 = 'R';
			break;
		case 'G':
			play0 = 'R';
			play1 = 'G';
	}
}

unsigned char numValidMoves(){
	return validActions;
}

bool terminal(char (&state0)[ROWS][COLS]){
    return validActions == 0;
}
// -----------------------------------------------------------------------------