#include "utils.h"

#include <cmath>
#include <cassert>


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Float-fixed convert
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
#pragma region FLP_FXP

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

fxp_t float_to_fixed(flp_t flp, unsigned f)
{
	return _float_to_fixed_aux(&flp, f);
}

flp_t fixed_to_float(fxp_t fxp, unsigned f)
{
	return _fixed_to_float_aux(&fxp, f);
}

#pragma endregion


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Interpolation
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

#pragma region Interpolation

// actual valid value:  3.141592653589793;
static const flp_t PI = 3.141592653589793238462643383;

// Both Chebyshev and Lagrange interpolation will call this one at last.
// The wrapper function only decides the points to use.
static cont_poly_ptr _LagrangeInterpolation(func_t F, flp_arr_ptr x_set, flp_arr_ptr y_set, int k_bar);

// Reference: https://blog.csdn.net/watqw/article/details/124331000
static std::vector<flp_t> _LagrangeLK(int k, flp_arr_ptr x_set, flp_arr_ptr y_set);
static void _LagrangeAux(int k, int i, int exp, double val, flp_arr_ptr x_set, std::vector<double>& vec);


// Use Chebyshev selected points.
cont_poly_ptr ChebyshevInterpolation(func_t F, flp_t a, flp_t b, int k_bar)
{
	assert(b - a > 0.0);
	
	const int N = k_bar;

	flp_arr_ptr x_set = flp_arr_ptr(new flp_arr_t());
	flp_arr_ptr y_set = flp_arr_ptr(new flp_arr_t());
	
	for (int i = 1; i <= N; i++)
	{
		flp_t _x = std::cos(((2 * i - 1) * PI) / (2.0 * N));
		flp_t x = ((b + a) + (b - a) * _x) * 0.5;
		x_set->push_back(x);
		y_set->push_back(F(x));
	}

	return _LagrangeInterpolation(F, x_set, y_set, k_bar);
}

// Use all feasible points in [a, b].
cont_poly_ptr LagrangeInterpolation(func_t F, flp_t a, flp_t b, int k_bar)
{
	// Get feasible points.
	fxp_t left = float_to_fixed(a, fxp_f);
	fxp_t right = float_to_fixed(b, fxp_f);
	
	const int N = (int)(right - left + 1);
	assert(N > 0);

	// Readjust k_bar.
	k_bar = N - 1;

	flp_arr_ptr x_set = flp_arr_ptr(new flp_arr_t());
	flp_arr_ptr y_set = flp_arr_ptr(new flp_arr_t());
	
	for (fxp_t i = left; i <= right; i++)
	{
		flp_t x = fixed_to_float(i, fxp_f);
		x_set->push_back(x);
		y_set->push_back(F(x));
	}
	
	return _LagrangeInterpolation(F, x_set, y_set, k_bar);
}


static cont_poly_ptr _LagrangeInterpolation(func_t F, flp_arr_ptr x_set, flp_arr_ptr y_set, int k_bar)
{
	// Calculate LK
	std::vector<std::vector<flp_t>> lk(x_set->size());
	for (int i = 0; i < lk.size(); i++)
		lk[i] = _LagrangeLK(i, x_set, y_set);

	cont_poly_ptr poly(new cont_poly_t(std::min((int)x_set->size(), k_bar + 1), 0.0));
	for (int i = 0; i < lk.size(); i++)
	{
		for (int j = 0; j < poly->size(); j++)
			(*poly)[j] += (*y_set)[i] * lk[i][j];
	}

	return poly;
}

static std::vector<flp_t> _LagrangeLK(int k, flp_arr_ptr x_set, flp_arr_ptr y_set)
{
	flp_t denominator = 1.0;
	for (int i = 0; i < x_set->size(); i++)
	{
		if (i == k)
			continue;
		denominator *= (*x_set)[k] - (*x_set)[i];
	}

	std::vector<flp_t> ret(x_set->size(), 0);
	_LagrangeAux(k, 0, 0, 1.0, x_set, ret);
	for (int i = 0; i < ret.size(); i++)
		ret[i] = ret[i] / denominator;

	return ret;
}

static void _LagrangeAux(int k, int i, int exp, double val, flp_arr_ptr x_set, std::vector<double>& vec)
{
	if (i >= x_set->size())
	{
		vec[exp] += val;
		return;
	}

	if (i == k)
	{
		_LagrangeAux(k, i + 1, exp, val, x_set, vec);
		return;
	}

	_LagrangeAux(k, i + 1, exp, -val * (*x_set)[i], x_set, vec);
	_LagrangeAux(k, i + 1, exp + 1, val, x_set, vec);
}

#pragma endregion


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Linspace
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

#pragma region LinearSpace

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
		t -= 2;
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

#pragma endregion


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Evaluate polynomials.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

#pragma region Evalutation

flp_t EvaluateDiscAsCont(disc_poly_ptr poly, fxp_t x)
{
	flp_t base = fixed_to_float(x, fxp_f);
	flp_t ret = 0;

	for (auto it = poly->rbegin(); it != poly->rend(); it++)
	{
		ret *= base;
		ret += fixed_to_float(it->first, fxp_f) * fixed_to_float(it->second, fxp_f);
	}

	return ret;
}

fxp_t EvaluateDiscAsDisc(disc_poly_ptr poly, fxp_t x)
{
	return float_to_fixed(EvaluateDiscAsCont(poly, x), fxp_f);
}

flp_t EvaluateCont(cont_poly_ptr poly, flp_t x)
{
	flp_t ret = 0.0;

	for (auto it = poly->rbegin(); it != poly->rend(); it++)
		ret = ret * x + *it;

	return ret;
}

#pragma endregion


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
