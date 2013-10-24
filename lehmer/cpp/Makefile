# Default to g++
CC=g++
# Generate compilation unite
LIBFLAGS=-c
# Default optimization, include debugging information
EXEFLAGS=-g

all: lehmer.o LehmerTest.cpp
	${CC} ${EXEFLAGS} LehmerTest.cpp lehmer.o -o lehmerTest

obj: lehmer.o

lehmer.o: LehmerPRNG.h LehmerPRNG.cpp
	${CC} ${LIBFLAGS} LehmerPRNG.cpp -o lehmer.o
	
test: lehmer.o LehmerTest.cpp
	${CC} ${EXEFLAGS} LehmerTest.cpp lehmer.o -o lehmerTest

clean:
	-rm -f lehmer.o
	-rm -f lehmerTest
