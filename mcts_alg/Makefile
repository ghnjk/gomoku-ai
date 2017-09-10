include ../inc.mk

LIB_MCTS_ALG = lib/libgomoku-mcts.a


MCTS_ALG = $(notdir $(wildcard code/mcts_alg/*.cpp))
OBJ_FILE += $(patsubst %.cpp, code/mcts_alg/%.obj, ${MCTS_ALG} )

TEST_FILE = $(notdir $(wildcard test/mcts_alg/test_*.cpp))
TEST += $(patsubst %.cpp, test/mcts_alg/%.test, ${TEST_FILE} )

TEST_LIB += ${LIB_MCTS_ALG}


all = ${LIB_MCTS_ALG}

target: ${all} mcts_ai

${all} : ${OBJ_FILE}
	@mkdir -p lib/include
	@cp -r code/* lib/include
	@find lib/include -name *.cpp -exec rm {} \;
	@find lib/include -name *.obj -exec rm {} \;
	@echo -e "generating $@ ......\c"
	${AR} -rs $@ $^
	@echo -e " ok!"

mcts_ai: code/main.cpp
	echo "start compiling $< ......"
	${CC} $(CFLAG)  $(INCLUDE) -o $@ $<  ${LIB}
	@echo "compiled ok!"


test: ${all} ${TEST} 
	@echo "compile test ok."
	@find test/ -name test*.test -exec  {} \;
clean:
	rm -f ${TEST}
	rm -f ${OBJ_FILE}
	rm -f ${all}
	rm -rf lib/include
	rm -f mcts_ai
