

namespace lehmer{
    class LehmerStream{
    public:
    	// Construct a LehmerStream with a = 48271, m = 2147483647, seeded to seed
        LehmerStream(long seed);
        // Construct a LehmerStream with custom a and m values
        // The caller is responsible for verifying the modulus compatibility,
        // etc., of the passed values
        LehmerStream(long a, long m, long seed);
        // Return a random value uniformly distributed on the interval (0.0, 1.0)
        double Random();
        // Set the current state of the stream to seed
        // The caller is responsible for verifying the validity of the seed
        void putSeed(long seed);
        // Get the current state of the stream, without advancing it
        long getSeed();
    private:
        const long a;
        const long m;
        const long q;
        const long r;
        long x;
        const static long A_DEFAULT = 48271; // 16807?
        const static long M_DEFAULT = 2147483647L;
    };
}
