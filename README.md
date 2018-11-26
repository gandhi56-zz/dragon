# nexC

Project Overview:
------------------------------------------------------------
nexC is a command-line interface for a connection game, 
called Nex. 

Game description:
------------------------------------------------------------
Nex is a Hex-variant connection game, played between two 
players, namely black and white. Following figure 
presents the board upon which this game is played, 
produced by nexC:

![9x9Nex](./images/9x9nex.png)

Although the standard size for this game is 13 x 13, nexC
allows sizes from 2 x 2 upto 13 x 13 as well as rectangular
sizes. The game is played by placing stones on the board, 
turn-wise. Black plays the first move. There are three 
kinds of stones: black (B), white (W) and neutral (?). 
Players have two kinds of moves to choose from:
	
1. Generate move: place a stone of their colour and a 
	neutral stone into two distinct empty cells. 
	
	For example,


![3x3blank](./images/3x3blank.png)

![3x3genmove](./images/3x3genmove.png)

Black places a **B** on _a1_ and a **?** on _c2_.

2. Transform move: convert two neutral stones on the 
	board into the player's colour and convert one of 
	their coloured stones into a neutral stone. 
	
	For example,

![3x3transform1](./images/3x3transform1.png)

![3x3transform2](./images/3x3transform2.png)

Black converts **?** at _a2_ and _c2_ to **B** and **B** 
at _a1_ to **?**.
			   
The _objective_ of the game is to form a connection between
opposite sides. Black aims to connect the top and bottom
edges with **B** while white aims to connect the left and
right edges with **W**.

![3x3blackwin](./images/3x3blackwin.png)

Black forms a connection from the top edge to the bottom 
edge with the help of the stones on _a2_, _b2_ and _c2_.

The game terminates when either player wins or no legal
moves exist. Legal moves exist if and only if there are at least
two empty cells for the player to play a generate move or
at least one neutral stone to allow a transform move to
be playable.

Communication Protocol
------------------------------------------------------------
The server and the players are expected to follow the
protocol given below; following are the different input
commands a player may receive from the server:

1. "rR-cC#"

	* Server sends the game settings (format may be extended 
	later). R denotes the number of rows of the board and
	C denotes the number of columns.

2. "!"
	* Due to an unexpected error caused by a player or the
	server, the player must exit with status 1.

3. "?"
	* Server expects the player to send a move in the following
	valid format and type; **"#RC#RC(#RC)"**. An invalid move
	type or format will cause the server to terminate.

4. "+"
	* Server informs the player that it has won the game. The
	player must also terminate upon receiving this input.

5. "-"
	* Server informs the player that it has lost the game. The
	player must also terminate upon receiving this input.

6. "#"
	* Server informs the player that the game ended in a draw.
	The player must also terminate upon receiving this input.

7. ">(...)" (string with '>' as the first character)
	* The substring followed by '>' is the most recent move
	that updated the server state. No response is expected.

Issues
------------------------------------------------------------
* Technical
	* Server to be able to run tournaments (/Makefile)
	* Support for Mac and Windows
	* Report search information
		* number of nodes in the game tree
		* number of nodes pruned
		* number of moves simulated
		* time taken to determine a move at various
			depths
		* number of leaf nodes

* Theoretical
	* Game tree complexity

* Implementation
	* Improve algorithms implemented in solver 1.0
	* Good heuristic evaluation
		* number of legal moves available from a state
		* minimum number of stones to win
		* board dominance
		* number of chains
		* weak vs strong connections
		* mustplay regions 
			* preserving strong connections
			* defensive plays

Nex Problems
------------------------------------------------------------
* **3x3 Nex**
	1. Find all winning openings for Black and conclude that
		a winning strategy exists for the first player.
	2. Find all weak openings for Black and reason why these
		openings are weak. How can White use these openings
		to perhaps win the game?
* **NxN Nex**
	1. Describe the game tree.
	2. Where should the neutral stones be placed?
	3. Find strong and weak openings.
	4. How should the game states be evaluated?
	5. Is Nex a first player win game?

What do we know about Nex so far?
------------------------------------------------------------
* Combinatorics behind 2x2 Nex and 2x2 Hex
	* Number of openings: 12 in Nex, 4 in Hex
	* The opening **Ba1?a2** is the unique winning opening in
		2x2 Nex. Every other opening concludes in a draw.
	* There are 61 nodes in the game tree of 2x2 Nex, comparing 
		with 65 nodes in the game tree of 2x2 Hex.
