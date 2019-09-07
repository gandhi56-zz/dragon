
#include "../include/server.h"

int main(){
	Server server;
	server.run();

	// MCTNode<HexState, HexAction> root(HexAction("root"), nullptr);
	// MCTNode<HexState, HexAction> s1(HexAction("Ba1"), &root);
	// MCTNode<HexState, HexAction> s2(HexAction("Ba2"), &root);

	// cout << s2.action.move << endl;
	// cout << s2.has_parent() << endl;
	// s2 = root;
	// cout << s2.action.move << endl;
	// cout << s2.has_parent() << endl;

	return 0;
}
