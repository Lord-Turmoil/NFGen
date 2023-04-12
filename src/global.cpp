#include "global.h"

int MS = 1000;
int m_max = 10;

unsigned fxp_n = 32;
unsigned fxp_f = 12;

flp_t EPSILON = 1e-3;
flp_t SOFT_ZERO = 1.0;

std::vector<disc_poly_ptr> disc_polys;
std::vector<flp_t> divisions;
