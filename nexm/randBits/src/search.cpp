#include "../include/search.h"

MCTNode::MCTNode(){
	parent 	= 	nullptr;
	move 	= 	"start";
	wins	= 	0;
	visits 	= 	0;
	maxPtm	= 	true;
}

MCTNode::MCTNode(MCTNode* parentNode, string _move, bool maxTurn){
	parent = parentNode;
	move = _move;
	wins	= 	0;
	visits 	= 	0;
	maxPtm = maxTurn;
}

MCTNode::~MCTNode(){

}
/*
MCTNode::has_children(){
	
}

MCTNode::evaluate(){
	
}
*/
void MCTNode::get_moves(State state, vector<string>& moves, bool isMax){
	string myStone = (isMax?"B":"W");

	// store all cells where a stone may be placed
	vector<string> emptyPos;
	vector<string> stonePos;
	vector<string> neutralPos;
	for (uint row = 0; row < state.numRows; ++row){
		for (uint col = 0; col < state.numColumns; ++col){
			uint key = row * state.numColumns + col;
			if (state.graph[key].first == EMPTY){
				emptyPos.push_back(state.get_key(row, col));
			}
			else if((state.graph[key].first==BLACK and isMax) or
				(state.graph[key].first == WHITE and !isMax)){
				stonePos.push_back(state.get_key(row, col));
			}
			else if (state.graph[key].first == NEUTRAL){
				neutralPos.push_back(state.get_key(row, col));
			}
		}
	}


	if (emptyPos.size() >= 2){
	
		for (uint i = 0; i < emptyPos.size(); ++i){
			for (uint j = i+1; j < emptyPos.size(); ++j){
				string key0 = myStone;
				key0 += emptyPos[i] + "?" + emptyPos[j];
				moves.push_back(key0);

				string key1 = "?";
				key1 += emptyPos[i] + myStone  + emptyPos[j];
				moves.push_back(key1);
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
					moves.push_back(key);
				}
			}
		}
	}
}

