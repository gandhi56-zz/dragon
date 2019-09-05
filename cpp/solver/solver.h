#ifndef _SOLVER_
#define _SOLVER_

// #define SHUFFLE_MOVES

#include <algorithm>	// for random shuffle

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

	// alpha beta negamax ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	action_t solve(){
		return action_t("");
	}
	int negamax(state_t s, int alpha, int beta, bool isMax, int depth){
		//cout << "negmax call " << depth << endl;
		int value = evaluate(s, isMax);
		if (value != -100){
			return value;
		}

		vector<action_t> actions;
		s.get_moves(actions, isMax);

#ifdef SHUFFLE_MOVES
		srand(time(nullptr));
		random_shuffle(actions.begin(), actions.end());
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

		if (depth == 0)
			cout << "best move = " << bestAction.move << endl;

		return value;
	}
	
	int evaluate(state_t s, bool isMax){
		char gameStatus = s.check_win();
		if (gameStatus == GAME_NOT_OVER)	return -100;
		int value;
		if (gameStatus == s.player1())		value = 1;
		else if (gameStatus == s.player2())	value = -1;
		else if (gameStatus == s.draw())		value = 0;
		if (!isMax)	value *= -1;
		return value;	
	
	}

	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	~Solver(){
		cout << "Solver dies" << endl;
	}
};

#endif
