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

struct grid* buf_draw;
struct grid* buf_prox;

SDL_Texture* tex_draw;
SDL_Texture* tex_prox;

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
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(
			NULL, 
			WIDTH, 
			HEIGHT, 
			SDL_WINDOW_RESIZABLE, 
			&window, 
			&renderer
	);
	draw     = grid_init(WIDTH, HEIGHT);
	prox     = prox_init(WIDTH, HEIGHT);
	buf_draw = grid_init(WIDTH, HEIGHT);
	buf_prox = grid_init(WIDTH, HEIGHT);

	tex_draw = tex(renderer, WIDTH, HEIGHT);
	tex_prox = tex(renderer, WIDTH, HEIGHT);
}

void fini() {
  	SDL_DestroyRenderer(renderer);
  	SDL_DestroyWindow(window);
  	SDL_Quit();
}

void step(const struct input *in) {
  	int w, h;
  	SDL_GetWindowSize(window, &w, &h);
	if (in->resize) {
		printf("Resize: %d %d %d\n", w, h, w*h);
		grid_resize(draw, w, h);
		prox_resize(prox, w, h);
		grid_resize(buf_draw, w, h);
		grid_resize(buf_prox, w, h);
		SDL_DestroyTexture(tex_draw);
		SDL_DestroyTexture(tex_prox);
		tex_draw = tex(renderer, w, h);
		tex_prox = tex(renderer, w, h);
	}
	if (in->left) {
		for (int x = -3; x <= 3; x++)
			for (int y = -3; y <= 3; y++) {
				int px = in->mouse_x + x;
				int py = in->mouse_y + y;
				grid_set(draw, px, py, 1);
				prox_set(prox, px, py);
			}
	}
	const uint32_t pm = max(prox_max(prox), 1u);
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			const int   v   = prox_get(prox, x, y);
			const float m   = (float) v / pm;
			const int   col = m * 255;
			grid_set(buf_prox, x, y, (255 << 24) | col);
			if (grid_get(draw, x, y) != 0)
				grid_set(buf_draw, x, y, ~((uint32_t) 0));
			else
				grid_set(buf_draw, x, y, 0);
		}
	}
	SDL_FRect rect = {.x = 0, .y = 0, .w = 100, .h = 100};
  	SDL_RenderClear(renderer);
	SDL_UpdateTexture(tex_draw, NULL, grid_buf(buf_draw), w * sizeof(uint32_t));
	SDL_UpdateTexture(tex_prox, NULL, grid_buf(buf_prox), w * sizeof(uint32_t));
	SDL_RenderTexture(renderer, tex_draw, NULL, NULL);
	SDL_RenderTexture(renderer, tex_prox, NULL, &rect);
  	SDL_RenderPresent(renderer);
}

int main() {
  	init();
  	for (struct input in = input_init(); !in.quit; input_load(&in))
  	  	step(&in);
  	fini();
}
