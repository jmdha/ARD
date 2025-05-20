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

void prox_set(struct prox* prox, int x, int y) {
	const int      w = prox_width(prox);
	const int      h = prox_height(prox);
	const uint32_t r = count(prox, x, y,  1,  0);
	const uint32_t l = count(prox, x, y, -1,  0);
	const uint32_t u = count(prox, x, y,  0,  1);
	const uint32_t d = count(prox, x, y,  0, -1);

	prox->buf[y * w + x] = (1 << 31) | (u + d + r + l);

	for (int m = 1; x + m <  w && prox_active(prox, x + m, y); m++)
		prox->buf[y * w + (x + m)] = prox->buf[y * w + (x + m)] + l + 1;

	for (int m = 1; x - m >= 0 && prox_active(prox, x - m, y); m++)
		prox->buf[y * w + (x - m)] = prox->buf[y * w + (x - m)] + r + 1;

	for (int m = 1; y + m <  h && prox_active(prox, x, y + m); m++)
		prox->buf[(y + m) * w + x] = prox->buf[(y + m) * w + x] + d + 1;

	for (int m = 1; y - m >= 0 && prox_active(prox, x, y - m); m++)
		prox->buf[(y - m) * w + x] = prox->buf[(y - m) * w + x] + u + 1;
}
