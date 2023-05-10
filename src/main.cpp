#include "macros.h"

#if !_TEST

#include "nfg/nfg.h"
#include "nfg/global.h"
#include "dump.h"

#include <cstdio>
#include <cmath>
  
const double PI = 3.14159265358979323846;
const double A = 0.044715;

static flp_t gelu(flp_t x);
static flp_t zero(flp_t x);
static flp_t sinx(flp_t x);

int main()
{
	double left = -5.0;
	double right = 5.0;
	int sample = 100;

	// Get NFGen result.
	FitPiecewise(sinx, left, right, MAX_K);
	std::vector<disc_poly_ptr> polys;
	std::vector<flp_t> divs;
	if (!GetResult(&polys, &divs))
	{
		printf("NFGen failed!\n");
		return 1;
	}

	DumpResult(&polys, &divs);
	auto p = FuncPtr(new SegmentFunc<disc_poly_t>(polys, divs));

	// Dump Result for python.
	FILE* fp;

	if (fopen_s(&fp, "result.txt", "a") != 0)
	{
		printf("Cannot open 'function.csv' for writing!\n");
		return 2;
	}
	DumpResult(fp, &polys, &divs);
	fclose(fp);

	if (fopen_s(&fp, "function.csv", "w") != 0)
	{
		printf("Cannot open 'function.csv' for writing!\n");
		return 2;
	}
	DumpForPython(fp, p, left, right, sample);
	fclose(fp);

	if (fopen_s(&fp, "example.csv", "w") != 0)
	{
		printf("Cannot open 'example.csv' for writing!\n");
		return 2;
	}
	DumpForPython(fp, FuncPtr(new MonoFunc<func_t>(sinx)), left, right, sample);
	fclose(fp);

	return 0;
}

static flp_t gelu(flp_t x)
{
	return 0.5 * x * (1 + tanh(sqrt(2.0 / PI) * (x + A * std::pow(x, 3.0))));
}

static flp_t zero(flp_t x)
{
	return 0.0;
}

static flp_t sinx(flp_t x)
{
	return std::sin(x);
}

#endif