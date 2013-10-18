
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "lehmerset.h"

LehmerSet::LehmerSet(long a, long m, int streamcount, long seed){
	long aj = jumpMult(streamcount);
	long q = m / a;
	long r = m % a;
	for(int i = 0; i < streamcount; i++){
		streams.push_back(LehmerStream(a, m, seed));
		draws.push_back(0);
		seed = aj * (seed % q) - r * (seed / q);
		seed += (seed > 0 ? 0 : m);
	}
}

LehmerSet::LehmerSet(long a, long m, long aj, long seed) : a(a), m(m){
	long q = m / a;
	long r = m % a;
	for(int i = 0; i < COUNT_DEFAULT; i++){
		streams.push_back(LehmerStream(a, m, seed));
		draws.push_back(0);
		seed = aj * (seed % q) - r * (seed / q);
		seed += (seed > 0 ? 0 : m);
	}
}

LehmerSet::LehmerSet(long seed) : a(a), m(m){
	long q = m / a;
	long r = m % a;
	for(int i = 0; i < COUNT_DEFAULT; i++){
		streams.push_back(LehmerStream(a, m, seed));
		draws.push_back(0);
		seed = jumpMult256 * (seed % q) - r * (seed / q);
		seed += (seed > 0 ? 0 : m);
	}
}

double LehmerSet::Random(int streamIndex){
	draws[streamIndex]++;
	long draw = streams[streamIndex].Random();
	return ((double)draw / m);
}

long LehmerSet::jumpMult(long int s){
	long spacing = m /s;
	// TODO: Binary Exponentiation or similar
}
