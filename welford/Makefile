FLAGS?=-g

all: welford.o welfordtest.cpp
	g++ ${FLAGS} -Wall -pedantic -o welfordtest welford.cpp welfordtest.cpp

c: welford.c
	gcc --std=c99 -g -Wall -pedantic welford.c -o welfordtest
	
welford.o: welford.cpp
	g++ ${FLAGS} -Wall -pedantic -c welford.cpp

clean:
	rm -f welfordtest
	rm -f welford.o
