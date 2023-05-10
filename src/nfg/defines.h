/********************************************************************
** Filename: defines.h
** 
** Description: Global type definitions.
*/

#pragma once

#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <cstdint>
#include <vector>
#include <memory>
#include <utility>

#include <cinttypes>

typedef double   flp_t;
typedef uint64_t fxp_t;

#define PRI_FXP PRIu64
#define PRI_FLP "f"

typedef std::vector<flp_t> flp_arr_t;
typedef std::vector<fxp_t> fxp_arr_t;

typedef std::shared_ptr<flp_arr_t> flp_arr_ptr;
typedef std::shared_ptr<fxp_arr_t> fxp_arr_ptr;

/*
 * We assume F(x) is a Lipschitz continuous function, and we can evaluate
 * F(x) in plaintext using FLP.
 * Here, `func_t` is the so-called continuous function.
 */
// typedef fxp_t func_t(fxp_t);
typedef flp_t (*func_t)(flp_t);

/*
 * A k-polynomial can be presented as k + 1 coefficients.
 * Both FLP (continuous) and FXP (discrete) forms are needed.
 * To avoid fxp overflow, we use two terms to represent one coefficient.
 * But flp coefficient doesn't need this.
 */
// typedef std::pair<flp_t, flp_t> flp_cof_t;
typedef flp_t                   flp_cof_t;
typedef std::pair<fxp_t, fxp_t> fxp_cof_t;

// continuous and discrete polynomial
typedef std::vector<flp_cof_t> cont_poly_t;
typedef std::vector<fxp_cof_t> disc_poly_t;

// pointer to polynomial
typedef std::shared_ptr<cont_poly_t> cont_poly_ptr;
typedef std::shared_ptr<disc_poly_t> disc_poly_ptr;

#endif
