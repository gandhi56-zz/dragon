from easyAI import TwoPlayersGame
from copy import deepcopy
import random
from math import sqrt, log

nrows = 3
ncols = 3

BLACK_WON = 0
WHITE_WON = 1
DRAW = 2
NOT_DONE = 3

class Nex( TwoPlayersGame ):
	def __init__(self, players, startPos=None):
		self.players = players
		self.nplayer = 1 # player 1 starts.
		self.board = { chr(j+ord('a')) : {str(i+1) : '.' for i in range(ncols)} for j in range(nrows)}
		self.count = {'B':0, 'W':0, '?':0}
		self.status = NOT_DONE

		if startPos is not None:
			self.make_move(startPos)

	def clone(self):
		return deepcopy(self)

	def possible_moves(self):
		emptyPos = list()
		stonePos = list()
		neutralPos = list()
		stone = 'B'
		if self.nplayer == 1:
			stone = 'B'
		else:
			stone = 'W'

		for r in self.board.keys():
			for c in self.board[r].keys():
				if self.board[r][c] == '.':
					emptyPos.append(r+c)
				elif self.board[r][c] == stone:
					stonePos.append(r+c)
				elif self.board[r][c] == '?':
					neutralPos.append(r+c)
		moves = list()
		if len(emptyPos) >= 2:
			for i in range(len(emptyPos)):
				for j in range(i+1, len(emptyPos)):
					moves.append(stone+emptyPos[i]+'?'+emptyPos[j])
					moves.append(stone+emptyPos[j]+'?'+emptyPos[i])

		if len(neutralPos) >= 2 and len(stonePos) >= 1:
			for i in range(len(neutralPos)):
				for j in range(i+1, len(neutralPos)):
					for k in range(len(stonePos)):
						moves.append(stone+neutralPos[i]+stone+neutralPos[j]+'?'+stonePos[k])
		return moves


	def make_move(self, move, used_by_undo = False):
		def submove(board, count, m):
			stone, row, col = m[0], m[1], m[2:]
			board[row][col] = stone
			if not used_by_undo:
				count[stone] += 1

		i = 0
		j = 1
		while j < len(move):
			if move[j] == 'B' or move[j] == 'W' or move[j] == '?':
				submove(self.board, self.count, move[i:j])
				i = j
			j += 1
		submove(self.board, self.count, move[i:j])
	
	def unmake_move(self, move):
		playerStone = 'B'
		if move.count('B') == 0:
			playerStone = 'W'
		def undo_genmove(board, count, m):
			stone, row, col = m[0], m[1], m[2:]
			board[row][col] = '.'
			count[stone] -= 1
		
		def undo_transform_move(board, count, m):
			stone, row, col = m[0], m[1], m[2:]
			if stone == '?':
				board[row][col] = playerStone
				count[stone] += 1
				count['?'] -= 1
			else:
				board[row][col] = '?'
				count[stone] -= 1
				count['?'] += 1
		moveType = 'transform'
		if move.count(playerStone) == 1:
			moveType = 'generate'
		i = 0
		j = 1
		while j < len(move):
			if move[j] == playerStone or move[j] == '?':
				if moveType == 'generate':
					undo_genmove(self.board, self.count, move[i:j])
				else:
					undo_transform_move(self.board, self.count, move[i:j])
				i = j
			j += 1
		
		if moveType == 'generate':
			undo_genmove(self.board, self.count, move[i:j])
		else:
			undo_transform_move(self.board, self.count, move[i:j])

	def lose(self):
		return ((self.nplayer == 1 and self.status == WHITE_WON) or 
				(self.nplayer == 2 and self.status == BLACK_WON))

	def is_over(self):
		self.status = DRAW
		if self.connected(('B','0'), ('B','1')):
			self.status = BLACK_WON
			return True
		elif self.connected(('W','0'), ('W','1')):
			self.status = WHITE_WON
			return True
		emptyCells = (nrows*ncols) - self.count['B'] - self.count['W'] - self.count['?']
		if emptyCells <= 1:
			if self.count['?'] <= 1:
				return True
		if len(self.possible_moves()) == 0:
			return True
		self.status = NOT_DONE
		return False

	def show(self):
		print('nplayer = ', self.nplayer)
		print('  ', end='')
		for c in range(1, ncols+1):
			if c < 10:
				print('  {}'.format(c), end='')
			else:
				print(' {}'.format(c), end='')
				
		print()

		for r in self.board.keys():
			print(' '*(ord(r)-ord('a')), end='')
			print(r, end=' \\')
			for c in self.board[r].keys():
				print('  {}'.format(self.board[r][c]), end='')
			print('  \\', r)

		print(' ', end='')
		for r in range(nrows+2):
			print(' ', end='')
		for c in range(1, ncols+1):
			if c < 10:
				print('  {}'.format(c), end='')
			else:
				print(' {}'.format(c), end='')
		print('\n')

	def scoring(self):
		return -100 if self.lose() else 0
	
	def nbrs(self, key):
		# key is a tuple (r,c)
		if key[0] == 'B':
			r = 'a'
			if key[1] == '1':
				r = chr(ord('a')+nrows-1)
			return [(r, str(c)) for c in range(1, ncols+1)]
		elif key[0] == 'W':
			c = '1'
			if key[1] == '1':
				c = chr(ord('1')+ncols-1)
			return [(r,c) for r in self.board.keys()]

		nbrs = []
		if self.valid_pos(chr(ord(key[0])+1), key[1]):
			nbrs.append((chr(ord(key[0])+1), key[1]))
		if self.valid_pos(chr(ord(key[0])-1), key[1]):
			nbrs.append((chr(ord(key[0])-1), key[1]))
		if self.valid_pos(key[0], chr(ord(key[1])+1)):
			nbrs.append((key[0], chr(ord(key[1])+1)))
		if self.valid_pos(key[0], chr(ord(key[1])-1)):
			nbrs.append((key[0], chr(ord(key[1])-1)))
		if self.valid_pos(chr(ord(key[0])-1), chr(ord(key[1])+1)):
			nbrs.append((chr(ord(key[0])-1), chr(ord(key[1])+1)))
		if self.valid_pos(chr(ord(key[0])+1), chr(ord(key[1])-1)):
			nbrs.append((chr(ord(key[0])+1), chr(ord(key[1])-1)))

		if key[0] == 'a':
			nbrs.append(('B', '0'))
		if ord(key[0])-ord('a') == nrows-1:
			nbrs.append(('B', '1'))
		if key[1] == '1':
			nbrs.append(('W', '0'))
		if ord(key[1])-ord('1') == nrows-1:
			nbrs.append(('W', '1'))

		return nbrs

	def valid_pos(self, r,c):
		return (r >= 'a' and r < chr(ord('a')+nrows) and c >= '1' and c < chr(ord('1')+ncols))

	def connected(self, start, end):
		isConnected = False
		visited = {('B','0'):False, ('B','1'):False,('W','0'):False, ('W','1'):False}
		for r in self.board.keys():
			for c in self.board[r].keys():
				visited[(r,c)] = False

		stack = list()
		stack.append(start)
		visited[start] = True
		while len(stack) > 0 and not isConnected:
			curr = stack.pop()
			if curr == end:
				isConnected = True
				break
			for adj in self.nbrs(curr):
				if visited[adj]:
					continue
				if adj == end:
					stack.append(adj)
					visited[adj] = True
					isConnected = True
					break
				if adj[0] == 'B' or adj[0] == 'W':
					continue
				if self.board[adj[0]][adj[1]] == 'B' and curr[0] != 'W':
					if curr[0] == 'B' or self.board[curr[0]][curr[1]] == 'B':
						stack.append(adj)
						visited[adj] = True
				elif self.board[adj[0]][adj[1]] == 'W' and curr[0] != 'B':
					if curr[0] == 'W' or self.board[curr[0]][curr[1]] == 'W':
						stack.append(adj)
						visited[adj] = True
		return isConnected

	def ttentry(self):

		# TODO use polynomial hashing to store the game state as key

		state = ''
		for r in self.board.keys():
			for c in self.board[r].keys():
				state += self.board[r][c]
		return state

