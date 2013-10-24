/*
 * LehmerPRNG.cpp
 *
 *  Created on: Oct 17, 2013
 *      Author: jack
 */

#include "LehmerPRNG.h"

#include <stdint.h>

/* + LehmerStream */

LehmerStream::LehmerStream(long seed, long a, long m)
	: a(a), m(m), x(seed), q(m/a), r(m%a){
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

long LehmerStream::getSeed() const{
	return x;
}

/* - LehmerStream */

/* + LehmerSet */

LehmerSet::LehmerSet(long seed, int streamCount, long a, long m) : a(A_DEFAULT), m(M_DEFAULT){
	long q = m / a;
	long r = m % a;
	long jumpMult;
	// Select the jumpMultiplier
	switch(streamCount){
	case 128:
		jumpMult = jumpMult128;
		break;
	case 256:
		jumpMult = jumpMult256;
		break;
	case 512:
		jumpMult = jumpMult512;
		break;
	case 1024:
		jumpMult = jumpMult1024;
		break;
	default:
		jumpMult = calcJumpMult(a, streamCount, m);
		break;
	}
	// Initialize the streamss
	for(int i = 0; i < streamCount; i++){
		streams.push_back(LehmerStream(seed, a, m));
		draws.push_back(0);
		seed = jumpMult * (seed % q) - r * (seed / q);
		seed += ((seed > 0) ? 0 : m);
	}
}

double LehmerSet::random(int streamIndex){
	draws[streamIndex]++;
	return streams[streamIndex].random();
}

bool LehmerSet::validate(int streamIndex) const{
	return draws[streamIndex] < m / streams.size();
}

long LehmerSet::getSeed(int streamIndex) const{
	return streams[streamIndex].getSeed();
}

void LehmerSet::putSeed(int streamIndex, long seed){
	streams[streamIndex].putSeed(seed);
}

long LehmerSet::calcJumpMult(long a, int streamCount, long m){
	long currentJ = m / streamCount;
	long jumpMult = modular_pow(a, currentJ, m);
	while(!isModulusCompatible(jumpMult, m)){
		jumpMult = modular_pow(a, --currentJ, m);
	}
	return jumpMult;
}

// Binary modular exponentiation. Pseudo code taken from
// http://en.wikipedia.org/wiki/Modular_exponentiation#Right-to-left_binary_method
long LehmerSet::modular_pow(long base, long exponent, long modulus) {
	int_least64_t result = 1;
	int_least64_t hiBase = base;
	int_least64_t hiExponent = exponent;
	int_least64_t hiModulus = modulus;
	while(hiExponent > 0){
		if(hiExponent % 2 == 1){
			result = (result * hiBase) % hiModulus;
		}
		hiExponent >>= 1;
		hiBase = (hiBase * hiBase) % hiModulus;
	}
	return result;
}

/* - LehmerSet */


