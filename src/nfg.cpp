#include "nfg.h"
#include "global.h"

static int m_cnt = 0;

static void _AddPoly(poly_ptr poly, div_t a, div_t b);
static void _FitPiecewiseAux(func_t F, div_t a, div_t b, int k);

void FitPiecewise(func_t F, uint64_t a, uint64_t b, int k)
{
	m_cnt = 0;
	polynomials.clear();
	divisions.clear();
	_FitPiecewiseAux(F, a, b, k);
}

/*
 * Add a polynomial and the segment it represents.
 */
static void _AddPoly(poly_ptr poly, div_t a, div_t b)
{
	polynomials.emplace_back(poly);
	if (divisions.empty())
		divisions.push_back(a);
	divisions.push_back(b);
}

/*
 * This will be called recursively, and the segments in [a, b] are
 * calculated strictly from left to right.
 */
static void _FitPiecewiseAux(func_t F, div_t a, div_t b, int k)
{
	poly_ptr poly = FitOnePiece(F, a, b, k);
	if (poly)
	{
		_AddPoly(poly, a, b);
	}
	else
	{
		div_t mid = (a + b) / 2;
		FitPiecewise(F, a, mid, k);
		FitPiecewise(F, mid, b, k);

		if (m_cnt++ > m_max)
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
			auto it = polynomials.erase(polynomials.begin() + i);
			*it = poly;	// make p[i + 1] p[k]
		}
		else
			i++;
	}
}


poly_ptr FitOnePiece(func_t F, uint64_t a, uint64_t b, int k)
{

}