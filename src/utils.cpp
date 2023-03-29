#include "utils.h"

#include <cmath>

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Float-fixed convert
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static inline uint64_t _float_to_fixed_aux(double* value, unsigned f)
{
	return (uint64_t)(std::round((*value) * ((uint64_t)1 << f)));
}

static inline double _fixed_to_float_aux(uint64_t* value, unsigned f)
{
	return (double)((int64_t)(*value)) / (double)((uint64_t)1 << f);
}

void float_to_fixed(double* src, uint64_t* dst, size_t size, unsigned f)
{
	for (size_t i = 0; i < size; i++)
		*(dst++) = _float_to_fixed_aux(src++, f);
}

void fixed_to_float(uint64_t* src, double* dst, size_t size, unsigned f)
{
	for (size_t i = 0; i < size; i++)
		*(dst++) = _fixed_to_float_aux(src++, f);
}

uint64_t FLPsimFXP(double flp)
{
	return _float_to_fixed_aux(&flp, fxp_f);
}

double FXPsimFLP(uint64_t fxp)
{
	return _fixed_to_float_aux(&fxp, fxp_f);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Interpolation
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
cont_poly_ptr ChebyInterpolation(func_t F, double a, double b, int k)
{
	return nullptr;
}

cont_poly_ptr LagrangeInterpolation(func_t F, double a, double b, int k)
{
	return nullptr;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Linspace
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

/*
 * There is no legality check by now...
 */
flp_arr_ptr LinspaceFLP(double a, double b, int n)
{
	flp_arr_ptr arr(new flp_arr_t(n));

	double step = (b - a) / (n - 1);
	double left = a;
	double right = b;

	int i = 0;
	int t = n;
	while (t >= 2)
	{
		(*arr)[i] = left;
		(*arr)[n - i - 1] = right;
		left += step;
		right -= step;
		i++;
		n -= 2;
	}
	if (t > 0)
		(*arr)[i] = (left + right) / 2;

	return arr;
}

fxp_arr_ptr LinspaceFXP(double a, double b, int n)
{
	fxp_arr_ptr arr(new fxp_arr_t(n));
	auto flp_arr = LinspaceFLP(a, b, n);

	float_to_fixed(flp_arr->data(), arr->data(), n, fxp_f);

	return arr;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Evaluate polynomials.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
uint64_t Evaluate(disc_poly_ptr poly, uint64_t x)
{
	uint64_t var = 1;
	uint64_t ret = 0;
	
	for (auto cof : *poly)
	{
		ret += cof * var;
		var *= x;
	}

	return ret;
}

double Evaluate(cont_poly_ptr poly, double x)
{
	double var = 1.0;
	double ret = 0.0;

	for (auto cof : *poly)
	{
		ret += cof * var;
		var *= x;
	}

	return ret;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Utils
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
double Distance(double x, double y)
{
	if (std::abs(x) > SOFT_ZERO)
		return std::abs(x - y) / std::abs(x);
	else
		return std::abs(x - y);
}