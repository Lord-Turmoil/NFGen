#include "nfg.h"
#include "global.h"

static int m_cnt = 0;

static void _AddPoly(const poly_t poly, uint64_t a, uint64_t b);
static void _FitPiecewiseAux(func_t F, uint64_t a, uint64_t b, int k);

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
static void _AddPoly(const poly_t poly, uint64_t a, uint64_t b)
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
static void _FitPiecewiseAux(func_t F, uint64_t a, uint64_t b, int k)
{
	poly_t p = FitOnePiece(F, a, b, k);
	if (p.size() > 0)
	{
		_AddPoly(p, a, b);
	}
	else
	{
		uint64_t mid = (a + b) >> 1;
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

		p = FitOnePiece(F, a, b, k);
		if (p.size() > 0)
		{
			// Replace p[i] and p[i + 1] with p[k]
			
			// Merge the segment?
			divisions.erase(divisions.begin() + (i + 1));

			// Erase p[i], then it points at p[i + 1]
			auto it = polynomials.erase(polynomials.begin() + i);
			*it = p;	// make p[i + 1] p[k]
		}
		else
			i++;
	}
}