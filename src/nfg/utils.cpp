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
** Linspace
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

#pragma region LinearSpace

int GetFeasiblePointNumber(flp_t a, flp_t b)
{
	return std::max(0, (int)std::floor((b - a) * (double)((uint64_t)1 << fxp_f)));
}

/*
 * There is no legality check by now...
 */
flp_arr_ptr LinspaceFLP(flp_t a, flp_t b, int n)
{
	fxp_t fxpa = float_to_fixed(a, fxp_f);
	fxp_t fxpb = float_to_fixed(b, fxp_f);
	n = std::min((int)(fxpb - fxpa), n);
	if (n <= 0)
		return nullptr;

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
	if (flp_arr == nullptr)
		return nullptr;

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
	return EvaluateDiscAsCont(*poly, x);
}

flp_t EvaluateDiscAsCont(const disc_poly_t& poly, fxp_t x)
{
	flp_t base = fixed_to_float(x, fxp_f);
	flp_t ret = 0;

	for (auto it = poly.rbegin(); it != poly.rend(); it++)
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

fxp_t EvaluateDiscAsDisc(const disc_poly_t& poly, fxp_t x)
{
	return float_to_fixed(EvaluateDiscAsCont(poly, x), fxp_f);
}

flp_t EvaluateCont(cont_poly_ptr poly, flp_t x)
{
	return EvaluateCont(*poly, x);
}

flp_t EvaluateCont(const cont_poly_t& poly, flp_t x)
{
	flp_t ret = 0.0;

	for (auto it = poly.rbegin(); it != poly.rend(); it++)
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
