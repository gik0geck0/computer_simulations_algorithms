FLAGS?=-g -O0

test: welford.cpp welfordtest.cpp
	g++ ${FLAGS} -Wall -pedantic -o welfordtest welford.cpp welfordtest.cpp

lib: welford.cpp
	g++ ${FLAGS} -Wall -pedantic -c welford.cpp;ar rvs libwelford.a welford.o

clean:
	rm -f welfordtest
	rm -f welford.o
	rm -f libwelford.a
