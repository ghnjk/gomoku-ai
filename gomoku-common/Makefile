include ../inc.mk

LIB_GOMOKU = lib/libgomoku-common.a

TOOLS_CPP = $(notdir $(wildcard code/tools/*.cpp))
OBJ_FILE += $(patsubst %.cpp, code/tools/%.obj, ${TOOLS_CPP} )

GOMOKU = $(notdir $(wildcard code/gomoku/*.cpp))
OBJ_FILE += $(patsubst %.cpp, code/gomoku/%.obj, ${GOMOKU} )

GOMOKU_TEST_FILE = $(notdir $(wildcard test/gomoku/test_*.cpp))
TEST += $(patsubst %.cpp, test/gomoku/%.test, ${GOMOKU_TEST_FILE} )
TOOLS_TEST_FILE += $(notdir $(wildcard test/tools/test_*.cpp))
TEST += $(patsubst %.cpp, test/tools/%.test, ${TOOLS_TEST_FILE} )


TEST_LIB += ${LIB_GOMOKU}

all = ${LIB_GOMOKU}

${all} : ${OBJ_FILE}
	@mkdir -p lib/include
	@cp -r code/* lib/include
	@find lib/include -name *.cpp -exec rm {} \;
	@find lib/include -name *.obj -exec rm {} \;
	@echo -e "generating $@ ......\c"
	${AR} -rs $@ $^
	@echo -e " ok!"

test: ${all} ${TEST} run_test
	@echo "compile test ok."

run_test:
	@cd test/gomoku ; find . -type f -name test_*.test -exec {} \;
	@cd test/tools ; find . -type f -name test_*.test -exec {} \;
clean:
	rm -f ${TEST}
	rm -f ${OBJ_FILE}
	rm -f ${all}
	rm -rf lib/include
