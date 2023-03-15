#pragma once

#ifndef _UTILS_H_
#define _UTILS_H_

#include <cstdint>
#include <cinttypes>

static_assert(sizeof(uint64_t) == 8, "sizeof(uint64_t) error");

void float_to_fixed(double* src, uint64_t* dst, size_t size, unsigned n);
void fixed_to_float(uint64_t* src, double* dst, size_t size, unsigned n);

#endif
