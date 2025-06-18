#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "misc.h"

// ABC
struct grid;

struct grid* grid_init(int w, int h);
void         grid_free(struct grid* grid);

int          grid_width (const struct grid* grid);
int          grid_height(const struct grid* grid);
uint         grid_count (const struct grid* grid);
uint         grid_get   (const struct grid* grid, int x, int y);
uint         grid_max   (const struct grid* grid);

void         grid_resize(struct grid* grid, int w, int h);
void         grid_reset (struct grid* grid);
void         grid_set   (struct grid* grid, int x, int y, uint v);
uint*        grid_buf   (struct grid* grid);
