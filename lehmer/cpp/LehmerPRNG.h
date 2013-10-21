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
	long getSeed() const;
	// Set the current state of the stream to seed
	// The caller is responsible for verifying the validity of the seed
	void putSeed(long seed);
private:
	long a;
	long m;
	long q;
	long r;
	long x;
	const static long A_DEFAULT = 48271;
	const static long M_DEFAULT = 2147483647L;
};

class LehmerSet {
public:
	// Generate a stream set with streamCount streams using specified jumpMult.
	// If streamCount is one of {128, 256, 512, 1024}, a precalculated jump
	// multiplier is used. Otherwise, the jump multiplier must be calculated
	// at runtime; this is very slow and not recommended.
	// See calcJumpMult for offline calculation of jump multipliers.
	LehmerSet(long seed, int streamCount = 256, long jumpMult = jumpMult256);
	// Return a value from Uniform(0.0, 1.0)
	double random(int streamIndex);
	// True if the stream has not overrun into another stream or wrapped around into itself
	// Precondition: putSeed() has not been called on this StreamSet since initialization
	// If the precondition is not met, validity of streams is unknown (behavior undefined)
	bool validate(int streamIndex) const;
	// Current state of the stream at an index
	long getSeed(int streamIndex) const;
	// Set the state of the stream at an index.
	// Postcondition: Validity of streams is no longer known
	void putSeed(int streamIndex, long seed);
	// Calculate jump multiplier for the given parameters. On a 32 bit machine,
	// some trickery is used to fake 64 bit intermediaries and thus this method may be
	// very slow. If possible, calculate the jump multiplier once offline and then use
	// a parameterized ctor for LehmerSet.
	static long calcJumpMult(long a, int streamCount, long m);
private:
	const static long jumpMult128 = 40509L;
	const static long jumpMult256 = 22925L;
	const static long jumpMult512 = 44857L;
	const static long jumpMult1024 = 97070L;
	const static long A_DEFAULT = 48271;
	const static long M_DEFAULT = 2147483647L;
	const static long COUNT_DEFAULT = 256;
	long a;
	long m;
	std::vector<LehmerStream> streams;
	std::vector<int> draws;
	static long modular_pow(long base, long exponent, long modulus);
	static bool isModulusCompatible(long ajmm, long m) {return (m % ajmm) < (m / ajmm);}
};


#endif /* LEHMERPRNG_H_ */
