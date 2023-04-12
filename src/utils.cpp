#include "utils.h"

#include <cmath>

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Float-fixed convert
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static inline fxp_t _float_to_fixed_aux(double* value, unsigned f)
{
	return (fxp_t)(std::round((*value) * ((fxp_t)1 << f)));
}

static inline double _fixed_to_float_aux(fxp_t* value, unsigned f)
{
	return (double)((int64_t)(*value)) / (flp_t)((fxp_t)1 << f);
}

void float_to_fixed(flp_t* src, fxp_t* dst, size_t size, unsigned f)
{
	for (size_t i = 0; i < size; i++)
		*(dst++) = _float_to_fixed_aux(src++, f);
}

void fixed_to_float(fxp_t* src, flp_t* dst, size_t size, unsigned f)
{
	for (size_t i = 0; i < size; i++)
		*(dst++) = _fixed_to_float_aux(src++, f);
}

fxp_t FLPsimFXP(flp_t flp)
{
	return _float_to_fixed_aux(&flp, fxp_f);
}

flp_t FXPsimFLP(fxp_t fxp)
{
	return _fixed_to_float_aux(&fxp, fxp_f);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Interpolation
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
cont_poly_ptr ChebyInterpolation(func_t F, flp_t a, flp_t b, int k)
{
	return nullptr;
}

cont_poly_ptr LagrangeInterpolation(func_t F, flp_t a, flp_t b, int k)
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
flp_arr_ptr LinspaceFLP(flp_t a, flp_t b, int n)
{
	flp_arr_ptr arr(new flp_arr_t(n));

	flp_t step = (b - a) / (n - 1);
	flp_t left = a;
	flp_t right = b;

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

fxp_arr_ptr LinspaceFXP(flp_t a, flp_t b, int n)
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
fxp_t Evaluate(disc_poly_ptr poly, fxp_t x)
{
	fxp_t var = 1;
	fxp_t ret = 0;
	
	for (auto cof : *poly)
	{
		ret += cof.first * cof.second * var;
		var *= x;
	}

	return ret;
}

flp_t Evaluate(cont_poly_ptr poly, flp_t x)
{
	flp_t var = 1.0;
	flp_t ret = 0.0;

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
flp_t Distance(flp_t x, flp_t y)
{
	if (std::abs(x) > SOFT_ZERO)
		return std::abs(x - y) / std::abs(x);
	else
		return std::abs(x - y);
}