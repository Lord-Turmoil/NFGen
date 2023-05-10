/********************************************************************
** Filename: dump.cpp
**
** Description: Utility functions to present NFGen results.
*/

#include "dump.h"

bool GetResult(std::vector<disc_poly_ptr>* polys, std::vector<flp_t>* divs)
{
	// It shouldn't be this...
	if (disc_polys.size() + 1 != divisions.size())
		return false;

	if (polys)
		*polys = disc_polys;
	if (divs)
		*divs = divisions;

	return true;
}

void DumpResult(std::vector<disc_poly_ptr>* polys, std::vector<flp_t>* divs)
{
	DumpResult(stdout, polys, divs);
}

void DumpResult(FILE* fp, std::vector<disc_poly_ptr>* polys, std::vector<flp_t>* divs)
{
	if (!polys || !divs)
	{
		fprintf(fp, "polys and div must not be nullptr!\n");
		return;
	}
	if (polys->size() + 1 != divs->size())
	{
		fprintf(fp, "Segment number mismatch! %zd : %zd\n",
				polys->size(), divs->size());
		return;
	}

	for (int i = 0; i < (int)polys->size(); i++)
	{
		fprintf(fp, "[%8.6f, %8.6f]: ", (*divs)[i], (*divs)[i + 1]);
		for (auto it : *(*polys)[i])
			fprintf(fp, "(%" PRI_FXP ", %" PRI_FXP ") ", it.first, it.second);
		fprintf(fp, "\n");
	}

}

void DumpForPython(FuncPtr func, flp_t a, flp_t b, int n)
{
	DumpForPython(stdout, func, a, b, n);
}

void DumpForPython(FILE* fp, FuncPtr func, flp_t a, flp_t b, int n)
{
	fprintf(fp, "x,y\n");	// csv header
	auto x_set = LinspaceFLP(a, b, n);
	if (!x_set)
		return;

	for (auto x : *x_set)
		fprintf(fp, "%f,%f\n", x, func->Evaluate(x));
}
