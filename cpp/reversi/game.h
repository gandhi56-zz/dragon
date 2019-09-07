// game.h

#ifndef _GAME_H
#define _GAME_H

#define ROWS 8
#define COLS 8
#define NUM_NBRS 8
#define MAX_ACTIONS 20

// Global variables ----------------------------------------------------------
// Memory Usage (for a standard 8x8 game) = 70 bytes
static bool playable[ROWS*COLS];	// bool array for setting playability
static unsigned char validActions = 0;	// number of valid actions found
static unsigned char moveNum = 0;		// number of moves played
static char play0 = 'G';			// player to move
static char play1 = 'R';			// player on hold
static unsigned short int redScore;
static unsigned short int greenScore;
// ---------------------------------------------------------------------------

struct Pos{
    int row, col;
    void add(Pos pos1);
};

// function declarations -------------------------------------------
void initState(char (&state0)[ROWS][COLS]);
void displayState(char (&state0)[ROWS][COLS]);
void setValue(char (&state0)[ROWS][COLS], Pos selectPos, char val);
char getValue(char (&state0)[ROWS][COLS], Pos selectPos);

// Generate moves
void movesList(char (&state0)[ROWS][COLS]);
void generateMoves(char (&state0)[ROWS][COLS], Pos selectPos);
void getActions(char (&state0)[ROWS][COLS], Pos actionsList[MAX_ACTIONS]);

// Update state
void updateScores(char (&state0)[ROWS][COLS], unsigned char numFlips);
unsigned char flipCoins(char (&state0)[ROWS][COLS], Pos startPos, Pos changePos);
unsigned char processMove(char (&state0)[ROWS][COLS], Pos startPos, Pos changePos);
void updateState(char (&state0)[ROWS][COLS], Pos selectPos);
void switchTurns();
unsigned char numValidMoves();

bool terminal(char (&state0)[ROWS][COLS]);   

bool posInBounds(Pos pos);
// -----------------------------------------------------------------
#endif
