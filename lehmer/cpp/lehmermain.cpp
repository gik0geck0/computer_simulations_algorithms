
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "lehmerstream.h"

// TODO: Protect against redefining main
int main(int argc, char** argv) {
//    if (argc < 5) {
//        printf("Required args: a j s x0 n\n");
//        exit(1);
//    }
    lehmer::LehmerStream ls(1);
    for(int i = 0; i < 100; i++){
    	std::cout << ls.Random() << std::endl;
    }
    return 0;
}
