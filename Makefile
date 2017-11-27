

all:
	cd gomoku-common && make
	cd mcts_alg && make
	cd tools && make
test:
	cd gomoku-common && make test
	cd mcts_alg && make test
clean:
	cd gomoku-common && make clean
	cd mcts_alg && make clean
	cd tools && make clean
	find . -name *.dSYM -exec rm -rf {} \;
