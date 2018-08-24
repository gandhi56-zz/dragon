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

```

		1  2  3  4  5  6  7  8  9 10 11 12 13
	a\  .  .  .  .  .  .  .  .  .  .  .  .  .  \a
	 b\  .  .  .  .  .  .  .  .  .  .  .  .  .  \b
	  c\  .  .  .  .  .  .  .  .  .  .  .  .  .  \c
	   d\  .  .  .  .  .  .  .  .  .  .  .  .  .  \d
		e\  .  .  .  .  .  .  .  .  .  .  .  .  .  \e
		 f\  .  .  .  .  .  .  .  .  .  .  .  .  .  \f
		  g\  .  .  .  .  .  .  .  .  .  .  .  .  .  \g
		   h\  .  .  .  .  .  .  .  .  .  .  .  .  .  \h
			i\  .  .  .  .  .  .  .  .  .  .  .  .  .  \i
			 j\  .  .  .  .  .  .  .  .  .  .  .  .  .  \j
			  k\  .  .  .  .  .  .  .  .  .  .  .  .  .  \k
			   l\  .  .  .  .  .  .  .  .  .  .  .  .  .  \l
				m\  .  .  .  .  .  .  .  .  .  .  .  .  .  \m
					 1  2  3  4  5  6  7  8  9 10 11 12 13
```

Although the standard size for this game is 13 x 13, nexC
allows sizes from 3 x 3 upto 13 x 13 as well as rectangular
sizes. The game is played by placing stones on the board, 
turn-wise. Black plays the first move. There are three 
kinds of stones: black (B), white (W) and neutral (?). 
Players have two kinds of moves to choose from:
	
1) place a stone of their colour and a neutral stone 
into two distinct empty cells. For example,
	   
```
		   1  2  3					   1  2  3
		a\  .  .  .  \a				a\  .  .  ?  \a
		 b\  .  .  .  \b			 b\  .  .  B  \b
		  c\  .  .  .  \c			  c\  .  .  .  \c
			   1  2  3				       1  2  3
```

Black places a `B` on `b3` and a `?` on `a3`.

2) convert two neutral stones on the board into the
	player's colour and convert one of their coloured
	stones into a neutral stone. For example,

```
		   1  2  3					   1  2  3
		a\  ?  .  ?  \a				a\  W  .  W  \a
		 b\  B  W  B  \b			 b\  B  ?  B  \b
		  c\  .  ?  .  \c			  c\  .  ?  .  \c
			   1  2  3				       1  2  3
```
White converts `?` at a1 and a3 to `W` and `W` at b2 to 
`?`.
			   
The objective of the game is to form a connection between
opposite sides. Black aims to connect the top and bottom
edges with `B` while white aims to connect the left and
right edges with `W`.

					   1  2  3
					a\  W  B  W  \a
					 b\  ?  B  B  \b
					  c\  B  .  W  \c
						   1  2  3

black forms a connection from the top edge to the bottom 
edge with the help of the stones on a2, b2 and c1.

The game terminates when either player wins or no legal
moves exist.

Utility
------------------------------------------------------------
<Incomplete>

Communication Protocol
------------------------------------------------------------
The server and the players are expected to follow the
protocol given below; following are the different input
commands a player may receive from the server:

1) "r<R>-c<C>#"
	Server sends the game settings (format may be extended 
	later). R denotes the number of rows of the board and
	C denotes the number of columns.

2) "!"
	Due to an unexpected error caused by a player or the
	server, the player must exit with status 1.

3) "?"
	Server expects the player to send a move in the following
	valid format and type; "#RC#RC(#RC)". An invalid move
	type or format will cause the server to terminate.

4) "+"
	Server informs the player that it has won the game. The
	player must also terminate upon receiving this input.

5) "-"
	Server informs the player that it has lost the game. The
	player must also terminate upon receiving this input.

6) "#"
	Server informs the player that the game ended in a draw.
	The player must also terminate upon receiving this input.

7) ">(...)" (string with '>' as the first character)
	The substring followed by '>' is the most recent move
	that updated the server state. No response is expected.

Issues
------------------------------------------------------------
* Timer benchmarking
* Random player

Nex Problems
------------------------------------------------------------
1. Describe the game tree.
2. Where should the neutral stones be placed?
3. Solve the 1s2n problem.

					   1  2  3  4
					a\  .  .  .  .  \a
					 b\  .  .  .  .  \b
					  c\  .  *  .  .  \c
					   d\  ?  ?  .  .  \d
						   1  2  3  4

	What should player * do? What should the opponent 
	of * do if its their turn to play?

4. Find strong and weak openings.
5. How should the game states be evaluated?
6. Is Nex a first player win game?
