#pragma once

#ifndef _UTILS_H_
#define _UTILS_H_

#include "global.h"

static_assert(sizeof(uint64_t) == 8, "sizeof(uint64_t) error");

void float_to_fixed(double* src, uint64_t* dst, size_t size, unsigned f);
void fixed_to_float(uint64_t* src, double* dst, size_t size, unsigned f);
uint64_t FLPsimFXP(double flp);
double FXPsimFLP(uint64_t fxp);

cont_poly_ptr ChebyInterpolation(func_t F, double a, double b, int k);
cont_poly_ptr LagrangeInterpolation(func_t F, double a, double b, int k);

flp_arr_ptr LinspaceFLP(double a, double b, int n);
fxp_arr_ptr LinspaceFXP(double a, double b, int n);

uint64_t Evaluate(disc_poly_ptr poly, uint64_t x);
double Evaluate(cont_poly_ptr poly, double x);

double Distance(double x, double y);

#endif
