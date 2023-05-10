#pragma once

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "defines.h"

extern const int MS;    // Max sampling numbers.
extern const int m_MAX; // Max pieces.

extern const unsigned fxp_n;
extern const unsigned fxp_f;

extern const int MAX_K;

extern const flp_t EPSILON;
extern const flp_t SOFT_ZERO;

/*
 * The fitted piecewise polynomial p(k, m) hat.
 * The subscription starts with 1?
 */
extern std::vector<disc_poly_ptr> disc_polys;

// The divisions [w0, w1, ... ] of [a, b]
extern std::vector<flp_t> divisions;

#endif