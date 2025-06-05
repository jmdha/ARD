#include <utest.h/utest.h>

#include "lib/grid.h"
#include "lib/layer.h"

UTEST(pval, empty) {
	struct grid* prox = grid_init(1, 1);
	ASSERT_EQ(pval(grid_buf(prox), 1, 1, 0, 0), 1u);
	grid_free(prox);
}

UTEST(pval, 3x3) {
	const uint w = 3;
	const uint h = 3;
	struct grid* grid = grid_init(w, h);
	ASSERT_EQ(pval(grid_buf(grid), w, h, 1, 1), 1u);
	for (uint x = 0; x < w; x++)
		for (uint y = 0; y < h; y++)
			grid_set(grid, x, y, 1);
	ASSERT_EQ(pval(grid_buf(grid), w, h, 0, 1), 1u);
	ASSERT_EQ(pval(grid_buf(grid), w, h, 1, 0), 1u);
	ASSERT_EQ(pval(grid_buf(grid), w, h, 1, 2), 1u);
	ASSERT_EQ(pval(grid_buf(grid), w, h, 2, 1), 1u);
	ASSERT_EQ(pval(grid_buf(grid), w, h, 1, 1), 2u);
	grid_free(grid);
}

UTEST(prox, 5x53x3) {
	const uint w = 5;
	const uint h = 5;
	struct grid* grid = grid_init(w, h);
	for (uint x = 1; x < 4; x++)
		for (uint y = 1; y < 4; y++)
			grid_set(grid, x, y, 1);
	prox(grid_buf(grid), grid_buf(grid), w, h);
	ASSERT_EQ(grid_get(grid, 0, 0), 0u);
	ASSERT_EQ(grid_get(grid, 0, 4), 0u);
	ASSERT_EQ(grid_get(grid, 4, 0), 0u);
	ASSERT_EQ(grid_get(grid, 4, 4), 0u);
	ASSERT_EQ(grid_get(grid, 1, 1), 1u);
	ASSERT_EQ(grid_get(grid, 1, 3), 1u);
	ASSERT_EQ(grid_get(grid, 3, 1), 1u);
	ASSERT_EQ(grid_get(grid, 3, 3), 1u);
	ASSERT_EQ(grid_get(grid, 2, 2), 2u);
	grid_free(grid);
}

UTEST(sect, 3x3) {
	const uint w = 3;
	const uint h = 3;
	struct grid* grid = grid_init(w, h);
	grid_set(grid, 0, 0, 1);
	grid_set(grid, 0, 1, 1);
	grid_set(grid, 0, 2, 1);
	grid_set(grid, 1, 0, 1);
	grid_set(grid, 1, 1, 2);
	grid_set(grid, 1, 2, 1);
	grid_set(grid, 2, 0, 1);
	grid_set(grid, 2, 1, 1);
	grid_set(grid, 2, 2, 1);
	sect(grid_buf(grid), grid_buf(grid), w, h);
	for (uint x = 0; x < w; x++)
		for (uint y = 0; y < h; y++)
			ASSERT_EQ(grid_get(grid, x, y), 1u);
	grid_free(grid);
}

UTEST(sect, two) {
	const uint w = 3;
	const uint h = 4;
	struct grid* grid = grid_init(w, h);
	grid_set(grid, 0, 0, 1);
	grid_set(grid, 0, 1, 1);
	grid_set(grid, 0, 2, 1);
	grid_set(grid, 1, 0, 1);
	grid_set(grid, 1, 1, 2);
	grid_set(grid, 1, 2, 1);
	grid_set(grid, 2, 0, 1);
	grid_set(grid, 2, 1, 1);
	grid_set(grid, 2, 2, 1);
	grid_set(grid, 2, 3, 1);
	sect(grid_buf(grid), grid_buf(grid), w, h);
	for (uint x = 0; x < w; x++)
		for (uint y = 0; y < 3; y++)
			ASSERT_EQ(grid_get(grid, x, y), 1u);
	ASSERT_EQ(grid_get(grid, 2, 3), 2u);
	grid_free(grid);

}
