#pragma once

#ifndef _NFG_H_
#define _NFG_H_

#include "global.h"

/*
 * Abbreviations:
 *   FLP -- floating point
 *   FXP -- fixed point
 */

void FitPiecewise(func_t F, double a, double b, int k);

disc_poly_ptr FitOnePiece(func_t F, double a, double b, int k);





#endif