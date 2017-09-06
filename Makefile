

all:
	cd gomoku-common && make
test:
	cd gomoku-common && make test
clean:
	cd gomoku-common && make clean
	find . -name *.dSYM -exec rm -rf {} \;
