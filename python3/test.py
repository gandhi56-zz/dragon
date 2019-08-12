def foo():
	for i in range(1, 11):
		yield i

for i in foo():
	print(i)
