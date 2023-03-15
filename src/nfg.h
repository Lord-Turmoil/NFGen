#pragma once

#ifndef _NFG_H_
#define _NFG_H_

#include "global.h"

/*
 * Abbreviations:
 *   FLP -- floating point
 *   FXP -- fixed point
 */

void FitPiecewise(func_t F, uint64_t a, uint64_t b, int k);

poly_t FitOnePiece(func_t F, uint64_t a, uint64_t b, int k);

#endif