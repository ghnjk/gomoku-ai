include ../inc.mk

APP_FULE = $(notdir $(wildcard *.cpp))
APPS = $(patsubst %.cpp, %.exe, ${APP_FULE} )

all: ${APPS}

%.exe: %.cpp
	${CC} $(CFLAG)  $(INCLUDE) -o $@ $< ${LIB}

clean:
	rm -f ${APPS}