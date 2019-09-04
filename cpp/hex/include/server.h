#ifndef _SERVER_

#define _SERVER_

#include <ctime>
#include <string.h>
#include <chrono>
#include "hex.h"
#include "../../solver/solver.h"

using namespace std::chrono;
		
class Server{
private:
public:
	uint maxGames;
	uint numBlackWin;
	uint numWhiteWin;
	uint numDraw;
	
	HexState state;
	Solver<HexState, HexAction> solver;

	Server();
	~Server();
	void set_state(string moves);
	void report_result(int gameId, char result);
	void run();
	void show();
};

#endif // _SERVER_
