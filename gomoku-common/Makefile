include ../inc.mk

LIB_GOMOKU = lib/libgomoku-common.a

UTIL_CPP = $(notdir $(wildcard code/util/*.cpp))
OBJ_FILE += $(patsubst %.cpp, code/util/%.obj, ${UTIL_CPP} )

GOMOKU = $(notdir $(wildcard code/gomoku/*.cpp))
OBJ_FILE += $(patsubst %.cpp, code/gomoku/%.obj, ${GOMOKU} )

TEST_FILE = $(notdir $(wildcard test/gomoku/test_*.cpp))
TEST += $(patsubst %.cpp, test/gomoku/%.test, ${TEST_FILE} )

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

test: ${TEST}
	@echo "compile test ok."
	@find test/ -name test*.test -exec  {} \;
clean:
	rm -f ${TEST}
	rm -f ${OBJ_FILE}
	rm -f ${all}
	rm -rf lib/include
