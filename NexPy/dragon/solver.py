

def negamaxAB(state, depth, alpha, beta, verbose=False):
	if verbose:
		state.show()
	
	state.switch_player()

	if state.is_over():
		return get_result()

