#include <stdlib.h>
#include <string.h>

#include "misc.h"
#include "grid.h"

struct grid {
	uint32_t* buffer; 
	int width;
	int height;
};

struct grid *grid_init(int w, int h) {
	struct grid* grid = malloc(sizeof(struct grid));
	grid->buffer      = calloc(w * h, sizeof(uint32_t));
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

uint32_t grid_get(const struct grid *grid, int x, int y) {
	return grid->buffer[y * grid_width(grid) + x]; 
}

void grid_resize(struct grid *grid, int w, int h) {
	uint32_t* tmp = calloc(w * h, sizeof(uint32_t));
	for (int x = 0; x < min(grid->width, w); x++)
		for (int y = 0; y < min(grid->height, h); y++)
			tmp[y * w + x] = grid->buffer[y * grid->width + x];
	
	free(grid->buffer);
	grid->buffer = tmp;
	grid->width = w;
	grid->height = h;
}

void grid_reset(struct grid *grid) {
	const int w = grid_width(grid);
	const int h = grid_height(grid);
	memset(
			grid->buffer, 
			0, 
			w * h * sizeof(uint32_t)
	);
}

void grid_set(struct grid *grid, int x, int y, uint32_t v) {
	grid->buffer[y * grid_width(grid) + x] = v; 
}

uint32_t* grid_buf(struct grid* grid) {
	return grid->buffer;
}
