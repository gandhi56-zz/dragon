#ifndef _SEARCH_

#define _SEARCH_

#include "../include/game.h"

class MCTNode{
public:
	string move;
	State *state;
	vector<MCTNode> childNodes;
	uint wins;
	uint visits;
	bool maxPtm;
	
	MCTNode();
	MCTNode(State *gameState, string _move);
	~MCTNode();
	bool has_children();
	void get_moves(State state, vector<string>& moves, bool isMax);
};

#endif	//	_SEARCH_
