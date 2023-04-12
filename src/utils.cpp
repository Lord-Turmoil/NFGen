#include "utils.h"

#include <cmath>
#include <cassert>


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


// Reference: https://blog.csdn.net/watqw/article/details/124331000
static std::vector<flp_t> _LagrangeLK(int k, flp_arr_t& x, flp_arr_t& y);
static void _LagrangeAux(int k, int i, int exp, double val, flp_arr_t& x, std::vector<double>& vec);
cont_poly_ptr LagrangeInterpolation(func_t F, flp_t a, flp_t b, int N, int k)
{
	// Get feasible points.
	flp_arr_ptr x_set = LinspaceFLP(a, b, N);
	assert(x_set);
	flp_arr_ptr y_set(new flp_arr_t());
	for (auto it : *x_set)
		y_set->push_back(F(it));
	
	// Calculate LK
	std::vector<std::vector<flp_t>> lk(x_set->size());
	for (int i = 0; i < lk.size(); i++)
		lk[i] = _LagrangeLK(i, *x_set, *y_set);

	cont_poly_ptr poly(new cont_poly_t(std::min((int)x_set->size(), k + 1), 0.0));
	for (int i = 0; i < lk.size(); i++)
	{
		for (int j = 0; j < poly->size(); j++)
			(*poly)[j] += (*y_set)[i] * lk[i][j];
	}

	return poly;
}

static std::vector<flp_t> _LagrangeLK(int k, flp_arr_t& x, flp_arr_t& y)
{
	flp_t denominator = 1.0;
	for (int i = 0; i < x.size(); i++)
	{
		if (i == k)
			continue;
		denominator *= x[k] - x[i];
	}

	std::vector<flp_t> ret(x.size(), 0);
	_LagrangeAux(k, 0, 0, 1.0, x, ret);
	for (int i = 0; i < ret.size(); i++)
		ret[i] = ret[i] / denominator;

	return ret;
}

static void _LagrangeAux(int k, int i, int exp, double val, flp_arr_t& x, std::vector<double>& vec)
{
	if (i >= x.size())
	{
		vec[exp] += val;
		return;
	}

	if (i == k)
	{
		_LagrangeAux(k, i + 1, exp, val, x, vec);
		return;
	}

	_LagrangeAux(k, i + 1, exp, -val * x[i], x, vec);
	_LagrangeAux(k, i + 1, exp + 1, val, x, vec);
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


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Evaluate polynomials.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
flp_t EvaluateDiscAsCont(disc_poly_ptr poly, fxp_t x)
{
	flp_t base = FXPsimFLP(x);
	flp_t ret = 0;

	for (auto it = poly->rbegin(); it != poly->rend(); it++)
		ret = ret * base + FXPsimFLP(it->first) * FXPsimFLP(it->second);

	return ret;
}

fxp_t EvaluateDiscAsDisc(disc_poly_ptr poly, fxp_t x)
{
	return FLPsimFXP(EvaluateDiscAsCont(poly, x));
}

flp_t EvaluateCont(cont_poly_ptr poly, flp_t x)
{
	flp_t ret = 0.0;

	for (auto it = poly->rbegin(); it != poly->rend(); it++)
		ret = ret * x + *it;

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