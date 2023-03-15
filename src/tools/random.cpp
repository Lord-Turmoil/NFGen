#include "random.h"

#include <ctime>
#include <cstdlib>

static inline double _Random()
{
	return (double)rand() / (double)RAND_MAX;
}

double Random()
{
	return _Random();
}

double Random(double upper)
{
	return _Random() * upper;
}

double Random(double lower, double upper)
{
	return lower + _Random() * (upper - lower);
}