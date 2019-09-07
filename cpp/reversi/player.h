// player.h

#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

class Player{
    public:
        // Member variables -------
        char state[ROWS][COLS];
        char play0;
        char play1;

        // Member functions -------
        Player();
        void observeState(char (&state0)[ROWS][COLS], char currPlay, char oppPlay);            
        Pos chooseAction();
};

// Functions declarations -----------------------------------------------------
int minimax(char (&state0)[ROWS][COLS], int depth, int depthLimit, char currPlay);
int maxValue(char (&state0)[ROWS][COLS], int depth);
int minValue(char (&state0)[ROWS][COLS], int depth);    
int evaluateState(char (&state0)[ROWS][COLS]);
// ----------------------------------------------------------------------------

#endif
