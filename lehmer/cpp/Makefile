lib: lehmer.o

lehmer.o: LehmerPRNG.h LehmerPRNG.cpp
	g++ -c LehmerPRNG.cpp -o lehmer.o
	
test: lehmer.o LehmerTest.cpp
	g++ LehmerTest.cpp lehmer.o -o lehmerTest

clean:
	rm lehmer
