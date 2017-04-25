#ifndef __Random_H__
#define __Random_H__

// Utility class for generating random integers and floats.
class Random {
private:
	static Random* instance;

	Random();

public:
	// Gets the singleton instance.
	static Random* Get();

	// Gets a random float between [0, 1].
	float NextFloat();

	// Gets a random integer between [0, maxExc).
	int NextInt(int maxExc);

	// Gets a random integer between [minInc, maxExc).
	int NextInt(int minInc, int maxExc);
};

#endif
