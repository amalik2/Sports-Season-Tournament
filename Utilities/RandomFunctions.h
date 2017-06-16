#pragma once

namespace RandomFunctions
{

	// Returns a random integer from min to max
	int randint(int min, int max);

	// Returns true if an event with the given chance % occurs
	bool randomChance(int chance);

}