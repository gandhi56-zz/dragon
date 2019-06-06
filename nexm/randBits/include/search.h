#ifndef _SEARCH_

#define _SEARCH_

#include "../include/game.h"
#include <unordered_map>

typedef unordered_map<string, double> umap;

class MCTNode{
public:
	
	string move;
	MCTNode* parentNode;
	umap childValues;
	State state;
	int wins;
	int visits;
	Valtype ptm;
	
	MCTNode();
	MCTNode(string _move, MCTNode* parentNode, State gameState, Valtype currPlayer);
	void update(int result);
	void print();

	void operator=(MCTNode node);
	void expand_node();

	bool is_leaf();
	bool has_parent();
};

#endif	//	_SEARCH_
