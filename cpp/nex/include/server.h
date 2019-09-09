#ifndef _SERVER_

#define _SERVER_

#include <ctime>
#include <chrono>
#include <ratio>
#include "nex.h"

using namespace std::chrono;
		
class Server{
private:
public:
	uint maxGames;
	uint numBlackWin;
	uint numWhiteWin;
	uint numDraw;
	
	NexState state;
	Solver<NexState, NexAction> solver;

	Server();
	~Server();
	void set_state(string moves);
	void report_result(int gameId, char result);
	void run();
	void show();
};

#endif // _SERVER_
