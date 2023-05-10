/********************************************************************
** Filename: global.h
**
** Description: Global variables and constants.
*/

#include "global.h"

const int MS = 1000;
const int m_MAX = 10;

const unsigned fxp_n = 32;
const unsigned fxp_f = 12;

const int K_MAX = 4;

const flp_t EPSILON = 1e-3;
const flp_t SOFT_ZERO = 1.0;

std::vector<disc_poly_ptr> disc_polys;
std::vector<flp_t> divisions;
