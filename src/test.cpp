#include "macros.h"

#ifdef _TEST

#include <cstdio>
#include <memory>
#include <cstdlib>
#include <cinttypes>
#include <iostream>

#include "nfg/utils.h"
#include "tools/random.h"
#include "nfg/nfg.h"
#include "nfg/func.h"

void float_to_fix_test();
void smart_pointer_test();
void vector_test();
void fxp_mult_test();
void lagrange_test();
void func_test();
int main()
{
	// float_to_fix_test();
	// smart_pointer_test();
	// vector_test();
	// fxp_mult_test();
	// lagrange_test();
	func_test();

	return 0;
}

static flp_t f1(flp_t x)
{
	return x * 2.0;
}

static flp_t f2(flp_t x)
{
	return x * x;
}

static flp_t f3(flp_t x)
{
	return x - 1;
}

void func_test()
{
	// f1 * (f2 + f3)
	// 2x * (x * x + (x - 1))
	MonoFunc<func_t> mono1(f1);
	MonoFunc<func_t> mono2(f2);
	MonoFunc<func_t> mono3(f3);

	CompoundFunc<MonoFunc<func_t>, MonoFunc<func_t>, BinaryAdd> comp1(mono2, mono3, BinaryAdd());
	CompoundFunc<MonoFunc<func_t>, CompoundFunc<MonoFunc<func_t>, MonoFunc<func_t>, BinaryAdd>, BinaryMul> comp2(mono1, comp1, BinaryMul());

	FuncPtr p1 = FuncPtr(new CompoundFunc<MonoFunc<func_t>, CompoundFunc<MonoFunc<func_t>, MonoFunc<func_t>, BinaryAdd>, BinaryMul>(comp2));

	// 2 * 3 * (3 * 3 + (3 - 1)) = 6 * (9 + 2) = 66!
	printf("%f\n", p1->Evaluate(3));


	cont_poly_ptr poly(new cont_poly_t());

	// 1 + x + x * x
	poly->push_back(1);
	poly->push_back(1);
	poly->push_back(1);

	FuncPtr p2 = FuncPtr(new MonoFunc<cont_poly_t>(*poly.get()));

	printf("%f\n", p2->Evaluate(3));
}


static flp_t func(flp_t x)
{
	return 1 + x * x + 3.4 * x * x * x;
}

#if 0
void lagrange_test()
{
	auto poly = LagrangeInterpolation(func, 1, 20, 4);
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

#endif

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

#endif