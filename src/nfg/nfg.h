/********************************************************************
** Filename: nfg.h
**
** Description: Implementation of NFGen core algorithms.
*/

#pragma once

#ifndef _NFG_H_
#define _NFG_H_

#include "defines.h"

#include <cstdio>


/*
 * Abbreviations:
 *   FLP -- floating point
 *   FXP -- fixed point
 */

// Load global variables.
void LoadConfig(FILE* fp);


// Algo 1.
void FitPiecewise(func_t F, flp_t a, flp_t b, int k);

// Algo 2.
disc_poly_ptr FitOnePiece(func_t F, flp_t a, flp_t b, int k);

// Algo 3.
int ConstrainK(flp_t a, flp_t b, int k);

// Algo 4.
fxp_t FLPsimFXP(flp_t flp);
flp_t FXPsimFLP(fxp_t fxp);

// Algo 5.
disc_poly_ptr ScalePoly(cont_poly_ptr poly, flp_t a, flp_t b);

// Algo 6.
disc_poly_ptr ResidualBoosting(disc_poly_ptr poly, func_t F, flp_t a, flp_t b);


#endif
