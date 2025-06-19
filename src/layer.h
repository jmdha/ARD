#pragma once

#include <sys/types.h>

uint pval(const uint* buf, uint w, uint h, uint x, uint y);

// Calculates the proximity values for each cell in the grid.
// The proximity value (pval) is the minimum distance to a non-set cell.
// NOTE: out and buf can be the same memory, in which case, it is simply overwritten.
void prox(uint* out, const uint* buf, uint w, uint h);

// Splits the grid into sectors.
// Requires a grid of pvals.
void sect(uint* out, const uint* buf, uint w, uint h);

// Combines sectors into rooms.
// Requires a grid of sectors.
void room(uint* out, const uint* buf, uint w, uint h);
