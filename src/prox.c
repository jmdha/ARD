#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "prox.h"
#include "grid.h"

struct prox {
	struct grid* buf;
};

struct prox* prox_init(int w, int h) {
	struct prox* prox = malloc(sizeof(struct prox));
	prox->buf         = grid_init(w, h);
	return prox;
}

int prox_width(const struct prox* prox) {
	return grid_width(prox->buf);
}

int prox_height(const struct prox* prox) {
	return grid_height(prox->buf);
}

uint32_t prox_max(const struct prox* prox) {
	uint32_t max = 0;
	for (int x = 0; x < prox_width(prox); x++)
		for (int y = 0; y < prox_height(prox); y++)
			if (prox_get(prox, x, y) > max)
				max = prox_get(prox, x, y);
	return max;
}

uint32_t prox_get(const struct prox* prox, int x, int y) {
	return grid_get(prox->buf, x, y) & (~(1 << 31));
}

bool prox_active(const struct prox* prox, int x, int y) {
	return (grid_get(prox->buf, x, y) & (1 << 31)) != 0;
}

void prox_free(struct prox* prox) {
	grid_free(prox->buf);
	free(prox);
}

void prox_resize(struct prox* prox, int w, int h) {
	grid_resize(prox->buf, w, h);
}

bool inside(int x, int y, int w, int h) {
	return x >= 0 &&
	       y >= 0 &&
	       x <  w &&
	       y <  h;
}

// TODO: Replace count with a directional function.
// The way it currently works, 3/4 of the work it is doing is wasted.
// Namely checking whether it is outside the bounds of directions
// it is not going.
uint32_t count(struct prox* prox, int x, int y, int xv, int yv) { 
	const int w = prox_width(prox);
	const int h = prox_height(prox);
	uint32_t v = 0;
	x += xv;
	y += yv;
	while (inside(x, y, w, h) && prox_active(prox, x, y)) {
		v += 1;
		x += xv;
		y += yv;
	}
	return v;
}

void prox_set(struct prox* prox, const int X, const int Y) {
	if (prox_active(prox, X, Y))
		return;

	const int      w = prox_width(prox);
	const int      h = prox_height(prox);
	const uint32_t r = count(prox, X, Y,  1,  0);
	const uint32_t l = count(prox, X, Y, -1,  0);
	const uint32_t u = count(prox, X, Y,  0,  1);
	const uint32_t d = count(prox, X, Y,  0, -1);

	grid_set(prox->buf, X, Y, (1 << 31) | (u + d + r + l));

	for (int x = X + 1; x <  w && prox_active(prox, x, Y); x++)
		grid_set(prox->buf, x, Y, grid_get(prox->buf, x, Y) + l + 1);

	for (int x = X - 1; x >= 0 && prox_active(prox, x, Y); x--)
		grid_set(prox->buf, x, Y, grid_get(prox->buf, x, Y) + r + 1);

	for (int y = Y + 1; y <  h && prox_active(prox, X, y); y++)
		grid_set(prox->buf, X, y, grid_get(prox->buf, X, y) + d + 1);

	for (int y = Y - 1; y >= 0 && prox_active(prox, X, y); y--)
		grid_set(prox->buf, X, y, grid_get(prox->buf, X, y) + u + 1);
}

void prox_unset(struct prox* prox, const int X, const int Y) {
	if (!prox_active(prox, X, Y))
		return;

	const int      w = prox_width(prox);
	const int      h = prox_height(prox);
	const uint32_t r = count(prox, X, Y,  1,  0);
	const uint32_t l = count(prox, X, Y, -1,  0);
	const uint32_t u = count(prox, X, Y,  0,  1);
	const uint32_t d = count(prox, X, Y,  0, -1);

	grid_set(prox->buf, X, Y, 0);

	for (int x = X + 1; x <  w && prox_active(prox, x, Y); x++)
		grid_set(prox->buf, x, Y, grid_get(prox->buf, x, Y) - l - 1);

	for (int x = X - 1; x >= 0 && prox_active(prox, x, Y); x--)
		grid_set(prox->buf, x, Y, grid_get(prox->buf, x, Y) - r - 1);

	for (int y = Y + 1; y <  h && prox_active(prox, X, y); y++)
		grid_set(prox->buf, X, y, grid_get(prox->buf, X, y) - d - 1);

	for (int y = Y - 1; y >= 0 && prox_active(prox, X, y); y--)
		grid_set(prox->buf, X, y, grid_get(prox->buf, X, y) - u - 1);
}
