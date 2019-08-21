
#include "../include/server.h"

int main(){
	Server server;

	//server.run();

	server.state.update(Action("Ba1?b2"));

	vector<Action> actions;
	server.state.get_moves(actions);

	for (auto a : actions){
		cout << a.move << endl;
	}

	return 0;
}
