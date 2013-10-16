#/usr/bin/env python3

import sys
from functools import partial

def lehmer_func(a, m, x):
    return a*x % m

# As it turns out, in python 3, there is no limit to integer size
#   Despite this, all the calculations will be done in non-overflow
#   sort, to ease the ability to port the code to other languages
class StreamedLehmerGen:

    def __init__(self, a, m, j, streams, x0, aj=None):
        if aj is None:
            self.aj = get_jump_mult(a, j, m)
        else:
            self.aj = aj
        self.a = a
        self.m = m
        #self.j = j
        self.streams = streams
        self.seeds = [0]*streams
        self.my_gen = partial(lehmer_func, self.a, self.m)
        self.plant_seeds(x0)

    def plant_seeds(self, first_seed):
        self.seeds[0] = first_seed
        Q = (self.m // self.a)
        R = (self.m % self.a)
        for i in range(1,self.streams):
            x = self.a * (self.seeds[i-1] % Q) - R * (self.seeds[i-1] // Q)

            if x > 0:
                self.seeds[i] = x
            else:
                self.seeds[i] = x + self.m
        #for i in range(0, self.streams-1):
        #    print("Seed %i is %i" % (i, self.seeds[i]))

    def get_random(self, stream):
        self.seeds[stream] = self.my_gen(self.seeds[stream])
        return self.seeds[stream]

def get_jump_mult(a, j, m):
    result = 1
    # Binary modular exponentiation. Pseudo code taken from http://en.wikipedia.org/wiki/Modular_exponentiation#Right-to-left_binary_method
    while (j > 0):
        if (j % 2 == 1):
           result = (result * a) % m;

        j = j >> 1
        a = (a*a) % m

    return result;

# Pick a and m for 64-bit amazingness
if len(sys.argv) < 5:
    print("Required args: a j s x0 n")

a = int(sys.argv[1])

# 64-bit generation isn't feasible, since a full-period multiplier hasn't been found
#m = 2**63-1
m = 2**31-1

if '--aj' in sys.argv:
    index = 0
    for arg in sys.argv:
        if arg == '--aj':
            if index > len(sys.argv):
                print("Please provide the aj after '--aj'")
            aj = int(sys.argv[index+1])
            break
        index += 1
    stream_gen = StreamedLehmerGen(a, m, int(sys.argv[2]), int(sys.argv[3]), int(sys.argv[4]), aj)
else:
    stream_gen = StreamedLehmerGen(a, m, int(sys.argv[2]), int(sys.argv[3]), int(sys.argv[4]))


if '--seeds' in sys.argv:
    for i in range(0, int(sys.argv[3])):
        print("Seed %i: %i" % (i, stream_gen.seeds[i]))
else:
    for j in range(0, int(sys.argv[3])):
        print("Stream %i" % j)
        for i in range(0, int(sys.argv[5])):
            print(stream_gen.get_random(j))
