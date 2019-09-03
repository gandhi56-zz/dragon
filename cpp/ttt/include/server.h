#ifndef _SERVER_

#define _SERVER_

#include <ctime>
#include <string.h>
#include <chrono>

//#ifndef _STATE_
	#include "state.h"
//#endif
#include "ttt.h"
#include "../../solver/solver.h"

using namespace std::chrono;
		
class Server{
private:
public:
	uint maxGames;
	
	TTTState state;
	Solver<TTTState, TTTAction> solver;

	Server();
	~Server();
	void set_state(string moves);
	void report_result(int gameId, char result);
	void run();
	void show();
};

#endif // _SERVER_
