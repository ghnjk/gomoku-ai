BASE_DIR=..

ifeq "$(QUIET)" "1"
    CC=@g++
    AR=@ar -c
    RM=@rm
else
	CC=g++
    AR=ar -c
    RM=rm
endif
CFLAG += -O -g  -Wall -DDEBUG  

#gomoku-common lib
INCLUDE += -I${BASE_DIR}/gomoku-common/code
LIB +=  ${BASE_DIR}/gomoku-common/lib/libgomoku-common.a

#mcts-alg lib
INCLUDE += -I${BASE_DIR}/mcts_alg/code
LIB +=  ${BASE_DIR}/mcts_alg/lib/libgomoku-mcts.a

LIB += -lpthread

#gtest
TEST_INCLUDE += -I/usr/local/lib/gtest/include
TEST_LIB += /usr/local/lib/gtest/lib/gtest_main.a

%.obj: %.cpp
	@echo "start compiling $< ......"
	${CC} -c $(CFLAG)  $(INCLUDE) -o $@ $<
	@echo "compiled ok!"
%.test: %.cpp
	@echo "start compiling $< ......"
	${CC} $(CFLAG)  $(INCLUDE) ${TEST_INCLUDE} -o $@ $<  ${TEST_LIB} ${LIB}
	@echo "compiled ok!"

.%.d: %.cpp
	$(CC) $(INCLUDE) -MM $< > $@
	@$(CC) $(INCLUDE) -MM $< | sed s/"^"/"\."/  |  sed s/"^\. "/" "/  | \
		sed s/"\.o"/"\.d"/  >> $