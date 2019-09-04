#ifndef _SOLVER_
#define _SOLVER_

//#define RANDOM_OPEN

#include <algorithm>	// for random shuffle

template <class state_t, class action_t>
class Solver{
public:
	state_t state;
	Solver(){
		cout << "Solver says Hi!" << endl;
	}
	void set_state(state_t s){
		state = s;
	}

	// alpha beta negamax ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	action_t solve(){

		cout << "solving state..." << endl;
		state.show();

		vector<action_t> actions;
		bool isMax = true;
		int alpha = -100;
		int beta = 100;
		int value = -100;
		action_t bestAction;
		state_t s = state;
		s.get_moves(actions, "x");

#ifdef RANDOM_OPEN
		srand(time(nullptr));
		random_shuffle(actions.begin(), actions.end());
#endif
		cout << actions[0].move << endl;
		for (auto action : actions){
			s.update(action);
			int negVal = -negamax(s, -beta, -alpha, !isMax);
			if (negVal > value){
				value = negVal;
				bestAction.move = action.move;
			}

			alpha = max(alpha, value);
			if (alpha >= beta)
				break;

			s.revert(action);
		}

		cout << "best move = " << bestAction.move << endl;
		return bestAction;
	}
	
	int negamax(state_t s, int alpha, int beta, bool isMax){
		int value = evaluate(s, isMax);

		if (value != -100)
			return value;

		vector<action_t> actions;
		s.get_moves(actions, (isMax?"x":"o"));
		value = -100;
		for (auto action : actions){
			s.update(action);
			//s.show();
			value = max(value, -negamax(s, -beta, -alpha, !isMax));
			alpha = max(alpha, value);
			if (alpha >= beta)	break;	// alpha-beta cutoff
			s.revert(action);
		}
		return value;
	}
	
	int evaluate(state_t s, bool isMax){
		char gameStatus = state.check_win();
		if (gameStatus == GAME_NOT_OVER)	return -100;
		int value;
		if (gameStatus == s.player1())		value = 1;
		else if (gameStatus == s.player2())	value = -1;
		else if (gameStatus == '#')		value = 0;
		if (!isMax)	value *= -1;
		return value;	
	
	}

	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	~Solver(){
		cout << "Solver dies" << endl;
	}
};

#endif
