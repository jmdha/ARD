#include <stdlib.h>
#include <string.h>

#include "grid.h"

struct grid {
	uint32_t* buffer; 
	int width;
	int height;
};

struct grid *grid_init(int w, int h) {
	struct grid* grid = malloc(sizeof(struct grid));
	grid->buffer      = calloc(w * h, sizeof(uint));
	grid->width       = w;
	grid->height      = h;
	return grid;
}

void grid_free(struct grid *grid) {
	free(grid->buffer);
	free(grid);
}

int grid_width(const struct grid *grid) {
	return grid->width;
}

int grid_height(const struct grid *grid) {
	return grid->height;
}

uint grid_get(const struct grid *grid, int x, int y) {
	return grid->buffer[y * grid_width(grid) + x]; 
}

uint grid_max(const struct grid* grid) {
	uint m = 0;
	for (uint x = 0; x < grid_width(grid); x++)
		for (uint y = 0; y < grid_height(grid); y++)
			if (grid_get(grid, x, y) > m)
				m = grid_get(grid, x, y);
	return m;
}

void grid_set(struct grid *grid, int x, int y, uint v) {
	grid->buffer[y * grid_width(grid) + x] = v; 
}

uint* grid_buf(struct grid* grid) {
	return grid->buffer;
}
