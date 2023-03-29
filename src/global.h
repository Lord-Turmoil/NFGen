#pragma once

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <cstdint>
#include <vector>
#include <set>
#include <memory>

/*
 * We assume F(x) is a Lipschitz continuous function, and we can evaluate
 * F(x) in plaintext using FLP.
 * Here, `func_t` is the so-called continuous function.
 */
typedef uint64_t func_t(uint64_t);

/*
 * A k-polynomial can be presented as k + 1 coefficients.
 * Both FLP (continuous) and FXP (discrete) forms are needed.
 */
typedef std::vector<uint64_t> fxp_arr_t;
typedef std::shared_ptr<fxp_arr_t> fxp_arr_ptr;
typedef std::vector<double> flp_arr_t;
typedef std::shared_ptr<flp_arr_t> flp_arr_ptr;

using cont_poly_t = flp_arr_t;
using cont_poly_ptr = flp_arr_ptr;

using disc_poly_t = fxp_arr_t;
using disc_poly_ptr = fxp_arr_ptr;


extern int MS;    // Max sampling numbers.
extern int m_max; // Max pieces.

extern int fxp_n;
extern int fxp_f;

extern double EPSILON;
extern double SOFT_ZERO;


/*
 * The fitted piecewise polynomial p(k, m) hat.
 * The subscription starts with 1?
 */
extern std::vector<disc_poly_ptr> disc_polys;

// The divisions [w0, w1, ... ] of [a, b]
extern std::vector<double> divisions;

#endif