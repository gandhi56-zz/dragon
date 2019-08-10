
# Monte Carlo Tree Search algorithm for two player games
# --------------------------------------------------------------------------------
'''
	Requirements to run:
		class State:
			- get_moves()
			- playerJustMoved
			- clone()
			- do_move() [expected to switch players and play the move]
			- get_result()
'''
# --------------------------------------------------------------------------------

import random
from copy import deepcopy

class MCTNode:
	def __init__(self, move = None, parent = None, state = None):
		self.move = move
		self.parentNode = parent
		self.childNodes = []
		self.wins, self.visits = 0, 0
		self.untriedMoves = state.get_moves()
		self.playerJustMoved = state.playerJustMoved

	def select_child(self):
		# child selection policy
		return sorted(self.childNodes, key=lambda c : c.wins/c.visits)[-1]

	def add_child(self, m, s):
		node = MCTNode(m, self, s)
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

		# selection ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if verbose:
			print('selection')
		while len(node.untriedMoves) == 0 and len(node.childNodes) > 0:
			node = node.select_child()
			state.do_move(node.move)
		# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		# expansion ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if verbose:
			print('\nexpansion')
		if len(node.untriedMoves) > 0:
			move = random.choice(node.untriedMoves)
			state.do_move(move)
			node = node.add_child(move, state)
			if verbose:
				print('--', move)
		# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		# simulation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if verbose:
			print('\nsimulation')
		while True:
			movesList = state.get_moves()
			if len(movesList) == 0:
				break
			state.do_move(random.choice(movesList))
		# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
		# backpropagation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if verbose:
			print('\nsimulation')
		while node != None:
			node.update(state.get_result(node.playerJustMoved))
			node = node.parentNode
		# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	if verbose:
		print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~')

	return sorted(rootNode.childNodes, key=lambda c : c.visits)[-1].move