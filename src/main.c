#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "prox.h"

const int WIDTH = 640;
const int HEIGHT = 480;

SDL_Window *window;
SDL_Renderer *renderer;

struct prox* prox;
uint32_t*    buf;
SDL_Texture* tex;

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
	prox = prox_init(WIDTH, HEIGHT);
	buf  = calloc(WIDTH * HEIGHT, sizeof(uint32_t));
	tex  = SDL_CreateTexture(
			renderer, 
			SDL_PIXELFORMAT_ARGB8888, 
			SDL_TEXTUREACCESS_STREAMING, 
			WIDTH, 
			HEIGHT
	);
}

void fini() {
	prox_free(prox);
	free(buf);
	SDL_DestroyTexture(tex);
  	SDL_DestroyRenderer(renderer);
  	SDL_DestroyWindow(window);
  	SDL_Quit();
}

void step(const struct input *in) {
  	int w, h;
  	SDL_GetWindowSize(window, &w, &h);
	if (in->left) {
		for (int x = -3; x <= 3; x++)
			for (int y = -3; y <= 3; y++) {
				int px = in->mouse_x + x;
				int py = in->mouse_y + y;
				if (!prox_active(prox, px, py))
					prox_set(prox, px, py);
			}
	}
	const uint32_t max = prox_max(prox);
	if (max > 0) {
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				const int   v   = prox_get(prox, x, y);
				const float m   = (float) v / max;
				const int   col = m * 255;
				buf[y * w + x] = (255 << 24) | col;
			}
		}
	}
  	SDL_RenderClear(renderer);
	SDL_UpdateTexture(tex, NULL, buf, WIDTH * sizeof(uint32_t));
	SDL_RenderTexture(renderer, tex, NULL, NULL);
  	SDL_RenderPresent(renderer);
}

int main() {
  	init();
  	for (struct input in = input_init(); !in.quit; input_load(&in))
  	  	step(&in);
  	fini();
}
