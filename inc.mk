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

%.obj: %.cpp
	@echo -e "start compiling $< ......"
	${CC} -c $(CFLAG)  $(INCLUDE) -o $@ $<
	@echo -e "compiled ok!"
.%.d: %.cpp
	$(CC) $(INCLUDE) -MM $< > $@
	@$(CC) $(INCLUDE) -MM $< | sed s/"^"/"\."/  |  sed s/"^\. "/" "/  | \
		sed s/"\.o"/"\.d"/  >> $