// TODO: #ifndef

#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "lehmerset.h"

using namespace lehmer;

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

LehmerSet::LehmerSet(long seed){
	long q = m / a;
	long r = m % a;
	for(int i = 0; i < streamcount; i++){
		streams.push_back(LehmerStream(a, m, seed));
		draws.push_back(0);
		seed = jumpMult * (seed % q) - r * (seed / q);
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

//typedef struct lehmerset {
//     int64_t a;
//     int64_t m;
//     int64_t streams;
//     int64_t* seeds;
//} lehmerset;
//
//    // Initialize the seeds in the lehmer-set
//    // TODO: Explicit precondition/postcondition
//    void plant_seeds(struct lehmerset* state,  int64_t x0) {
//        // TODO: Protect against leak in seeds field
//        state->seeds = (int64_t*) malloc(sizeof(int64_t) * state->streams);
//        state->seeds[0] = x0;
//        int64_t q = state->m / state->a;
//        int64_t r = state->m % state->a;
//        for (int64_t i=1; i < state->streams; i++) {
//            //printf("Calculating seed %li\n", i);
//            //printf("R: %li\n", r);
//            //printf("Q: %li\n", q);
//            state->seeds[i] = state->a * ( state->seeds[i-1] % q) - r * (state->seeds[i-1] / q);
//
//            if (state->seeds[i] <= 0) {
//                state->seeds[i] += state->m;
//            }
//        }
//    }
//
//    int64_t calc_aj(int64_t a, int64_t j) {
//        // Calc a**j mod m
//    	return 0;
//    }
//
