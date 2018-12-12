#ifndef _SEARCH_

#define _SEARCH_

#include "../include/game.h"

class MCTNode{
private:
	State state;
	string move;
	MCTNode* parent;
	vector<MCTNode*> children;
	int wins;
	int visits;
	uint depth;
	bool maxPtm;	// Black to move, by default
	
public:
	MCTNode();
	MCTNode(const State& gameState, MCTNode* parent, 
		string _move, bool maxTurn);
	~MCTNode();

	// -----------------------------------------------------------
	// getters
	const State& get_state()  const	{	return state;	}
	const string& get_move()  const	{	return move;	}
	bool is_fully_expanded()  const	{return !children.empty();}
	int get_num_visits()	  const	{return visits;	}
	int get_num_wins()		  const	{return wins;}
	uint get_depth()		  const	{return depth;}
	int count_children()	  const	{return	children.size();}
	MCTNode* get_child(int i) const	{return children[i].get();}
	MCTNode* get_parent()	  const	{return parent;}

	bool is_terminal();
	void get_moves(State state, vector<string>& moves, bool isMax);
	// -----------------------------------------------------------


};

#endif	//	_SEARCH_
