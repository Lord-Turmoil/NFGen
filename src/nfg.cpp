#include "nfg.h"
#include "global.h"
#include "utils.h"

#include <cmath>
#include <cassert>


static int m_cnt = 0;

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Algorithm 1. FitPiecewise
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

#pragma region FitPiecewise

static void _AddPoly(disc_poly_ptr poly, flp_t a, flp_t b);
static void _FitPiecewiseAux(func_t F, flp_t a, flp_t b, int k);

void FitPiecewise(func_t F, flp_t a, flp_t b, int k)
{
	m_cnt = 0;
	disc_polys.clear();
	divisions.clear();
	_FitPiecewiseAux(F, a, b, k);
}

/*
 * Add a polynomial and the segment it represents.
 */
static void _AddPoly(disc_poly_ptr poly, flp_t a, flp_t b)
{
	disc_polys.emplace_back(poly);
	if (divisions.empty())
		divisions.push_back(a);
	divisions.push_back(b);
}

/*
 * This will be called recursively, and the segments in [a, b] are
 * calculated strictly from left to right.
 */
static void _FitPiecewiseAux(func_t F, flp_t a, flp_t b, int k)
{
	disc_poly_ptr poly = FitOnePiece(F, a, b, k);
	if (poly)
	{
		_AddPoly(poly, a, b);
	}
	else
	{
		double mid = (a + b) / 2;
		FitPiecewise(F, a, mid, k);
		FitPiecewise(F, mid, b, k);

		if (m_cnt++ > m_MAX)
			return;
	}

	int i = 0;
	while (true)
	{
		if (i + 2 >= (int)divisions.size())
			break;

		a = divisions[i];
		b = divisions[i + 2];

		poly = FitOnePiece(F, a, b, k);
		if (poly)
		{
			// Replace p[i] and p[i + 1] with p[k]
			
			// Merge the segment?
			divisions.erase(divisions.begin() + (i + 1));

			// Erase p[i], then it points at p[i + 1]
			auto it = disc_polys.erase(disc_polys.begin() + i);
			*it = poly;	// make p[i + 1] p[k]
		}
		else
			i++;
	}
}

#pragma endregion


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Algorithm 2. FitOnePiece
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

#pragma region FitOnePiece

static void ExpandPoly(disc_poly_ptr poly, int k);
static bool EvaluatePrecision(func_t F, disc_poly_ptr poly, fxp_arr_ptr points);

static disc_poly_ptr FitOnePiece(func_t F, flp_t a, flp_t b, int k)
{
	/* Step 1. Constrain k */
	int k_bar = ConstrainK(a, b, k);
	
	/* Step 2. Fit best polynomial in FLP space. */

	/*
	 * Maximum representable points number.
	 * N = (b - a) / (2 ^ -fxp_f) = (b - a) * (2 ^ fxp_f)
	 */
	int N = (int)std::floor((b - a) * (double)((uint64_t)1 << fxp_f));
	
	cont_poly_ptr cont_poly(nullptr);
	if (N > k_bar + 1)
		cont_poly = ChebyshevInterpolation(F, a, b, k);
	else
	{
		k_bar = N - 1;
		cont_poly = LagrangeInterpolation(F, a, b, k);
	}

	assert(cont_poly);	// must not be nullptr

	/* Step 3. Convert to FXP space. */
	disc_poly_ptr disc_poly = ScalePoly(cont_poly, a, b);

	/* Step 4. Further reducing the rounding precision loss. */
	disc_poly = ResidualBoosting(disc_poly, F, a, b);

	ExpandPoly(disc_poly, k);

	/* Step 5. Check accuracy and return. */
	int sampled_number = std::min(MS, N);
	auto x_arr = LinspaceFXP(a, b, sampled_number);
	if (EvaluatePrecision(F, disc_poly, x_arr))
		return disc_poly;
	else
		return nullptr;
}

/*
 * Expand coefficients and scaling factors to k.
 */
static void ExpandPoly(disc_poly_ptr poly, int k)
{
	while (poly->size() <= k)
		poly->emplace_back((fxp_t)0, (fxp_t)0);
}

static bool EvaluatePrecision(func_t F, disc_poly_ptr poly, fxp_arr_ptr points)
{
	for (auto x : *points)
	{
		if (Distance(EvaluateDiscAsCont(poly, x), F(FXPsimFLP(x))) > EPSILON)
			return false;
	}

	return true;
}

#pragma endregion


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Algo 3. ConstrainK
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

#pragma region ConstrainK

int ConstrainK(flp_t a, flp_t b, int k)
{
	double x_max = std::max(std::abs(a), std::abs(b));
	double x_min = std::min(std::abs(a), std::abs(b));
	int k_o = (x_max < 1.0) ? k : (int)std::floor((fxp_n - fxp_f - 1) / std::log2(x_max));
	int k_u = (x_min > 1.0) ? k : (int)std::floor(fxp_f / (-std::log2(x_min)));

	return std::min(k, std::min(k_u, k_o));
}

#pragma endregion


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Algo 4. FLP - FXP
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

#pragma region FLPsimFXP

fxp_t FLPsimFXP(flp_t flp)
{
	return float_to_fixed(flp, fxp_f);
}

flp_t FXPsimFLP(fxp_t fxp)
{
	return fixed_to_float(fxp, fxp_f);
}

#pragma endregion


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Algo 5. ScalePoly
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

#pragma region ScalePoly

static void ScaleC(flp_t c, int k, flp_t x, fxp_t* c_hat, fxp_t* s_hat);

disc_poly_ptr ScalePoly(cont_poly_ptr poly, flp_t a, flp_t b)
{
	assert(poly);

	int k = (int)poly->size();
	flp_t x_hat = std::max(std::abs(a), std::abs(b));
	disc_poly_ptr disc(new disc_poly_t());
	fxp_t c, s;
	for (int i = 0; i < k; i++)
	{
		ScaleC((*poly)[i], i, x_hat, &c, &s);
		disc->emplace_back(c, s);
	}

	return disc;
}

static void ScaleC(flp_t c, int k, flp_t x, fxp_t* c_hat, fxp_t* s_hat)
{
	assert(s_hat);
	assert(c_hat);

	fxp_t s1 = (fxp_t)1;	// FLPsimFXP(2^-f) = 2^-f * 2^f = 1;
	fxp_t s2 = FLPsimFXP(c * std::pow(x, k) / std::pow(2.0, fxp_n - fxp_f - 1));

	*s_hat = std::min(std::max(s1, s2), (fxp_t)1);
	*c_hat = FLPsimFXP(c / *s_hat);
}

#pragma endregion


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Algo 6. ResidualBoosting
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
#pragma region ResidualBoosting

disc_poly_ptr ResidualBoosting(disc_poly_ptr poly, func_t F, flp_t a, flp_t b)
{
	return nullptr;
}

#pragma endregion
