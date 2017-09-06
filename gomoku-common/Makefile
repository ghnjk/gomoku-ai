include ../inc.mk


UTIL_CPP = $(notdir $(wildcard code/util/*.cpp))
OBJ_FILE += $(patsubst %.cpp, code/util/%.obj, ${UTIL_CPP} )

GOMOKU = $(notdir $(wildcard code/gomoku/*.cpp))
OBJ_FILE += $(patsubst %.cpp, code/gomoku/%.obj, ${GOMOKU} )



all = lib/libgomoku-common.a

${all} : ${OBJ_FILE}
	@mkdir -p lib/include
	@cp -r code/* lib/include
	@find lib/include -name *.cpp -exec rm {} \;
	@find lib/include -name *.obj -exec rm {} \;
	@echo -e "generating $@ ......\c"
	${AR} -rs $@ $^
	@echo -e " ok!"

clean:
	rm -f ${OBJ_FILE}
	rm -f ${all}
	rm -rf lib/include
