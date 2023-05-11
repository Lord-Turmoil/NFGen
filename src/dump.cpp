/********************************************************************
** Filename: dump.cpp
**
** Description: Utility functions to present NFGen results.
*/

#include "dump.h"

#include <cmath>
#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#endif

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
	printf("Dumping Result...\n");

	if (!polys || !divs)
	{
		fprintf(stderr, "polys and div must not be nullptr!\n");
		return;
	}
	if (polys->size() + 1 != divs->size())
	{
		fprintf(stderr, "Segment number mismatch! %zd : %zd\n",
				polys->size(), divs->size());
		return;
	}

#ifdef _WIN32	// print timestamp
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	fprintf(fp, "==================== ");	// * 20
	fprintf(fp, "%02hu/%02hu/%02hu ", sysTime.wMonth, sysTime.wDay, sysTime.wYear);
	fprintf(fp, "%02hu:%02hu:%02hu ", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	fprintf(fp, "====================\n");	// *30
#else
	fprintf_s(fp, "Dump ==============================\n");	// *30
#endif

	fprintf(fp, "        FXP: n = %u, f = %u\n", fxp_n, fxp_f);
	fprintf(fp, " Max sample: %d\n", MS);
	fprintf(fp, "Max segment: %d\n", m_MAX);
	fprintf(fp, "  Max order: %d\n", MAX_K);
	fprintf(fp, "    EPSILON: %g\n", EPSILON);
	fprintf(fp, "  Soft zero: %g\n", SOFT_ZERO);
	fprintf(fp, "-------------------------------------------------------------\n");	// *30

	for (int i = 0; i < (int)polys->size(); i++)
	{
		fprintf(fp, "[%+10.6f, %+10.6f]: ", (*divs)[i], (*divs)[i + 1]);
		// for (auto it : *(*polys)[i])
		for (int j = 0; j < (*polys)[i]->size(); j++)
		{
			auto it = (*(*polys)[i])[j];
			double coef = fixed_to_float(it.first, fxp_f) * fixed_to_float(it.second, fxp_f);
			fprintf(fp, " %c %.6f", coef < 0.0 ? '-' : '+', std::abs(coef));
			if (j > 1)
				fprintf(fp, "x^%d", j);
			else if (j > 0)
				fprintf(fp, "x");
			fprintf(fp, "");
		}
		fprintf(fp, "\n");
	}

	printf("Succeeded!\n");
}

void DumpForPython(FuncPtr func, flp_t a, flp_t b, int n)
{
	DumpForPython(stdout, func, a, b, n);
}

void DumpForPython(FILE* fp, FuncPtr func, flp_t a, flp_t b, int n)
{
	printf("Dumping CSV...\n");

	fprintf(fp, "x,y\n");	// csv header
	auto x_set = LinspaceFLP(a, b, n);
	if (!x_set)
		return;

	for (auto x : *x_set)
		fprintf(fp, "%f,%f\n", x, func->Evaluate(x));
	printf("Succeeded!\n");
}


std::chrono::steady_clock::time_point _start;
void StartTick()
{
	printf("Tick starts...\n");
	_start = std::chrono::steady_clock::now();
}

void PrintTick()
{
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds(end - _start);
	printf("Tick elapsed: %f seconds.\n", elapsed_seconds.count());
}
