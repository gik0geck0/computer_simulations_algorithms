#include <vector>

#include "lehmerstream.h"

namespace lehmer{

	class LehmerSet {
	public:
		// Precondition: m is a large prime, a is modulus-compatible with m, 0 < seed < m
		// This requires the intensive calculation of a^j%m
		LehmerSet(long a, long m, int streamcount, long seed);
		// Precondition: m is a large prime, a is modulus-compatible with m, 0 < seed < m
		// The jump multiplier is provided explicitly, allowing it to be computed
		// offline rather than during runtime
		LehmerSet(long a, long m, long aj, long seed);
		// Generate a stream set with 256 streams using precalculated a-, m-values
		// Fast setup, but potentially limited utility for pRNG-hungry applications
		// seed is used to seed the streams
		LehmerSet(long seed);
		// Return a value on the interval (0.0, 1.0)
		double Random(int streamIndex);
		// True if the stream has not overrun into another stream or wrapped around into itself
		bool validate(int streamIndex);
	private:
		const static long jumpMult256 = 22925L;
        const static long A_DEFAULT = 48271;
        const static long M_DEFAULT = 2147483647L;
		const long a;
		const long m;
		const int streamcount;
		std::vector<LehmerStream> streams;
		std::vector<int> draws;
		long jumpMult(long s);
	};

}
