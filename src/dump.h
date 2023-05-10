/********************************************************************
** Filename: dump.h
**
** Description: Utility functions to present NFGen results.
*/

#pragma once

#include "nfg/defines.h"
#include "nfg/func.h"

bool GetResult(std::vector<disc_poly_ptr>* polys, std::vector<flp_t>* divs);

void DumpResult(std::vector<disc_poly_ptr>* polys, std::vector<flp_t>* divs);
void DumpResult(FILE* fp, std::vector<disc_poly_ptr>* polys, std::vector<flp_t>* divs);

// Dump x, y as CSV file.
void DumpForPython(FuncPtr func, flp_t a, flp_t b, int n);
void DumpForPython(FILE* fp, FuncPtr func, flp_t a, flp_t b, int n);
