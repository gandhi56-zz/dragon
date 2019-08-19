#ifndef _SERVER_

#define _SERVER_

#include "game.h"
#include <ctime>
#include <string.h>
#include <chrono>

using namespace std::chrono;
		
class Server{
	private:
		uint maxGames;
		uint numBlackWin;
		uint numWhiteWin;
		uint numDraw;

		void set_state(string moves);
		void set_step(string value);
		void import_settings();
		void report_result(int gameId, char result);

	public:
		State state;
		
		Server();
		~Server();
		void run_games();
    	void run();

};

#endif // _SERVER_
