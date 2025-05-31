#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

#include "misc.h"
#include "grid.h"
#include "input.h"
#include "prox.h"

const int WIDTH = 640;
const int HEIGHT = 480;

SDL_Window *window;
SDL_Renderer *renderer;

struct grid* draw;
struct prox* prox;
struct grid* room;

struct grid* buf_draw;
struct grid* buf_prox;
struct grid* buf_room;

SDL_Texture* tex_draw;
SDL_Texture* tex_prox;
SDL_Texture* tex_room;

	struct cor {
		uint32_t v;
		int x, y;
	};
	
	int comp(const void *a, const void *b) {
		const struct cor *pa = a;
		const struct cor *pb = b;
		if (pa->v > pb->v)
			return -1;
		else if (pa->v < pb->v)
			return 1;
		else
			return 0;
	}

SDL_Texture* tex(SDL_Renderer* r, int w, int h) {
	return SDL_CreateTexture(
			r, 
			SDL_PIXELFORMAT_ARGB8888, 
			SDL_TEXTUREACCESS_STREAMING, 
			w, 
			h
	);
}

void init() {
	int rw = WIDTH / 3;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(
			NULL, 
			WIDTH, 
			HEIGHT, 
			SDL_WINDOW_RESIZABLE, 
			&window, 
			&renderer
	);
	draw     = grid_init(rw, HEIGHT);
	prox     = prox_init(rw, HEIGHT);
	room     = grid_init(rw, HEIGHT);
	buf_draw = grid_init(rw, HEIGHT);
	buf_prox = grid_init(rw, HEIGHT);
	buf_room = grid_init(rw, HEIGHT);

	tex_draw = tex(renderer, rw, HEIGHT);
	tex_prox = tex(renderer, rw, HEIGHT);
	tex_room = tex(renderer, rw, HEIGHT);
}

void fini() {
  	SDL_DestroyRenderer(renderer);
  	SDL_DestroyWindow(window);
  	SDL_Quit();
}

void step(const struct input *in) {
  	int w, h;
  	SDL_GetWindowSize(window, &w, &h);
	int rw = w / 3;
	if (in->resize) {
		grid_resize(draw, rw, h);
		prox_resize(prox, rw, h);
		grid_resize(room, rw, h);
		grid_resize(buf_draw, rw, h);
		grid_resize(buf_prox, rw, h);
		grid_resize(buf_room, rw, h);
		SDL_DestroyTexture(tex_draw);
		SDL_DestroyTexture(tex_prox);
		SDL_DestroyTexture(tex_room);
		tex_draw = tex(renderer, rw, h);
		tex_prox = tex(renderer, rw, h);
		tex_room = tex(renderer, rw, h);
	}
	if (in->left) {
		for (int x = -3; x <= 3; x++)
			for (int y = -3; y <= 3; y++) {
				int px = ((int) in->mouse_x % rw) + x;
				int py = in->mouse_y + y;
				grid_set(draw, px, py, 1);
				prox_set(prox, px, py);
			}
	}
	const uint32_t pm = max(prox_max(prox), 1u);
	for (int x = 0; x < rw; x++) {
		for (int y = 0; y < h; y++) {
			const int   v   = prox_get(prox, x, y);
			const float m   = (float) v / pm;
			const int   col = m * 255;
			grid_set(buf_prox, x, y, (255 << 24) | col);
			grid_set(buf_room, x, y, 0);
			if (grid_get(draw, x, y) != 0)
				grid_set(buf_draw, x, y, ~((uint32_t) 0));
			else
				grid_set(buf_draw, x, y, 0);
		}
	}
	const uint32_t p_count = prox_count(prox);
	struct cor *cors = NULL;
	if (p_count > 0)
		cors = calloc(p_count, sizeof(struct cor));
	uint32_t r_count = 0;
	for (int x = 0; x < rw; x++)
		for (int y = 0; y < h; y++)
			if (prox_active(prox, x, y)) {
				cors[r_count].v = prox_get(prox, x, y);
				cors[r_count].x = x;
				cors[r_count].y = y;
				r_count++;
			}
	if (r_count > 0)
		qsort(cors, r_count, sizeof(struct cor), comp);

	uint32_t rooms = 0;
	for (uint32_t i = 0; i < r_count; i++) {
		const struct cor *c = &cors[i];
		if (grid_get(buf_room, c->x, c->y) != 0)
			continue;
		for (int ox = -((int)c->v); ox <= (int) c->v; ox++) {
			for (int oy = -((int)c->v); oy <= (int) c->v; oy++) {
				int x = c->x + ox;
				int y = c->y + oy;
				if (x < 0 ||
				    y < 0 ||
				    x >= grid_width(buf_room) ||
				    y >= grid_height(buf_room) ||
				    grid_get(buf_room, x, y) != 0)
					continue;
				grid_set(buf_room, x, y, rooms + 1);
			}
		}
		rooms++;
	}
	printf("%d/%d-%d\n", rooms, p_count, r_count);
	for (uint32_t x = 0; x < grid_width(buf_room); x++)
		for (uint32_t y = 0; y < grid_height(buf_room); y++)
			if (grid_get(buf_room, x, y) < 5) {
				int val = 255.0f / min(5, rooms) * grid_get(buf_room, x, y);
				grid_set(buf_room, x, y, (255 << 24) | val);
			}
	if (cors)
		free(cors);
	SDL_FRect rect_draw = {.x = 0 * rw, .y = 0, .w = rw, .h = h};
	SDL_FRect rect_prox = {.x = 1 * rw, .y = 0, .w = rw, .h = h};
	SDL_FRect rect_room = {.x = 2 * rw, .y = 0, .w = rw, .h = h};
  	SDL_RenderClear(renderer);
	SDL_UpdateTexture(tex_draw, NULL, grid_buf(buf_draw), rw * sizeof(uint32_t));
	SDL_UpdateTexture(tex_prox, NULL, grid_buf(buf_prox), rw * sizeof(uint32_t));
	SDL_UpdateTexture(tex_room, NULL, grid_buf(buf_room), rw * sizeof(uint32_t));
	SDL_RenderTexture(renderer, tex_draw, NULL, &rect_draw);
	SDL_RenderTexture(renderer, tex_prox, NULL, &rect_prox);
	SDL_RenderTexture(renderer, tex_room, NULL, &rect_room);
  	SDL_RenderPresent(renderer);
}

int main() {
  	init();
  	for (struct input in = input_init(); !in.quit; input_load(&in))
  	  	step(&in);
  	fini();
}
