
#include "../include/server.h"

int main(){
	//Server server;
	//server.run();

	// ReversiState state;
	// state.set_size(8, 8);
	// state.create_graph();

	// for (int i = 0; i < 8*8; ++i){
	// 	char c;
	// 	cin >> c;
	// 	if (c == BLACK_COIN or c == WHITE_COIN){
	// 		state.graph[i].first = c;
	// 	}
	// }
	// state.show();

	// state.update(ReversiAction("Ba1"));
	// state.show();

	ReversiState s;
	s.set_size(4,4);
	s.create_graph();
	s.show();

	return 0;
}
