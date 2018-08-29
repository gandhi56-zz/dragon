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
		uint port;
		sockaddr_in servAddr;
		int serverSd;

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
		
	public:
		uint step;
		State state;
		
		Server();
		~Server();
    	void run();
};

#endif // _SERVER_
