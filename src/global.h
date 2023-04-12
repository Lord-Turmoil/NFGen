#pragma once

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "defines.h"

extern int MS;    // Max sampling numbers.
extern int m_max; // Max pieces.

extern unsigned fxp_n;
extern unsigned fxp_f;

extern flp_t EPSILON;
extern flp_t SOFT_ZERO;

/*
 * The fitted piecewise polynomial p(k, m) hat.
 * The subscription starts with 1?
 */
extern std::vector<disc_poly_ptr> disc_polys;

// The divisions [w0, w1, ... ] of [a, b]
extern std::vector<flp_t> divisions;

#endif