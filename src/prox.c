#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "prox.h"

struct prox {
	uint32_t* buf;
	uint32_t  max;
	int       width;
	int       height;
};

struct prox* prox_init(int w, int h) {
	struct prox* prox = malloc(sizeof(struct prox));
	prox->buf         = calloc(w * h, sizeof(uint32_t));
	prox->max         = 0;
	prox->width       = w;
	prox->height      = h;
	return prox;
}

int prox_width(const struct prox* prox) {
	return prox->width;
}

int prox_height(const struct prox* prox) {
	return prox->height;
}

uint32_t prox_max(const struct prox* prox) {
	return prox->max;
}

uint32_t prox_get(const struct prox* prox, int x, int y) {
	return prox->buf[y * prox_width(prox) + x] & (~(1 << 31));
}

bool prox_active(const struct prox* prox, int x, int y) {
	return (prox->buf[y * prox_width(prox) + x] & (1 << 31)) != 0;
}

void prox_free(struct prox* prox) {
	free(prox->buf);
	free(prox);
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
	const int      w = prox_width(prox);
	const int      h = prox_height(prox);
	const uint32_t r = count(prox, X, Y,  1,  0);
	const uint32_t l = count(prox, X, Y, -1,  0);
	const uint32_t u = count(prox, X, Y,  0,  1);
	const uint32_t d = count(prox, X, Y,  0, -1);

	prox->buf[Y * w + X] = (1 << 31) | (u + d + r + l);

	for (int x = X + 1; x <  w && prox_active(prox, x, Y); x++)
		prox->buf[Y * w + x] = prox->buf[Y * w + x] + l + 1;

	for (int x = X - 1; x >= 0 && prox_active(prox, x, Y); x--)
		prox->buf[Y * w + x] = prox->buf[Y * w + x] + r + 1;

	for (int y = Y + 1; y <  h && prox_active(prox, X, y); y++)
		prox->buf[y * w + X] = prox->buf[y * w + X] + d + 1;

	for (int y = Y - 1; y >= 0 && prox_active(prox, X, y); y--)
		prox->buf[y * w + X] = prox->buf[y * w + X] + u + 1;
}

void prox_unset(struct prox* prox, const int X, const int Y) {
	const int      w = prox_width(prox);
	const int      h = prox_height(prox);
	const uint32_t r = count(prox, X, Y,  1,  0);
	const uint32_t l = count(prox, X, Y, -1,  0);
	const uint32_t u = count(prox, X, Y,  0,  1);
	const uint32_t d = count(prox, X, Y,  0, -1);

	prox->buf[Y * w + X] = (1 << 31) | (u + d + r + l);

	for (int x = X + 1; x <  w && prox_active(prox, x, Y); x++)
		prox->buf[Y * w + x] = prox->buf[Y * w + x] - l - 1;

	for (int x = X - 1; x >= 0 && prox_active(prox, x, Y); x--)
		prox->buf[Y * w + x] = prox->buf[Y * w + x] - r - 1;

	for (int y = Y + 1; y <  h && prox_active(prox, X, y); y++)
		prox->buf[y * w + X] = prox->buf[y * w + X] - d - 1;

	for (int y = Y - 1; y >= 0 && prox_active(prox, X, y); y--)
		prox->buf[y * w + X] = prox->buf[y * w + X] - u - 1;
}
