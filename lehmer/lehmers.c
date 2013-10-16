// TODO: #ifndef

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct lehmerset {
     int64_t a;
     int64_t aj;
     int64_t m;
     int64_t streams;
     int64_t* seeds;
} lehmerset;

// Initialize the seeds in the lehmer-set
// TODO: Explicit precondition/postcondition
void plant_seeds(struct lehmerset* state,  int64_t x0) {
    // TODO: Protect against leak in seeds field
    state->seeds = (int64_t*) malloc(sizeof(int64_t) * state->streams);
    state->seeds[0] = x0;
    int64_t q = state->m / state->a;
    int64_t r = state->m % state->a;
    for (int64_t i=1; i < state->streams; i++) {
        //printf("Calculating seed %li\n", i);
        //printf("R: %li\n", r);
        //printf("Q: %li\n", q);
        state->seeds[i] = state->a * ( state->seeds[i-1] % q) - r * (state->seeds[i-1] / q);

        if (state->seeds[i] <= 0) {
            state->seeds[i] += state->m;
        }
    }
}

int64_t modular_pow(int64_t base, int64_t exponent, int64_t modulus) {
    __uint128_t hp_result;
    __uint128_t hp_base = (__uint128_t) base;
    __uint128_t hp_exponent = (__uint128_t) exponent;
    __uint128_t hp_modulus = (__uint128_t) modulus;

    // Binary modular exponentiation. Pseudo code taken from http://en.wikipedia.org/wiki/Modular_exponentiation#Right-to-left_binary_method
    hp_result = 1;
    while (hp_exponent > 0) {
        if (hp_exponent % 2 == 1)
           hp_result = (hp_result * hp_base) % hp_modulus;

        hp_exponent = hp_exponent >> 1;
        hp_base = (hp_base * hp_base) % hp_modulus;
    }
    return (int64_t) hp_result;
}

int64_t calc_aj(int64_t a, int64_t j, int64_t m) {
    int64_t calced_aj = modular_pow(a, j, m);
    printf("Calculated AJ to be %li\n", calced_aj);
    return calced_aj;
}

// TODO: Protect against redefining main
int main(int argc, char** argv) {
    if (argc < 5) {
        printf("Required args: a j s x0 n\n");
        exit(1);
    }

    /*
    for (int i=0; i < argc; i++) {
        printf("Arg %i: %s\n", i, argv[i]);
    }
    */

    lehmerset state = {
        .a = atol(argv[1]),
        .m = LONG_MAX,
        //.aj = calc_aj(state.a, atol(argv[2]), state.m),
        .streams = atol(argv[3]),
        .seeds = NULL
    };
    printf("Made state.\n");

    int has_aj = 0;
    int show_seeds_only = 0;
    for (int i=6; i < argc; i++) {
        printf("Is %s '--aj'?\n", argv[i]);
        if (strcmp(argv[i], "--aj") == 0) {
            //printf("Yes, %s is '--aj'?\n", argv[i]);
            has_aj = 1;
            if (argc <= i) {
                printf("After '--aj', the next argument must be the jump multiplier!\n");
            }
            state.aj = atol(argv[i+1]);

            // Skip the next arg: its a number
            i++;
        }
        if (strcmp(argv[i], "--seeds")) {
            show_seeds_only = 1;
        }
    }

    if (!has_aj) {
        printf("No AJ provided. Finding one based off the number of streams\n");
        state.aj = calc_aj(state.a, atol(argv[2]), state.m);
    }


    /*
    printf("A is %li\n", state.a);
    printf("M is %li\n", state.m);
    printf("X0 is %li\n", atol(argv[4]));
    */

    plant_seeds(&state, atol(argv[4]));

    if (show_seeds_only) {
        for (int64_t i=0; i < state.streams; i++) {
            printf("Seed %li: %li\n", i, state.seeds[i]);
        }
    }
}
