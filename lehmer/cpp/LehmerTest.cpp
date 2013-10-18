#include "LehmerPRNG.h"

#include <iostream>

using namespace std;

/** Magic Constants for checking behavior **/
#define CHECK 399268537 // seed of 1, 10k calls later for default a, m on stream
#define AJ256  22925     // for a=48271, m=2147483647, precomputed jump multiplier

int main(int argc, char** argv){

	// Testing LehmerStream
	cout << "Testing LehmerStream Behavior" << endl;
	// Seed with 1
	LehmerStream stream(1);
	cout << "LehmerStream initialized with seed" << stream.getSeed() << endl;
	// Draw 10k times
	for(int i = 0; i < 10000; i++){
		stream.random();
	}
	cout << "10000 calls later, state/seed is " << stream.getSeed() << endl;
	// Test for correct stream state
	if(stream.getSeed() != CHECK){
		cout << "Stream was not on the proper state; expected " << CHECK << " found " << stream.getSeed() << endl;
		return 1;
	}
	// Stream okay; continue
	cout << "Stream behaving correctly." << endl;

	cout << endl;

	// Testing LehmerSet
	cout << "Testing LehmerSet Behavior" << endl;
	// Seed with 1
	LehmerSet set(1);
	cout << "LehmerSet initialized with seed 1" << endl;
	// Test state of stream 1
	if(set.getSeed(1) != AJ256){
		cout << "Stream 1 of set not on the proper state; expected " << AJ256 << " found " << set.getSeed(1) << endl;
		return 1;
	}
	cout << "Stream 1 at state " << set.getSeed(1) << endl;
	// Set okay; continue
	cout << "Set behaving correctly." << endl;

	cout << endl;

	cout << "LehmerPRNG library functioning as expected." << endl;

	return 0;
}
