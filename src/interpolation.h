#pragma once

#include "defines.h"
#include "func.h"

cont_poly_ptr ChebyshevInterpolation(FuncPtr F, flp_t a, flp_t b, int k);
cont_poly_ptr LagrangeInterpolation(FuncPtr F, flp_t a, flp_t b, int k);
