#include <cstdio>			// For NULL macro
#include <time.h>			// For generating random numbers
#include <stdlib.h>

#include "Random.h"

Random* Random::instance = NULL;

Random* Random::Get() {
	if (instance == NULL)
		instance = new Random();

	return instance;
}

Random::Random() { 
	srand(time(NULL));
}

float Random::NextFloat() {
	return rand() / (float) RAND_MAX;
}

int Random::NextInt(int maxExc) {
	return this->NextInt(0, maxExc);
}

int Random::NextInt(int minInc, int maxExc) {
	return (rand() % (maxExc - minInc)) + minInc;
}