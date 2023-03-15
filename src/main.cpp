#include <cstdio>

#include <cstdlib>

#include "utils.h"
#include "tools/random.h"

void float_to_fix_test();
int main()
{
	float_to_fix_test();

	return 0;
}

void float_to_fix_test()
{
	double x1, x2;
	uint64_t y;

	const int TEST_ROUND = 10;

	for (int i = 0; i < TEST_ROUND; i++)
	{
		x1 = Random(-100.0, 100.0) / 50.0;
		printf("%20lf  -  ", x1);
		float_to_fixed(&x1, &y, 1, 20);
		printf("%10" PRIu64 "  -  ", y);
		fixed_to_float(&y, &x2, 1, 20);
		printf("%-20lf\n", x2);
	}
}
