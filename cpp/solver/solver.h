#ifndef _SOLVER_
#define _SOLVER_

#define MAX_ITERATIONS 		1000
#define DEPTH_LIMIT			10

// #define SHUFFLE_MOVES

#include <algorithm>	// for random shuffle

class Action{
public:
	Action& operator=(const Action& a);
};

class State{
public:
	State& operator=(State& s);
	void show();
	bool update(Action action);
	// void revert(Action& action);
	void get_moves(...);
	char check_win();
	char player1();
	char player2();
	char draw();
	int evaluate(bool isMax);
	bool gameover();
};

template <class state_t, class action_t>
class Solver{
public:
	state_t state;
	action_t bestAction;
	Solver(){
		cout << "Solver says Hi!" << endl;
	}
	void set_state(state_t s){
		state = s;
	}

	// alpha beta negamax (without revert)~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int negamax(state_t s, int alpha, int beta, bool isMax, int depth){
		// // cout << "negmax call " << depth << endl;
		// int value = s.evaluate(isMax);
		// // cout << "value = " << value << endl;
		// if (value != -100 and value != 100){
		// 	return value;
		// }

		if (s.gameover() or depth >= DEPTH_LIMIT){
			return s.evaluate(isMax);
		}

		vector<action_t> actions;
		s.get_moves(actions, isMax);

		#ifdef SHUFFLE_MOVES
			srand(time(nullptr));
			if (depth == 0){
				random_shuffle(actions.begin(), actions.end());
			}
		#endif

		state_t _s = s;
		int value = -100;
		for (auto action : actions){
			s = _s;
			s.update(action);
			// s.virtual_connections(isMax);
			if (depth == 0){
				int neg = -negamax(s, -beta, -alpha, !isMax, depth+1);
				if (neg > value){
					value = neg;
					bestAction = action;
				}
			}
			else{
				value = max(value, -negamax(s, -beta, -alpha, !isMax, depth+1));
			}
			alpha = max(alpha, value);
			if (alpha >= beta)
				break;
		}

		if (depth == 0){
			cout << "best move\t=\t" << bestAction.move << endl;
			cout << "negamax value\t=\t" << value << endl;
		}
		return value;
	}

	// alpha beta negamax (with revert)~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int negamax_r(state_t s, int alpha, int beta, bool isMax, int depth){
		int value = s.evaluate(isMax);
		if (value != -100 and value != 100){
			return value;
		}

		vector<action_t> actions;
		s.get_moves(actions, isMax);

		#ifdef SHUFFLE_MOVES
		srand(time(nullptr));
		if (depth == 0){
			random_shuffle(actions.begin(), actions.end());
		}
		#endif

		for (auto action : actions){
			s.update(action);
			if (depth == 0){
				int neg = -negamax(s, -beta, -alpha, !isMax, depth+1);
				if (neg > value){
					value = neg;
					bestAction = action;
				}
			}
			else{
				value = max(value, -negamax(s, -beta, -alpha, !isMax, depth+1));
			}
			alpha = max(alpha, value);
			if (alpha >= beta)
				break;
			s.revert(action);
		}

		if (depth == 0){
			cout << "best move\t=\t" << bestAction.move << endl;
			cout << "negamax value\t=\t" << value << endl;
		}

		return value;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	action_t best_action(){
		return bestAction;
	}

	~Solver(){
		cout << "Solver dies" << endl;
	}
};



template<class state_t, class action_t>
class MCTNode{
public:
	action_t action;
	MCTNode<state_t, action_t>* parent;
	vector<MCTNode<state_t, action_t>*> children;
	uint16_t wins, visits;

	MCTNode(){
		parent = nullptr;
		wins = 0;
		visits = 0;
	};

	MCTNode(action_t actionPlayed, MCTNode* parentNode){
		action = actionPlayed;
		parent = parentNode;
		wins = 0;
		visits = 0;
	}

	~MCTNode(){}

	void expand(state_t s, bool isMax){
		if (!s.gameover()){
			vector<action_t> actions;
			s.get_moves(actions, isMax);
			for (auto action : actions){
				MCTNode* child(action, this);
				children.push_back(child);
			}
		}
	}

	void update(int result){
		visits++;
		if (result)
			wins++;
	}

	bool is_leaf(){
		return children.size() == 0;
	}

	bool has_parent(){
		return parent != nullptr;
	}

	MCTNode& operator=(const MCTNode& node){
		action = node.action;
		parent = node.parent;
		children = node.children;
		wins = node.wins;
		visits = node.visits;
		return *this;
	}	

private:
};

template<class state_t, class action_t>
class Dragon{
public:
	uint16_t maxIter;

	Dragon(){
		maxIter = MAX_ITERATIONS;
	}

	void search(state_t currState, bool isMax){
		MCTNode<state_t, action_t> root(action_t("none"), nullptr);
		MCTNode<state_t, action_t> node;
		bool player = isMax;

		state_t state;
		for (uint16_t iter = 1; iter <= maxIter; ++iter){

			node = root;
			state = currState;

			int depth = 0;

			// selection
			while (!node.is_leaf()){
				select_policy(node, depth);
				state.update(node.action);
				depth++;
			}

			// expansion
			node.expand(state, isMax);
			select_policy(node, depth);

			isMax = !isMax;

			// simulation
			while (!state.gameover()){
				simulation_policy(state, isMax);
				isMax = !isMax;
			}

			int result = state.evaluate(player);
			
		//	while (n.has_parent()){
		//		n.update(result);
		//		n = n->parent;
		//	}

		}
	}

	void select_policy(MCTNode<state_t, action_t>& node, int depth){
		MCTNode<state_t, action_t> bestChild;
		double bestValue = -1.0;
		double value;
		for (auto child : node.children){
			value = child->wins / child->visits;
			if (value > bestValue){
				bestValue = value;
				bestChild = child;
			}
		}
		node = bestChild;
	}

	void simulation_policy(state_t& state, bool isMax){
		
		srand(time(nullptr));
		
		vector<action_t> actions;
		state.get_moves(actions, isMax);
		action_t action = actions[rand() % actions.size()];
		state.update(action);
		state.switch_turns();
	}

private:
};

#endif
