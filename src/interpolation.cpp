#include "interpolation.h"

#include <cmath>
#include <assert.h>


// actual valid value:  3.141592653589793;
static const flp_t PI = 3.141592653589793238462643383;

// Both Chebyshev and Lagrange interpolation will call this one at last.
// The wrapper function only decides the points to use.
static cont_poly_ptr _LagrangeInterpolation(FuncPtr F, flp_arr_ptr x_set, flp_arr_ptr y_set, int k_bar);

// Reference: https://blog.csdn.net/watqw/article/details/124331000
static std::vector<flp_t> _LagrangeLK(int k, flp_arr_ptr x_set, flp_arr_ptr y_set);
static void _LagrangeAux(int k, int i, int exp, double val, flp_arr_ptr x_set, std::vector<double>& vec);


// Use Chebyshev selected points.
cont_poly_ptr ChebyshevInterpolation(FuncPtr F, flp_t a, flp_t b, int k_bar)
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
		y_set->push_back(F->Evaluate(x));
	}

	return _LagrangeInterpolation(F, x_set, y_set, k_bar);
}

// Use all feasible points in [a, b].
cont_poly_ptr LagrangeInterpolation(FuncPtr F, flp_t a, flp_t b, int k_bar)
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
		y_set->push_back(F->Evaluate(x));
	}

	return _LagrangeInterpolation(F, x_set, y_set, k_bar);
}


static cont_poly_ptr _LagrangeInterpolation(FuncPtr F, flp_arr_ptr x_set, flp_arr_ptr y_set, int k_bar)
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
