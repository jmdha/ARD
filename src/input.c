#include <SDL3/SDL.h>

#include "input.h"

struct input input_init() {
  struct input in;
  in.quit          = false;
  in.left          = false;
  in.right         = false;
  in.reset         = false;
  in.resize        = false;
  in.draw_increase = false;
  in.draw_decrease = false;
  return in;
}

void input_load(struct input *in) {
  in->quit          = false;
  in->reset         = false;
  in->resize        = false;
  in->draw_increase = false;
  in->draw_decrease = false;

  SDL_GetMouseState(&in->mouse_x, &in->mouse_y);

  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_EVENT_QUIT:              in->quit          = true;  break;
    case SDL_EVENT_WINDOW_RESIZED:    in->resize        = true;  break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN: 
	switch (e.button.button) {
		case 1: in->left  = true; break;
		case 3: in->right = true; break;
		default: break;
	}
	break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
	switch (e.button.button) {
		case 1: in->left  = false; break;
		case 3: in->right = false; break;
		default: break;
	}
	break;
    case SDL_EVENT_KEY_DOWN:
        switch (e.key.key) {
          case SDLK_Q:      in->quit          = true; break;
          case SDLK_F1:     in->reset         = true; break;
          case SDLK_PERIOD: in->draw_increase = true; break;
          case SDLK_COMMA:  in->draw_decrease = true; break;
        }
    }
  }
}
