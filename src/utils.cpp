#include "utils.h"

#include <cmath>

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Float-fixed convert.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static inline uint64_t _float_to_fixed_aux(double* value, unsigned n)
{
	return (uint64_t)(std::round((*value) * ((uint64_t)1 << n)));
}

static inline double _fixed_to_float_aux(uint64_t* value, unsigned n)
{
	return (double)((int64_t)(*value)) / (double)((uint64_t)1 << n);
}

void float_to_fixed(double* src, uint64_t* dst, size_t size, unsigned n)
{
	for (size_t i = 0; i < size; i++)
		*(dst++) = _float_to_fixed_aux(src++, n);
}

void fixed_to_float(uint64_t* src, double* dst, size_t size, unsigned n)
{
	for (size_t i = 0; i < size; i++)
		*(dst++) = _fixed_to_float_aux(src++, n);
}
