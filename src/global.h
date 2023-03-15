#pragma once

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <cstdint>
#include <vector>
#include <set>

/*
 * We assume F(x) is a Lipschitz continuous function, and we can evaluate
 * F(x) in plaintext using FLP.
 * Here, `func_t` is the so-called continuous function.
 */
typedef uint32_t func_t(uint32_t);

/*
 * A k-polynomial can be presented as k + 1 coefficients.
 */
typedef std::vector<uint64_t> poly_t;

extern int MS;    // Max sampling numbers.
extern int m_max; // Max pieces.

/*
 * The fitted piecewise polynomial p(k, m) hat.
 * The subscription starts with 1?
 */
extern std::vector<poly_t> polynomials;

// The divisions [w0, w1, ... ] of [a, b]
extern std::vector<uint64_t> divisions;

#endif