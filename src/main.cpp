#include <cstdio>
#include <memory>
#include <cstdlib>
#include <cinttypes>

#include "utils.h"
#include "tools/random.h"

void float_to_fix_test();
void smart_pointer_test();
void vector_test();
int main()
{
	// float_to_fix_test();
	// smart_pointer_test();
	vector_test();

	return 0;
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