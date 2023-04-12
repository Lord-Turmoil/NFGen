#include <cstdio>
#include <memory>
#include <cstdlib>
#include <cinttypes>
#include <iostream>

#include "utils.h"
#include "tools/random.h"

void float_to_fix_test();
void smart_pointer_test();
void vector_test();
void fxp_mult_test();
void lagrange_test();
int main()
{
	// float_to_fix_test();
	// smart_pointer_test();
	// vector_test();
	// fxp_mult_test();
	lagrange_test();

	return 0;
}

static flp_t func(flp_t x)
{
	return 1 + x * x + 3.4 * x * x * x;
}

void lagrange_test()
{
	auto poly = LagrangeInterpolation(func, 1, 20, 20, 4);
	auto x_set = LinspaceFLP(1, 20, 20);

	printf_s("%zd\n", poly->size());
	for (int i = 0; i < poly->size(); i++)
	{
		flp_t c = poly->at(i);
		if (std::abs(c) < 1e-6)
			continue;
		printf_s("%f ", poly->at(i));
	}
	putchar('\n');

	for (auto x : *x_set)
	{
		printf_s("%20f - %20f - %20f\n", x, func(x), EvaluateCont(poly, x));
	}
}

void fxp_mult_test()
{
	flp_arr_t flps = { 2.0, 3.0, 4.0, 5.0, 6.0 };
	fxp_arr_t fxps(flps.size());

	float_to_fixed(flps.data(), fxps.data(), flps.size(), 4);
	
	for (auto it : fxps)
		std::cout << it << " ";
	std::cout << std::endl;

	flp_t flp = 1.0;
	fxp_t fxp = FLPsimFXP(1.0);

	std::cout << "flp = " << flp << std::endl;
	std::cout << "fxp = " << fxp << std::endl;

	for (auto it : flps)
		flp *= it;
	for (auto it : fxps)
		fxp *= it;
	std::cout << "flp = " << flp << " " << FLPsimFXP(flp) << std::endl;
	std::cout << "fxp = " << fxp << " " << FXPsimFLP(fxp) << std::endl;
}

void float_to_fix_test()
{
	double x1, x2;
	uint64_t y;

	const int TEST_ROUND = 10;

	/*
	for (int i = 0; i < TEST_ROUND; i++)
	{
		x1 = Random(-100.0, 100.0) / 50.0;
		printf("%20lf  -  ", x1);
		float_to_fixed(&x1, &y, 1, 20);
		printf("%10" PRIu64 "  -  ", y);
		fixed_to_float(&y, &x2, 1, 20);
		printf("%-20lf\n", x2);
	}
	*/

	uint64_t y2;
	x1 = -10.6;
	printf("%20lf  -  ", x1);
	float_to_fixed(&x1, &y, 1, 10);
	x1 /= 2;
	float_to_fixed(&x1, &y2, 1, 10);

	printf("%10" PRIu64 "  -  ", y);
	printf("%10" PRIu64 "  -  ", y2);
	printf("%10" PRIu64 "  -  ", y2 - y);
	y = y2 - y;

	fixed_to_float(&y, &x2, 1, 10);
	printf("%-20lf\n", x2);

}

void smart_pointer_test()
{
	std::shared_ptr<int> p;

	p = nullptr;

	if (!p)
		printf("p = nullptr");
}

void vector_test()
{
	std::vector<int> vec(3);

	vec[0] = 1;
	vec[1] = 2;
	vec[2] = 3;

	printf("%d\n", vec[2]);
}