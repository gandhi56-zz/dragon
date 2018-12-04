#ifndef _SEARCH_

#define _SEARCH_

#include "../include/game.h"

class MCTNode{
private:
	
	string move;
	MCTNode* parentNode;
	vector<MCTNode> childNodes;
	uint wins;
	uint visits;
	vector<string> untriedMoves;
	bool maxMoved;

	MCTNode(const State& gameState);
	~MCTNode();
	bool has_children()	const	{
		
	}
	void get_moves(State state, vector<string>& moves, bool isMax);
};

#endif	//	_SEARCH_
