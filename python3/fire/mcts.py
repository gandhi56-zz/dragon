
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
import time
import copy
from math import sqrt, log

AI = ['greedy', 'uct']

class MCTNode:
	def __init__(self, move = None, parent = None):
		self.move = move
		self.parent = parent
		self.children = list()
		self.wins, self.visits = 0, 0

	def expand_node(self, state):
		if state.gameover():
			print('game over!')
			return

		for move in state.get_moves():
			self.children.append(MCTNode(move, self))

	def update(self, result):
		self.visits += 1
		if result > 0:
			self.wins += 1

	def is_leaf(self):
		return len(self.children) == 0

	def has_parent(self):
		return self.parent is not None

def mcts(currState, timeBudget):

	'''
	TODO
	- troubleshoot mcts()
	'''

	startTime = time.time()
	root = MCTNode()
	player = currState.next()
	depth = 0
	while time.time() - startTime < timeBudget:
		node, state = root, copy.deepcopy(currState)
		print('before selection')
		state.show()
		while not node.is_leaf():
			node, _ = select_policy(node, depth)
			state.play(node.move)
			depth += 1

		print('after selection')
		state.show()

		print('before expansion')

		if not state.gameover():
			node.expand_node(state)
			node, _ = select_policy(node, depth)
			print('expanded new nodes')
			depth += 1
			while not state.gameover():
				move = random.choice(state.get_moves())
				state.play(move)
				state.show()
				print('status =', state.status)
				print()
				print()

		print('after rollout')
		state.show()

		result = state.evaluate(player)
		print('result =', result, 'player =', player)
		while node.has_parent():
			node.update(result)
			node = node.parent
			#result *= -1

		del state
	bestValue, bestMove = select_policy(root, 0)
	print('best move', bestMove)

	for child in root.children:
		print(child.move, child.wins/child.visits if child.visits > 0 else '0')


def select_policy(node, depth):
	bestValue = 1000 if depth&1 else -1000
	bestNode = None
	bestMove = None
	for i in range(len(node.children)):
		val = 0
		if node.children[i].visits > 0:
			w, v = node.children[i].wins, node.children[i].visits
			val = w/v

		if depth&1:
			if val < bestValue:
				bestValue = val
				bestNode = node.children[i]
				bestMove = node.children[i].move
		else:
			if val > bestValue:
				bestValue = val
				bestNode = node.children[i]
				bestMove = node.children[i].move

	return bestNode, bestMove

def simulate(state):
	move = random.choice(state.get_moves())
	nextState = copy.deepcopy(state)
	nextState.play(move)
	return nextState

'''
def mcts(rootState, itermax, verbose=False, select_policy=None):
	if verbose:
		print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
	
	rootNode = None
	if select_policy == 'greedy':
		rootNode = MCTNode_greedy(state = rootState)
	elif select_policy == 'uct':
		rootNode = MCTNode_uct(state = rootState)
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

	bestChild = sorted(rootNode.childNodes, key=lambda c : c.visits)[-1]
	return bestChild.move, bestChild.wins/bestChild.visits
'''
