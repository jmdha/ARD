#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
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
//SDL_Texture* tex_room;

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
	buf_draw = grid_init(rw, HEIGHT);
	buf_prox = grid_init(rw, HEIGHT);

	tex_draw = tex(renderer, rw, HEIGHT);
	tex_prox = tex(renderer, rw, HEIGHT);
	//tex_room = tex(renderer, rw, HEIGHT);
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
		grid_resize(buf_draw, rw, h);
		grid_resize(buf_prox, rw, h);
		SDL_DestroyTexture(tex_draw);
		SDL_DestroyTexture(tex_prox);
		//SDL_DestroyTexture(tex_room);
		tex_draw = tex(renderer, rw, h);
		tex_prox = tex(renderer, rw, h);
		//tex_room = tex(renderer, w, h);
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
			if (grid_get(draw, x, y) != 0)
				grid_set(buf_draw, x, y, ~((uint32_t) 0));
			else
				grid_set(buf_draw, x, y, 0);
		}
	}
	SDL_FRect rect_draw = {.x = 0 * rw, .y = 0, .w = rw, .h = h};
	SDL_FRect rect_prox = {.x = 1 * rw, .y = 0, .w = rw, .h = h};
	SDL_FRect rect_room = {.x = 2 * rw, .y = 0, .w = rw, .h = h};
  	SDL_RenderClear(renderer);
	SDL_UpdateTexture(tex_draw, NULL, grid_buf(buf_draw), rw * sizeof(uint32_t));
	SDL_UpdateTexture(tex_prox, NULL, grid_buf(buf_prox), rw * sizeof(uint32_t));
	SDL_RenderTexture(renderer, tex_draw, NULL, &rect_draw);
	SDL_RenderTexture(renderer, tex_prox, NULL, &rect_prox);
	//SDL_RenderTexture(renderer, tex_room, NULL, &rect_room);
  	SDL_RenderPresent(renderer);
}

int main() {
  	init();
  	for (struct input in = input_init(); !in.quit; input_load(&in))
  	  	step(&in);
  	fini();
}
