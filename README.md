# NexC

### Project Overview
NexC is a command-line interface for the game of Nex. This engine provides support for running Nex games amongst players upon a two-way communication link over a network. NexC also includes a Nex visualization tool, called NexViz; this tool can be used to visualize Nex games, either through a text file or a direct input of moves into the webpage.

### Game description
Nex is a connection game played between two players, namely Black and White. This game is a variation of the game of Hex. Fig.0 presents the board upon which this game is played, produced by NexC:

![9x9Nex](./images/9x9nex.png)

*Fig.0: Initial configuration of the 9 x 9 Nex board.*

The game starts with an empty board. Each cell can contain either a Black (**B**), White (**W**) or a Neutral (**?**) stone. By convention, Black plays the first move by playing a _generate_ move, as defined below.

1. Generate move: The player places a stone of their colour and a 
	Neutral stone into two distinct empty cells. 
	
For example,


![3x3blank](./images/blank.png)
![3x3genmove](./images/3x3genmove.png)


*Fig.1 shows the state obtained after Black places a black stone in cell a1 and a neutral stone in cell c2.*

Alternatively, if a state contains at least two neutral stones, the player may choose to play the following move, called the _transform_ move:

2. Transform move: The player converts two Neutral stones on the board into their colour and convert one of their coloured stones into a Neutral stone. 
	
For example,

![3x3transform1](./images/3x3transform1.png)
![3x3transform2](./images/3x3transform2.png)

*Fig.4: The state obtained after Black converts **?** at _a2_ and _c2_ to **B** and **B** at _a1_ to **?**.*

#### Terminology

We say that two adjacent cells are connected if both cells are nonempty and both cells contain stones of the same kind.

We define a connection between two cells, A and B, a nonempty sequence of cells such that every consecutive pair of cells in the sequence are connected, with A and B being the first and last cells in the sequence. A connection is a _B-connection_ if every cell in the connection is occupied by a **B** stone. Similarly, a connection is a _W-connection_ if every cell in the connection is occupied by a **W** stone.

The _objective_ for Black is to form a B-connection between some cell in row _a_ and a cell in the bottommost row. Likewise, the _objective_ for White is to form a W-connection between some cell in column _1_ and some cell in the rightmost column. If neither objective is satisfied and there is no legal move that can be played by the player to move, then we conclude that the game has terminated in a _draw_.

![3x3Blackwin](./images/3x3blackwin.png)

*Fig.5: Black wins this position with the help of the B-connection (a2, b2, c2).*

![3x3Whitewin](./images/whitewin.png)

*Fig.6: White wins this position with the help of the W-connection (c1, c2, b3).*

The game terminates when either player wins or no legal moves exist. Legal moves exist if and only if there are at least two empty cells for the player to play a generate move or at least one Neutral stone to allow a transform move to be playable.

We define an _opening_, or an _opening move_, as the first move played on the initial state of the game. Let S be the initial state of the game. Let A be an opening and S' be the state obtained after playing A on S. We say A is _winning_ if for every A' that can be played on S' there exists a sequence of moves that satifies the objective for Black. Such a sequence is defined as a _winning strategy_ for Black. We say A is _losing_ if there exists a move sequence which satisfies the objective for White. Such a move sequence is defined as the _winning strategy_ for White.

### Utility
Follow the steps below to run the program:
After cloning this repository,

```sh
cd nexc
mkdir obj
make run
```

Now attach each player by running their executables over the socket port with port ID 21299.

##### Communication Protocol
The server and the players are expected to follow the
protocol given below; following are the different input
commands a player may receive from the server:

1. "rR-cC#"

	* Server sends the game settings (format may be extended 
	later). R denotes the number of rows of the board and
	C denotes the number of columns. 
	
	* Expected response: "ok"

2. "!"
	* Due to an unexpected error caused by a player or the
	server, the player must exit with status 1.

	* No response expected.

3. "?"
	* The server is waiting for a move to be played.

	* The server expects the player to send a move in the following
	valid format and type; **"#RC#RC(#RC)"**. An invalid move
	type or format will cause the server to terminate.

4. "+"
	* The server informs the player that it has won the game. The
	player must also terminate upon receiving this input.

	* No response expected.

5. "-"
	* The server informs the player that it has lost the game. The
	player must also terminate upon receiving this input.

	* No response expected.

6. "#"
	* The server informs the player that the game ended in a draw.
	The player must also terminate upon receiving this input.

7. ">(...)" (string with '>' as the first character)
	* The substring followed by '>' is the most recent move
	that updated the server state.

	* Expected response: "ok"

8. "$"
	* All games have been run successfully. 
	
	* The server expects the players to terminate.

### Issues
* Technical
	* Improve NexC
		* handle unexpected exceptions
	* Improve NexViz
		* fix bugs
		* implement an applet to visualize Nex games
	* Implement predefined tournament modes:
		* Rapid
		* Blitz
	* Record games into seperate text files
	* Report search information
		* number of nodes in the game tree
		* number of nodes pruned
		* number of moves simulated
		* number of leaf nodes
	* Support for Mac and Windows

* Implementation
	* Finish implementing Monte Carlo Tree Search
    * Heuristic search
		* number of legal moves available from a state
		* minimum number of stones to win
		* board dominance
		* number of chains
		* weak vs strong connections
		* mustplay regions
			* preserving strong connections
			* defensive plays

### Nex Problems
* **3x3 Nex**
	1. Find all winning openings for Black and conclude that
		a winning strategy exists for the first player.
	2. Find all weak openings for Black and reason why these
		openings are weak. How can White use these openings
		to perhaps win the game?
