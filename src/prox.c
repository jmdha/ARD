#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "prox.h"
#include "grid.h"
#include "misc.h"

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

uint32_t val(struct prox* prox, int X, int Y, int w, int h) {
	for (int o = 1; o < INT_MAX; o++)
	for (int dx = -1; dx <= 1; dx++)
	for (int dy = -1; dy <= 1; dy++) {
		int x = X + o * dx;
		int y = Y + o * dy;
		if (x < 0 || y < 0 || x >= w || y >= h)
			continue;
		if (!prox_active(prox, x, y))
			return o;
	}
}

void prox_set(struct prox* prox, const int X, const int Y) {
	if (prox_active(prox, X, Y))
		return;

	const int w = prox_width(prox);
	const int h = prox_height(prox);

	grid_set(prox->buf, X, Y, (1 << 31) | val(prox, X, Y, w, h));

	for (int o = 1; o < INT_MAX; o++)
	for (int dx = -1; dx <= 1; dx++)
	for (int dy = -1; dy <= 1; dy++) {
		int x = X + o * dx;
		int y = Y + o * dy;
		if (x < 0 || y < 0 || x >= w || y >= h)
			return;
		if (prox_active(prox, x, y))
			grid_set(prox->buf, x, y, (1 << 31) | val(prox, x, y, w, h));
	}
}

void prox_unset(struct prox* prox, const int X, const int Y) {
	(void) prox;
	(void) X;
	(void) Y;
	exit(1);
}
