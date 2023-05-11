#include "macros.h"

#if !_TEST

#include "nfg/nfg.h"
#include "nfg/global.h"
#include "dump.h"

#include <cstdio>
#include <cmath>
  
const double PI = 3.14159265358979323846;
const double A = 0.044715;
const double e = 2.718281828459045235360;

static flp_t gelu(flp_t x);
static flp_t zero(flp_t x);
static flp_t sinx(flp_t x);
static flp_t sigmoid(flp_t x);
static flp_t sinxx(flp_t x);

int main()
{
	flp_t left = -5.0;
	flp_t right = 5.0;
	int sample = 1000;
	func_t func = sinxx;

	// Get NFGen result.
	StartTick();
	FitPiecewise(func, left, right, MAX_K);
	PrintTick();

	std::vector<disc_poly_ptr> polys;
	std::vector<flp_t> divs;
	if (!GetResult(&polys, &divs))
	{
		printf("NFGen failed!\n");
		return 1;
	}
	printf("NFGen succeeded!\n");

	// Display result.
	DumpResult(&polys, &divs);
	auto p = FuncPtr(new SegmentFunc<disc_poly_t>(polys, divs));

	// Dump Result for python.
	FILE* fp;

	// Basic result.
	if (fopen_s(&fp, "result.txt", "a") != 0)
	{
		printf("Cannot open 'function.csv' for writing!\n");
		return 2;
	}
	DumpResult(fp, &polys, &divs);
	fclose(fp);

	// Graph information for NFGen.
	if (fopen_s(&fp, "function.csv", "w") != 0)
	{
		printf("Cannot open 'function.csv' for writing!\n");
		return 2;
	}
	DumpForPython(fp, p, left, right, sample);
	fclose(fp);

	// Graph information for standard function.
	if (fopen_s(&fp, "sample.csv", "w") != 0)
	{
		printf("Cannot open 'sample.csv' for writing!\n");
		return 3;
	}
	DumpForPython(fp, FuncPtr(new MonoFunc<func_t>(func)), left, right, sample);
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

static flp_t sigmoid(flp_t x)
{
	return 1.0 / (1 + std::pow(e, -x));
}

static flp_t sinxx(flp_t x)
{
	return std::sin(x * x);
}


#endif