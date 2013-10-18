/*
 * LehmerPRNG.h
 *
 *  Created on: Oct 17, 2013
 *      Author: jack
 */

#ifndef LEHMERPRNG_H_
#define LEHMERPRNG_H_

#include <vector>

class LehmerStream{
public:
	// Construct a LehmerStream with a = 48271, m = 2147483647, seeded to seed
	LehmerStream(long seed);
	// Construct a LehmerStream with custom a and m values
	// The caller is responsible for verifying the modulus compatibility,
	// etc., of the passed values
	LehmerStream(long a, long m, long seed);
	// Return a random value uniformly distributed on the interval (0.0, 1.0)
	double random();
	// Get the current state of the stream, without advancing it
	long getSeed();
	// Set the current state of the stream to seed
	// The caller is responsible for verifying the validity of the seed
	void putSeed(long seed);
private:
	long a;
	long m;
	long q;
	long r;
	long x;
	const static long A_DEFAULT = 48271; // 16807?
			const static long M_DEFAULT = 2147483647L;
};

class LehmerSet {
public:
	// Generate a stream set with 256 streams using precalculated a-, m-values
	// Fast setup, but potentially limited utility for pRNG-hungry applications
	// seed is used to seed the streams
	LehmerSet(long seed);
	// Return a value on the interval (0.0, 1.0)
	double random(int streamIndex);
	// True if the stream has not overrun into another stream or wrapped around into itself
	// Precondition: putSeed() has not been called on this StreamSet since initialization
	// If the precondition is not met, validity of streams is unknown (behavior undefined)
	bool validate(int streamIndex);
	// Current state of the stream at an index
	long getSeed(int streamIndex);
	// Set the state of the stream at an index.
	// Postcondition: Validity of streams is no longer known
	void putSeed(int streamIndex, long seed);
private:
	const static long jumpMult256 = 22925L;
	const static long A_DEFAULT = 48271;
	const static long M_DEFAULT = 2147483647L;
	const static long COUNT_DEFAULT = 256;
	long a;
	long m;
	std::vector<LehmerStream> streams;
	std::vector<int> draws;
};


#endif /* LEHMERPRNG_H_ */
