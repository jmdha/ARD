#include <stdlib.h>

#include "utils.h"
#include "misc.h"

unsigned int dist_horizontal(int w, int h, int a, int b) {
	(void) h;
	return abs(a % w - b % w);
}

unsigned int dist_vertical(int w, int h, int a, int b) {
	(void) h;
	return abs((a - a % w) / w - (b - b % w) / w);
}

unsigned int dist_manhattan(int w, int h, int a, int b) {
	return dist_horizontal(w, h, a, b) + dist_vertical(w, h, a, b);
}

unsigned int dist_chebyshev(int w, int h, int a, int b) {
	return max(dist_horizontal(w, h, a, b), dist_vertical(w, h, a, b));
}
