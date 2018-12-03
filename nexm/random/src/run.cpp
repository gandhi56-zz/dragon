#include "../include/player.h"

int main(int argc, char *argv[]){
    if (argc != 2){
        cerr << "Usage: <port>" << endl;
        exit(0);
    }
    
    // fetch port number
    char serverIp[] = "127.0.0.1";
    int port = atoi(argv[1]);
    
    // create player and try connecting to the server
    Player player(serverIp, port);
    player.run(false);
   
	return 0;
}
