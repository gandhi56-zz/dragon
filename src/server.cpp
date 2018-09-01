#include "../include/server.h"
+#include <chrono>
+
+using namespace std::chrono;
 
 // ~~~~~~~~~~~~~~~~~~Config struct~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 void Config::read(){
 	*/
 	step = 0;
     port = 16011;
-	movesCounter = 0;
 }
 
 void Server::setup_socket(){
 			send(currPlayer.socket, "Move?", 
 				string("Move?").length(), 0);
 			
+			// start the decision time for player
+			auto start = high_resolution_clock::now();
+
 			cout << "S>?" << endl;
 			memset(&msg, 0, sizeof(msg));
 			
 				sizeof(msg));
 			cout << "S<" << msg << endl;
 			_msg = string(msg);
+		
+			// end the decision timer 
+			auto stop =  high_resolution_clock::now();
+
+			// output the time elapsed making the move
+			duration<double> elapsed = duration_cast<duration<double>>(stop - start);
+			cout << "Time elapsed: " << elapsed.count() << " seconds" << endl;
+
 			if (state.is_valid(_msg, currPlayer.stone)){
 				// if move is valid, update game state
 				// append move to the logfile
 				// increment moves count
-
-				movesCounter++;
-
 				state.update(_msg);
 				logFile << _msg << ";";
 				step++;
 				cout << "S>#" << endl;
 				logFile << result;
 			}
-
-			cout << "Moves elapsed=" << movesCounter << endl;
-
 			break;
 		}
 
     }
 }
 
-// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~