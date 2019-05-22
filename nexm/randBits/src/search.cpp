#include "../include/search.h"

/*
	string move;
	MCTNode* parentNode;
	vector<MCTNode> childNodes;
	State state;
	int wins;
	int visits;
*/
	
MCTNode::MCTNode(){
	parentNode = NULL;
}

MCTNode::MCTNode(string _move, MCTNode* parent, State state, Valtype currPlayer){
	move = _move;	// move that reached this state
	parentNode = parent;
	currState = state;
	ptm = currPlayer;
	expand_node();
}

void MCTNode::add_child(string move, State childState){
	// implement binary search here?
	vector<string>::iterator it = 
		find(untriedMoves.begin(), untriedMoves.end(), move);
	if (it != untriedMoves.end()){
		untriedMoves.erase(it);
	}
	else{
		cout << "Error: move " << move << "  not found." << endl;
	}

	childNodes.push_back(
		MCTNode(move, this, childState, (ptm==BLACK?WHITE:BLACK)));
}


void MCTNode::update(int result){
	visits++;
	wins += result;
}

void MCTNode::print(){
	cout << "[M:" << move << " W/V:" << wins << "/" << visits << "]" << endl;
}

void MCTNode::operator=(MCTNode node){
	move = node.move;
	parentNode = node.parentNode;
	currState = node.currState;
	wins = node.wins;
	visits = node.visits;
	ptm = node.ptm;

	for (string move : node.untriedMoves){
		untriedMoves.push_back(move);
	}
	for (auto child : node.childNodes){
		childNodes.push_back(child);
	}
}

bool cmp(MCTNode& node1, MCTNode& node2){
	// TODO test various child selection policies
	return node1.wins/node2.visits > node2.wins/node2.visits;
}

void MCTNode::get_random_move(string& move){
	//TODO
}

void MCTNode::uct_simulation(State state, int maxiter, Valtype currPlayer){
	MCTNode rootNode("None", NULL, state, ptm);
	MCTNode node;
	State currState;

	for (int iter = 1; iter <= maxiter; ++iter){
		node = rootNode;
		currState = state;

		// Selection --------------------------------------------------------
		sort(node.childNodes.begin(), node.childNodes.end(), cmp);
		while (node.untriedMoves.size() == 0 and node.childMoves.size() != 0){
			node = node.childNodes[0];
			currState.update(node.move);
		}
		// ------------------------------------------------------------------
		
		// Expansion---------------------------------------------------------
		if (node.untriedMoves.size() != 0){
			string move;
			get_random_move(move);
			currState.update(move);
			node.add_child(move, currState);
			node = node.childNodes.back();
		}
		// ------------------------------------------------------------------
	
		// Rollout-----------------------------------------------------------
		//while not leaf node
		//	play random simulation
		// ------------------------------------------------------------------
		
		// Backpropagation---------------------------------------------------
		//while node is not root
		//	update node stats
		// ------------------------------------------------------------------
	}
}

void MCTNode::expand_node(){
	string myStone = "B";
	if (ptm == WHITE)	myStone = "W";

	// store all cells where a stone may be placed
	vector<string> emptyPos;
	vector<string> stonePos;
	vector<string> neutralPos;
	for (uint row = 0; row < currState.numRows; ++row){
		for (uint col = 0; col < currState.numColumns; ++col){
			uint key = row * currState.numColumns + col;
			if (currState.graph[key].first == EMPTY){
				emptyPos.push_back(currState.get_key(row, col));
			}
			else if((currState.graph[key].first==BLACK and ptm==BLACK) or
				(currState.graph[key].first == WHITE and ptm==WHITE)){
				stonePos.push_back(currState.get_key(row, col));
			}
			else if (currState.graph[key].first == NEUTRAL){
				neutralPos.push_back(currState.get_key(row, col));
			}
		}
	}


	if (emptyPos.size() >= 2){
	
		for (uint i = 0; i < emptyPos.size(); ++i){
			for (uint j = i+1; j < emptyPos.size(); ++j){
				string key0 = myStone;
				key0 += emptyPos[i] + "?" + emptyPos[j];
				untriedMoves.push_back(key0);

				string key1 = "?";
				key1 += emptyPos[i] + myStone  + emptyPos[j];
				untriedMoves.push_back(key1);
			}
		}
	}

	if (neutralPos.size() >= 2 && stonePos.size() >= 1){
				
		for (uint i = 0; i < neutralPos.size(); ++i){
			for (uint j = i + 1; j < neutralPos.size(); ++j){
				for (uint k = 0; k < stonePos.size(); ++k){
					string key = myStone;
					key += neutralPos[i]+myStone+neutralPos[j];
					key += "?" + stonePos[k];
					untriedMoves.push_back(key);
				}
			}
		}
	}
}

