#include <ubench.h/ubench.h>

#include "lib/grid.h"
#include "lib/layer.h"

UBENCH_EX(prox, empty) {
	struct grid* grid = grid_init(5, 5);
	UBENCH_DO_BENCHMARK() {
		prox(grid_buf(grid), grid_buf(grid), 5, 5);
	}
	grid_free(grid);
}

UBENCH_EX(prox, 5x53x3) {
	struct grid* grid = grid_init(5, 5);
	for (uint x = 1; x < 4; x++)
		for (uint y = 1; y < 4; y++)
			grid_set(grid, x, y, 1);
	UBENCH_DO_BENCHMARK() {
		prox(grid_buf(grid), grid_buf(grid), 5, 5);
	}
	grid_free(grid);
}
