#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "layer.h"

// Checks whether a point is in bounds.
// Specifically whether x/y plus ox/oy is over or equal 0 and under w/h.
bool in(uint w, uint h, uint x, uint y, int ox, int oy) {
	if (ox > 0 && x + ox >= w)
		return false;
	if (oy > 0 && y + oy >= h)
		return false;
	if (ox < 0 && (uint) abs(ox) > x)
		return false;
	if (oy < 0 && (uint) abs(oy) > y)
		return false;
	return true;
}

// Checks whether a point offset by i hits unset cell or bounds.
bool phit(const uint* buf, uint w, uint h, uint x0, uint y0, uint i) {
	for (int mx = -1; mx <= 1; mx++) {
		for (int my = -1; my <= 1; my++) {
			const int ox = mx * i;
			const int oy = my * i;
			if (!in(w, h, x0, y0, ox, oy))
				return true;
			const uint x = x0 + ox;
			const uint y = y0 + oy;
			if (buf[y * w + x] == 0)
				return true;
		}
	}
	return false;
}

uint pval(const uint* buf, uint w, uint h, uint x, uint y) {
	for (uint i = 1; i < ~(uint)0; i++)
		if (phit(buf, w, h, x, y, i))
			return i;
	__builtin_unreachable();
}

void prox(uint* out, const uint* buf, uint w, uint h) {
	for (uint x = 0; x < w; x++)
		for (uint y = 0; y < h; y++)
			if (buf[y * w + x] == 0)
				out[y * w + x] = 0;
			else
				out[y * w + x] = pval(buf, w, h, x, y);
}

struct cor {
	uint x;
	uint y;
	uint v;
};

// For sorting biggest first
int cor_comp(const void* a, const void* b) {
	if (((struct cor*)a)->v > ((struct cor*)b)->v)
		return -1;
	if (((struct cor*)a)->v < ((struct cor*)b)->v)
		return 1;
	else
		return 0;
}

// Counts the number of non-zero values
uint set_count(const uint* buf, uint w, uint h) {
	uint v = 0;
	for (uint x = 0; x < w; x++)
		for (uint y = 0; y < h; y++)
			if (buf[y * w + x] != 0)
				v++;
	return v;
}

// Finds the value of each non-zero coordinate
uint grid_vals(struct cor** cors, const uint* buf, uint w, uint h) {
	const uint c = set_count(buf, w, h);
	*cors = malloc(c * sizeof(struct cor));
	uint i = 0;
	for (uint x = 0; x < w; x++)
		for (uint y = 0; y < h; y++)
			if (buf[y * w + x] != 0) {
				(*cors)[i].x = x;
				(*cors)[i].y = y;
				(*cors)[i].v = buf[y * w + x];
				i++;
			}
	return c;
}

// Splits the grid into sectors
void sect_gen(uint* out, const struct cor* cors, uint c, uint w, uint h) {
	memset(out, 0, w * h * sizeof(uint));
	uint s = 1;
	for (uint i = 0; i < c; i++) {
		const struct cor* cor = &cors[i];
		const bool novel = out[cor->y * w + cor->x] == 0;
		const uint val = novel ? s : out[cor->y * w + cor->x];
		for (int ox = -((int)cor->v); ox <= (int) cor->v; ox++)
			for (int oy = -((int)cor->v); oy <= (int) cor->v; oy++) {
				const uint x = cor->x + ox;
				const uint y = cor->y + oy;
				if (in(w, h, cor->x, cor->y, ox, oy) && !out[y * w + x])
					out[y * w + x] = val;
			}
		if (novel)
			s++;
	}
}

// Splits the grid into sectors
void sect(uint* out, const uint* buf, uint w, uint h) {
	struct cor* cors;
	const uint c = grid_vals(&cors, buf, w, h);
	qsort(cors, c, sizeof(struct cor), cor_comp);
	sect_gen(out, cors, c, w, h);
	free(cors);
}

void room(uint* out, const uint* buf, uint w, uint h) {

}
