#ifndef _SOLVER_
#define _SOLVER_

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
		if (value != -100)
			return value;

		vector<action_t> actions;
		s.get_moves(actions, (isMax?"x":"o"));
		value = -100;

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
		if (gameStatus == 'x')		value = 1;
		else if (gameStatus == 'o')	value = -1;
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
