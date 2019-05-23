#ifndef _SEARCH_

#define _SEARCH_

#include "../include/game.h"

class MCTNode{
public:
	
	string move;
	MCTNode* parentNode;
	vector<MCTNode*> childNodes;
	vector<string> untriedMoves;
	State state;
	int wins;
	int visits;
	Valtype ptm;
	
	MCTNode();
	MCTNode(string _move, MCTNode* parentNode, State gameState, Valtype currPlayer);
	//void UCT_select(vector<>);
	void add_child(string move);
	void update(int result);
	void print();

	void expand_node();

	void operator=(MCTNode node);

	void uct_simulation(State gameState, int maxiter, Valtype currPlayer);
};

#endif	//	_SEARCH_
