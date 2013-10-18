#include "lehmerstream.h"

LehmerStream::LehmerStream(long a, long m, long seed)
	: a(a), m(m), x(seed), q(m/a), r(m%a){
	// TODO: If r > q, signal a problem
}

LehmerStream::LehmerStream(long seed)
	: a(A_DEFAULT), m(M_DEFAULT), x(seed), q(m/a), r(m%a){
	// TODO: If r > q, signal a problem
}

double LehmerStream::Random(){
	long t = a * (x % q) - r * (x / q);
	x = t > 0 ? t : t + m;
	return ((double)x / m);
}

void LehmerStream::putSeed(long seed){
	x = seed;
}

long LehmerStream::getSeed(){
	return x;
}
