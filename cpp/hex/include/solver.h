#ifndef _SOLVER_
#define _SOLVER_

#include <algorithm>

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
	void solve(){
		cout << "solving state..." << endl;
		state_t s = state;
		s.show();
		string myStone = "B";
		bool isMax = true;
		if (s.playerJustMoved == 1){
			myStone = "W";
			isMax = false;
		}

		vector<action_t> actions;
		s.get_moves(actions, myStone);

		srand(time(nullptr));
		random_shuffle(actions.begin(), actions.end());

		int alpha = -100;
		int beta = 100;
		int value = -100;
		action_t bestAction;
		for (auto action : actions){
			s.update(action);
			int negVal = -negamax(s, !isMax, -beta, -alpha);
			if (negVal > value){
				value = negVal;
				bestAction = action;
			}

			alpha = max(alpha, value);
			if (alpha >= beta)
				break;
			action.move[0] = '.';
			s.update(action);
		}

		cout << "best move = " << bestAction.move << endl;

	}
	
	int negamax(state_t s, bool isMax, int alpha, int beta){
		string myStone = (isMax?"B":"W");
		int value = evaluate(s, isMax);

		if (s.status == BLACK_WIN or s.status == WHITE_WIN){
			return value;
		}

		vector<action_t> actions;
		s.get_moves(actions, myStone);
		value = -100;
		for (auto action : actions){
			s.update(action);
			s.status = s.check_win();
			value = max(value, -negamax(s, !isMax,
				-beta, -alpha));
			alpha = max(alpha, value);
			if (alpha >= beta)	break;	// alpha-beta cutoff
			action.move[0] = '.';
			s.update(action);
			s.status = s.check_win();
		}
		return value;
	}
	
	int evaluate(state_t s, bool isMax){
		if (s.status == GAME_NOT_OVER)	return -100;
		int value = 0;
		if (s.status == BLACK_WIN)		value = 1;
		else if (s.status == WHITE_WIN)	value = -1;
		else if (s.status == DRAW)		value = 0;
		if (!isMax)	value *= -1;
		return value;
	}

	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	~Solver(){
		cout << "Solver dies" << endl;
	}
};

#endif
