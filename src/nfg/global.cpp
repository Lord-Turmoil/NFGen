/********************************************************************
** Filename: global.h
**
** Description: Global variables and constants.
*/

#include "global.h"

// MS: 1000
int MS = 10000;

// m_MAX: 10
int m_MAX = 10;

// fxp <32, 12>
unsigned fxp_n = 32;
unsigned fxp_f = 12;

int MAX_K = 4;

// epsilon: 1e-3
flp_t EPSILON = 1e-3;
flp_t SOFT_ZERO = 1;

std::vector<disc_poly_ptr> disc_polys;
std::vector<flp_t> divisions;
