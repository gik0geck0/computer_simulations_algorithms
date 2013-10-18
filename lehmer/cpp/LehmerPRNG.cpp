/*
 * LehmerPRNG.cpp
 *
 *  Created on: Oct 17, 2013
 *      Author: jack
 */

#include "LehmerPRNG.h"

/* + LehmerStream */

LehmerStream::LehmerStream(long a, long m, long seed)
	: a(a), m(m), x(seed), q(m/a), r(m%a){
	// TODO: If r > q, signal a problem
}

LehmerStream::LehmerStream(long seed)
	: a(A_DEFAULT), m(M_DEFAULT), x(seed), q(m/a), r(m%a){
	// TODO: If r > q, signal a problem
}

double LehmerStream::random(){
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

/* - LehmerStream */

/* + LehmerSet */

LehmerSet::LehmerSet(long seed) : a(A_DEFAULT), m(M_DEFAULT){
	long q = m / a;
	long r = m % a;
	for(int i = 0; i < COUNT_DEFAULT; i++){
		streams.push_back(LehmerStream(a, m, seed));
		draws.push_back(0);
		seed = jumpMult256 * (seed % q) - r * (seed / q);
		seed += (seed > 0 ? 0 : m);
	}
}

double LehmerSet::random(int streamIndex){
	draws[streamIndex]++;
	return streams[streamIndex].random();
}

bool LehmerSet::validate(int streamIndex){
	return draws[streamIndex] < m / streams.size();
}

long LehmerSet::getSeed(int streamIndex){
	return streams[streamIndex].getSeed();
}

void LehmerSet::putSeed(int streamIndex, long seed){
	streams[streamIndex].putSeed(seed);
}
/* - LehmerSet */