class MCTNode:
	def __init__(self, move = None, parent = None, state = None):
		self.move = move
		self.parentNode = parent
		self.childNodes = []
		self.wins, self.visits = 0, 0
		self.untriedMoves = state.possible_moves()
		self.playerJustMoved = state.nplayer

	def select_child(self):
		return sorted(self.childNodes, key=lambda c : c.wins/c.visits + sqrt(2*log(self.visits)/c.visits))[-1]

	def add_child(self, m, s):
		node = MCTNode(move=m, parent=self, state=s)
		self.untriedMoves.remove(m)
		self.childNodes.append(node)
		return node

	def update(self, result):
		self.visits += 1
		self.wins += result

def mcts(rootState, itermax, verbose=False):
	if verbose:
		print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
	rootNode = MCTNode(state = rootState)
	for i in range(itermax):
		node = rootNode
		state = rootState.clone()

		# selection
		if verbose:
			print('selection')
		while len(node.untriedMoves) == 0 and len(node.childNodes) > 0:
			node = node.select_child()
			if verbose:
				print('--', node.move)
			state.make_move(node.move)
			if state.is_over():
				break
		if verbose:
			print()

		# expansion
		if verbose:
			print('expansion')
		if len(node.untriedMoves) > 0:
			move = random.choice(node.untriedMoves)
			state.make_move(move)
			if verbose:
				print('--', move)
			node = node.add_child(move, state)
		if verbose:
			print()

		# simulation
		if verbose:
			print('simulation')
		while not state.is_over():
			state.make_move(random.choice(state.possible_moves()))

		result = 1
		if (state.status == BLACK_WON and state.nplayer == 2) or (state.status == WHITE_WON and state.nplayer == 1):
			result = -1
		elif state.status == DRAW:
			result = 0

		if verbose:
			print('result =', result)
		if verbose:
			print()
		# backpropagation
		if verbose:
			print('simulation')
		while node != None:
			node.update(result)	# TODO
			result *= -1
			node = node.parentNode
	
	if verbose:
		print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~')

	return sorted(rootNode.childNodes, key=lambda c : c.visits)[-1].move

if __name__ == "__main__":
	from easyAI import AI_Player, Negamax, DUAL, SSS, TT
	from easyAI.Player import Human_Player
	import cProfile
	player2 = Human_Player()
	player1 = AI_Player(SSS(100, tt=TT()))
	#cProfile.run('nex.play(100)')

	black = 0
	white = 0
	draw = 0

	for gameNum in range(5):
		nex = Nex( [player1, player2])
		while not nex.is_over():
			#nex.show()

			if nex.nplayer == 1:
				move = mcts(nex, 1000)
			elif nex.nplayer == 2:
				#move = input('Move:').strip()
				move = player1.ask_move(nex)
				#move = random.choice(nex.possible_moves())
			#print('playing', move)

			nex.make_move(move)
			nex.nplayer = 1 if nex.nplayer == 2 else 2

		print('Game terminated.')

		print(gameNum)
		#nex.show()
		if nex.status == BLACK_WON:
			print('Black won!')
			black += 1
		elif nex.status == WHITE_WON:
			print('White won!')
			white += 1
		else:
			print('Draw!')
			draw += 1

	print(black)
	print(white)
	print(draw)

