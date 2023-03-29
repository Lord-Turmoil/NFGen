#include "global.h"

int MS = 1000;
int m_max = 10;

int fxp_n = 64;
int fxp_f = 12;

double EPSILON = 1e-3;
double SOFT_ZERO = 1e-6;

std::vector<disc_poly_ptr> disc_polys;
std::vector<double> divisions;