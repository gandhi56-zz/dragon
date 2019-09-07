#include "game.h"
#include "player.h"
#include <iostream>
#include <stdlib.h> // for rand()
#include <algorithm> // for max

using namespace std;

// class Player ---------------------------------------------------------------------------
// constructor
Player::Player(){

}

void Player::observeState(char (&state0)[ROWS][COLS], char currPlay, char oppPlay){
    for (int i = 0; i < ROWS; ++i){
        for (int j = 0; j < COLS; ++j){
            state[i][j] = state0[i][j];
        }
    }
    play0 = currPlay;
    play1 = oppPlay;
}

Pos Player::chooseAction(){
    Pos move;
    int val = minimax(state, 0, 1, play0);
    return move;
}

// ----------------------------------------------------------------------------------------
// Run minimax game tree search algorithm
int minimax(char (&state0)[ROWS][COLS], int depth, int depthLimit, char currPlay){
    if (terminal(state0)){
        return evaluateState(state0);
    }
    else if (currPlay == 'G'){
        return maxValue(state0, depth);
    }
    else if (currPlay == 'R'){
        return minValue(state0, depth);
    }
}

// Max node search
int maxValue(char (&state0)[ROWS][COLS], int depth){
    int val = -100;
    Pos actions[MAX_ACTIONS];
    getActions(state0, actions);

    for (int i = 0; i < validActions; ++i){
        cout << actions[i].row << actions[i].col << endl;
    }
    
}

// Min node search
int minValue(char (&state0)[ROWS][COLS], int depth){
    int val = 100;
    Pos actions[MAX_ACTIONS];
    getActions(state0, actions);

    for (int i = 0; i < validActions; ++i){
        cout << actions[i].row << actions[i].col << endl;
    }
    
}

// Evaluate state
int evaluateState(char (&state0)[ROWS][COLS]){
    int redCoins = 0;
    int greenCoins = 0;
    for (int i = 0; i < ROWS; ++i){
        for (int j = 0; j < COLS; ++j){
            if (state0[i][j] == 'R'){
                redScore++;
            }
            else if (state0[i][j] == 'G'){
                greenScore++;
            }
        }
    }
    return greenScore - redScore;
}