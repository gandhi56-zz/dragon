#ifndef _SEARCH_

#define _SEARCH_

#include "../include/game.h"

class MCTNode{
public:
	
	string move;
	MCTNode* parentNode;
	vector<MCTNode> childNodes;
	vector<string> untriedMoves;
	State currState;
	int wins;
	int visits;
	Valtype ptm;
	
	MCTNode();
	MCTNode(string _move, MCTNode* parentNode, State state, Valtype currPlayer);
	//void UCT_select(vector<>);
	void add_child(string move, State state);
	void update(int result);
	void print();
	void get_random_move(string& move);

	void expand_node();

	void operator=(MCTNode node);

	void uct_simulation(State state, int maxiter, Valtype currPlayer);
};

#endif	//	_SEARCH_
