// Flip 'em all

// reversi_main.cpp

#include <iostream>
#include "game.h"
#include "player.h"

using namespace std;

// Function declarations
void inputHumanMove(Pos& move);
void inputComputerMove(Player& comp, Pos& move);

// Main Function
int main(){
	// Variables declaration
    enum State{state0, state1, terminal};

    char game[ROWS][COLS];
    Pos move;

    // Initialize current state
    State currState = state0;

    // Initialize game state
    initState(game);

    Player red, green;

    while (currState != terminal){
        
        movesList(game);
        displayState(game);
        if (currState == state0){

            if (numValidMoves() == 0){

                switchTurns();
                currState = state1;

            }
            else{

                if (play0 == 'R'){
                    // red.observeState(game, 'R', 'G');
                    // inputComputerMove(red, move);
                    inputHumanMove(move);

                }
                else if (play0 == 'G'){

                    inputHumanMove(move);

                }
                updateState(game, move);
                switchTurns();
            }

        }
        else if (currState == state1){   
            if (numValidMoves() == 0){
                currState = terminal;
            }
            else{
                currState = state0;
            }
        }

    }
    return 0;
}

void inputHumanMove(Pos& move){
    cout << ">> " ;
    cin >> move.row >> move.col;
}

void inputComputerMove(Player& comp, Pos& move){
    move = comp.chooseAction();
    cout << move.row << move.col << endl;
}
