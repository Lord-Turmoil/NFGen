#pragma once

#ifndef _UTILS_H_
#define _UTILS_H_

#include "global.h"

static_assert(sizeof(fxp_t) == 8, "sizeof(fxp_t) error");

void float_to_fixed(flp_t* src, fxp_t* dst, size_t size, unsigned f);
void fixed_to_float(fxp_t* src, flp_t* dst, size_t size, unsigned f);
fxp_t FLPsimFXP(flp_t flp);
flp_t FXPsimFLP(fxp_t fxp);

cont_poly_ptr ChebyInterpolation(func_t F, flp_t a, flp_t b, int k);
cont_poly_ptr LagrangeInterpolation(func_t F, flp_t a, flp_t b, int N, int k);

flp_arr_ptr LinspaceFLP(flp_t a, flp_t b, int n);
fxp_arr_ptr LinspaceFXP(flp_t a, flp_t b, int n);

flp_t EvaluateDiscAsCont(disc_poly_ptr poly, fxp_t x);
fxp_t EvaluateDiscAsDisc(disc_poly_ptr poly, fxp_t x);
flp_t EvaluateCont(cont_poly_ptr poly, flp_t x);

flp_t Distance(flp_t x, flp_t y);

#endif
