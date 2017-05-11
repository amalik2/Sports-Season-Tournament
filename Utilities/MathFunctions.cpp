#include "MathFunctions.h"

float ratio(float numerator, float denominator) {
	if (denominator == 0)
		return numerator;

	return numerator / denominator;
}
