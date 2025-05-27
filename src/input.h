#pragma once

#include <stdbool.h>

struct input {
	bool quit;
	bool left, right;
	bool reset;
	bool resize;
	bool draw_increase, draw_decrease;
	float mouse_x, mouse_y;
};

struct input input_init();
void         input_load(struct input *in);
