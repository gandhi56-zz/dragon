#ifndef _SERVER_

#define _SERVER_

#include "game.h"
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <chrono>

using namespace std::chrono;
		
struct Player{
	string name;
	int socket;
	char stone;
};

struct Config{
	fstream file;
	map<string, string> data;
	void read();
	void parse_config(string in);
	bool store_data(string key, string& value);	
	void show();
};

class Server{
	private:
		// socket vars
		uint port;
		sockaddr_in servAddr;
		int serverSd;

		// game vars
		uint numRows;
		uint numColumns;

		uint maxGames;
		uint numBlackWin;
		uint numWhiteWin;
		uint numDraw;
		
		struct Player player1;
		struct Player player2;
		struct Config configObj;
		ofstream logFile;

   		void init_mem();
   		void setup_socket();
    	void create_socket();
    	void bind_socket();
    	void receive_ack(int& sock);
		void create_log();
		void set_state(string moves);
		void set_step(string value);
		void import_settings();

		bool receive_move();
		void report_result(char result);
		void report_error();

	public:
		uint step;
		uint movesCounter;
		State state;
		
		Server();
		~Server();
		void run_games();
    	void run();

};

#endif // _SERVER_
