from copy import deepcopy
import random
from math import sqrt, log

nrows = 3
ncols = 3

BLACK_WON = 0
WHITE_WON = 1
DRAW = 2
NOT_DONE = 3


class NexState:
	def __init__(self):
		self.playerJustMoved = 2
		self.board = { chr(j+ord('a')) : {str(i+1) : '.' for i in range(ncols)} for j in range(nrows)}
		self.count = {'B':0, 'W':0, '?':0}
		self.status = NOT_DONE

	def clone(self):
		return deepcopy(self)

	def do_move(self, move, usedByUndo=False):
		# switch players
		self.playerJustMoved = 3 - self.playerJustMoved

		# play move
		self.make_move(move, usedByUndo)
	
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
	
	def get_moves(self, allMoves=True):
		emptyPos = list()
		stonePos = list()
		neutralPos = list()
		stone = 'B'
		if self.playerJustMoved == 2:
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

		if len(neutralPos) >= 2 and len(stonePos) >= 1 and allMoves:
			for i in range(len(neutralPos)):
				for j in range(i+1, len(neutralPos)):
					for k in range(len(stonePos)):
						moves.append(stone+neutralPos[i]+stone+neutralPos[j]+'?'+stonePos[k])
		return moves
	
	def get_result(self, player):
		if self.check_black_win():
			self.status = BLACK_WON
			if player == 1:
				return 1.0
			return 0.0
		elif self.check_white_win():
			self.status = WHITE_WON
			if player == 2:
				return 1.0
			return 0.0
		elif len(self.get_moves()) == 0:
			self.status = DRAW
		return 0.0

	def check_black_win(self):
		return self.connected(('B', '0'), ('B', '1'))
	
	def check_white_win(self):
		return self.connected(('W', '0'), ('W', '1'))
	
	def show(self):
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

		def is_over(self):
			self.get_result()
			return self.status != NOT_DONE


def play_game():

	from dragon.mcts import MCTNode, mcts

	black = 0
	white = 0
	draw = 0

	for gameNum in range(1,101):
		state = NexState()
		while True:
			#print(state.status)
			#state.show()
			state.get_result(state.playerJustMoved)

			if state.status != NOT_DONE:
				break

			if state.playerJustMoved == 1:
				move = mcts(rootState = state, itermax = 1000, verbose = False)
			else:
				move = mcts(rootState = state, itermax = 1000, verbose = False)

			#print('Best move estimate:', move)
			state.do_move(move)

		if state.status == BLACK_WON:
			#print('Black')
			black += 1
		elif state.status == WHITE_WON:
			#print('White')
			white += 1
		elif state.status == DRAW:
			#print('Draw')
			draw += 1
		#else:
			#print('Error')

	print()
	print('black wins', black)
	print('white wins', white)
	print('draw', draw)


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

	'''
	for gameNum in range(1):
		nex = Nex( [player1, player2])
		while not nex.is_over():
			nex.show()

			if nex.nplayer == 1:
				move = mcts(nex, 1000)
				#move = player1.ask_move(nex)
			elif nex.nplayer == 2:
				#move = input('Move:').strip()
				move = player1.ask_move(nex)
				#move = random.choice(nex.possible_moves())
			#print('playing', move)

			nex.do_move(move)

		print('Game terminated.')

		nex.show()
		print('game #', gameNum, end=':')
		if nex.status == BLACK_WON:
			print('Black won!')
			black += 1
		elif nex.status == WHITE_WON:
			print('White won!')
			white += 1
		else:
			print('Draw!')
			draw += 1

	print('black won', black, 'games')
	print('white won', white, 'games')
	print(draw, 'games drew')

	'''

	
	play_game()

