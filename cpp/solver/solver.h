#ifndef _SOLVER_
#define _SOLVER_

#include <algorithm>	// for random shuffle

template <class state_t, class action_t>
class Solver{
public:
	state_t state;
	action_t optAction;
	Solver(){
		cout << "Solver says Hi!" << endl;
	}
	void set_state(state_t s){
		state = s;
	}

	// alpha beta negamax ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	action_t solve(){
		vector<action_t> actions;
		int alpha = -100;
		int beta = 100;
		int value = -100;
		action_t bestAction;
		
		state_t s = state;
		s.get_moves(actions);

#ifdef RANDOM_FIRST_MOVE
		srand(time(nullptr));
		random_shuffle(actions.begin(), actions.end());
#endif
		
		for (auto action : actions){
			s = state;	// copy
			s.update(action);
			s.switch_turns();
			s.status = s.check_win();
			int negVal = -negamax(s, -beta, -alpha);
			if (negVal > value){
				value = negVal;
				bestAction = action;
			}

			alpha = max(alpha, value);
			if (alpha >= beta)
				break;
		}

		cout << "best move = " << bestAction.move << endl;
		return bestAction;
	}
	
	int negamax(state_t s, int alpha, int beta){
		bool isMax = (s.playerJustMoved == 1 ? false : true);
		int value = evaluate(s, isMax);

		if (s.status == s.player1()){
			s.show();
			cout << "isMax = " << isMax << " ";
			cout << "b value = " << value << endl;
			return value;
		}
		else if (s.status == s.player2()){
			s.show();
			cout << "isMax = " << isMax << " ";
			cout << "w value = " << value << endl;
			return value;
		}

		vector<action_t> actions;
		s.get_moves(actions);
		value = -100;

		state_t _s = s;

		for (auto action : actions){
			s = _s;
			s.update(action);
			s.switch_turns();
			s.status = s.check_win();

			//s.show();

			int negVal = -negamax(s, -beta, -alpha);
			if (negVal > value){
				value = negVal;
				optAction = action;
			}
			alpha = max(alpha, value);
			if (alpha >= beta)	break;	// alpha-beta cutoff
			//action.move[0] = '.';
			//s.update(action);
			//s.switch_turns();
			//s.status = s.check_win();
		}

		return value;
	}
	
	int evaluate(state_t s, bool isMax){
		if (s.status == GAME_NOT_OVER)	return -100;
		int value = 0;
		if (s.status == 'x')		value = 1;
		else if (s.status == 'o')	value = -1;
		else								value = 0;
		if (!isMax)	value *= -1;
		s.show();
		cout << "value = " << value << endl;
		return value;
	}

	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	~Solver(){
		cout << "Solver dies" << endl;
	}
};

#endif
