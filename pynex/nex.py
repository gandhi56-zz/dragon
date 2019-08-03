from easyAI import TwoPlayersGame
from easyAI.Player import Human_Player

nrows = 3
ncols = 3

class Nex( TwoPlayersGame ):
    """ The board positions are numbered as follows:
            7 8 9
            4 5 6
            1 2 3
    """    

    def __init__(self, players):
		self.players = players
		self.nplayer = 1 # player 1 starts.
		self.board = { chr(j+ord('A')) : ['.' for i in range(ncols)] for j in range(nrows)}
		self.count = {'B':0, 'W':0, '?':0}

    def possible_moves(self):
		'''
		return [i+1 for i,e in enumerate(self.board) if e==0]
		'''
		pass

    def make_move(self, move):

		def submove(self, m):
			stone, row, col = m[0], m[1], m[2:]
			self.board[row][int(col)-1] = stone
			self.count[stone] += 1

		i = 0
		j = 1
		while j < len(move):
			if move[j] == 'B' or move[j] == 'W' or move[j] == '?':
				submove(move[i:j])
				i = j
			j += 1

    def unmake_move(self, move): # optional method (speeds up the AI)
        self.board[int(move)-1] = 0
    
    def lose(self):
        """ Has the opponent "three in line ?" """

		'''
        return any( [all([(self.board[c-1]== self.nopponent)
                      for c in line])
                      for line in [[1,2,3],[4,5,6],[7,8,9], # horiz.
                                   [1,4,7],[2,5,8],[3,6,9], # vertical
                                   [1,5,9],[3,5,7]]]) # diagonal
        '''
    def is_over(self):
        '''
		return (self.possible_moves() == []) or self.lose()
        '''
		pass
    def show(self):
		print()
		print(' ', end='')
		for c in range(1, ncols+1):
			if c < 10:
				print('  ', end='')
			else:
				print(' ', end='')
		print()

		for r in range(0, nrows):
			print(' '*r, end='')
			print(chr(r+ord('A')), end='\\')
			for c in range(ncols):
				print(' {}'.format(self.board[chr(r+ord('A'))]), end='')
			print('\\', chr(r+ord('A')))

		for r in range(nrows+2):
			print(' ', end='')
		for c in range(1, ncols+1):
			if c < 10:
				print('  ', end='')
			else:
				print(' ', end='')

    def scoring(self):
        return -100 if self.lose() else 0
    

if __name__ == "__main__":
    
	'''
    from easyAI import AI_Player, Negamax
    ai_algo = Negamax(6)
    TicTacToe( [AI_Player(ai_algo),AI_Player(ai_algo)]).play()
	'''


	nex = Nex(AI_Player(Negamax(6)), Human_Player)

	nex.show()




