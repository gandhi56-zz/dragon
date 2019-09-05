#ifndef _STATE_
#define _STATE_

class Action{
public:
	Action& operator=(const Action& a);
};

class State{
public:
	State& operator=(State& s);
	void show();
	bool update(Action action);
	void revert(Action& action);
	void get_moves(...);
	char check_win();
	char player1();
	char player2();
	char draw();
};

#endif