* **NxN Nex**
	1. Describe the game tree.
	2. Where should the Neutral stones be placed?
	3. Find strong and weak openings.
	4. How should the game states be evaluated?
	5. Is Nex a first player win game?

### What do we know about Nex so far?
##### Combinatorics behind 2x2 Nex and 2x2 Hex
* Number of openings: 12 in Nex, 4 in Hex
* The opening **Ba1?a2** and **Bb2?b1** are the winning openings in
	2x2 Nex. Every other opening concludes in a draw.

![](./images/2x2_0.png)
![](./images/2x2_1.png)
![](./images/2x2_2_1.png)
![](./images/2x2_2_12.png)

Alternatively, if White responds by **Wb2?b1** to **Ba1?a2**,

![](./images/2x2_2_0.png)
![](./images/2x2_2_2.png)

Hence, **Ba1?a2** is indeed a winning opening in 2x2 Nex. Symmetrically, **Bb2?b1** can also be proved to be a winning opening.

* There are 61 nodes in the game tree of 2x2 Nex, comparing 
	with 65 nodes in the game tree of 2x2 Hex.



### Nex players
* PseudoRand
	* Selects a move pseudorandomly
* Solver 1.0
	* Implements Alpha-beta Negamax search
	* State space complexity: ~200n bits, where n is the number of cells
	* Intractable for boards bigger than 3x3
* Solver 2.0
	* Implements Alpha-beta Negamax search
	* Random shuffles the moves before move selection
	* State representation using a bitboard, where each cell is denoted using 2 bits
	* State space complexity: ~20n bits, where n is the number of cells
	* Intractable for boards bigger than 3x3
	* Records
		* vs Solver 1.0, won 40/40 as Black
		* vs PseudoRand, won 39/40 and drew 1/40 as Black
* Solver 3.0 (Coming soon...)
* Random Bits (Coming soon...)
	* state representation as in Solver 2.0

### Nex tournaments

* **November 28 Midnight Challenge**

	*	Game settings:
		* Board size: 3 x 3
		* Time Control: None
		* Handicap: None


	* Results:

		| Black      | White      | Games played | Black won | Draw | White won |
		|:----------:|:----------:|:------------:|:---------:|:----:|:---------:|
		| Solver 1.0 | Solver 2.0 | 1            | 1         | 0    | 0         |
		| Solver 2.0 | Solver 1.0 | 1            | 0         | 1    | 0         |
		| Solver 1.0 | PseudoRand | 5            | 5         | 0    | 0         |
		| PseudoRand | Solver 1.0 | 5            | 0         | 3    | 2         |
		| Solver 2.0 | PseudoRand | 5            | 5         | 0    | 0         |
		| PseudoRand | Solver 2.0 | 5            | 1         | 3    | 1         |

* **December 2 Morning breakfast**
	* Game settings:
		* Board size : 3 x 3
		* Time Control : None
		* Handicap : None

	* Results:
		
		| Black      | White      | Games played | Black won | Draw | White won |
		|:----------:|:----------:|:------------:|:---------:|:----:|:---------:|
		| Solver 1.0 | Solver 2.0 | 2            | 2         | 0    | 0         |
		| Solver 2.0 | Solver 1.0 | 2            | 2         | 0    | 0         |
		| Solver 1.0 | PseudoRand | 5            | 5         | 0    | 0         |
		| PseudoRand | Solver 1.0 | 5            | 0         | 1    | 4         |
		| Solver 2.0 | PseudoRand | 5            | 5         | 0    | 0         |
		| PseudoRand | Solver 2.0 | 5            | 0         | 2    | 3         |


### Interesting games to analyze

##### Solver 1.0 vs Solver 1.0 on 3x3 Nex

![](./images/solver1_solver1/blank.png)
![](./images/solver1_solver1/0_1.png)
![](./images/solver1_solver1/0_2.png)
![](./images/solver1_solver1/0_3.png)
![](./images/solver1_solver1/0_4.png)
![](./images/solver1_solver1/0_5.png)

##### Solver 2.0 vs Solver 2.0 on 3x3 Nex

![](./images/solver2_solver2/blank.png)
![](./images/solver2_solver2/0_1.png)
![](./images/solver2_solver2/0_2.png)
![](./images/solver2_solver2/0_3.png)
![](./images/solver2_solver2/0_4.png)
![](./images/solver2_solver2/0_5.png)
![](./images/solver2_solver2/0_6.png)
![](./images/solver2_solver2/0_7.png)

##### Solver 2.0 vs Solver 1.0 on 3x3 Nex
![](./images/solver2_solver1/blank.png)
![](./images/solver2_solver1/1_0.png)
![](./images/solver2_solver1/1_1.png)
![](./images/solver2_solver1/1_2.png)
![](./images/solver2_solver1/1_3.png)
![](./images/solver2_solver1/1_4.png)

![](./images/solver2_solver1/blank.png)
![](./images/solver2_solver1/0_0.png)
![](./images/solver2_solver1/0_1.png)
![](./images/solver2_solver1/0_2.png)
![](./images/solver2_solver1/0_3.png)
![](./images/solver2_solver1/0_4.png)


##### Solver 2.0 vs PseudoRand on 3x3 Nex (Draw!)

![](./images/solver2_solver2/draw/0_0.png)
![](./images/solver2_solver2/draw/0_1.png)
![](./images/solver2_solver2/draw/0_2.png)
![](./images/solver2_solver2/draw/0_3.png)
![](./images/solver2_solver2/draw/0_4.png)
![](./images/solver2_solver2/draw/0_5.png)
![](./images/solver2_solver2/draw/0_6.png)
![](./images/solver2_solver2/draw/0_7.png)

