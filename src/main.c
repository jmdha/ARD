#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>

#include "grid.h"
#include "layer.h"

int save_img(const char* name, const struct grid* grid, uint w, uint h) {
	unsigned char* out = calloc(w * h, sizeof(unsigned char));
	uint m = grid_max(grid);
	for (uint x = 0; x < (uint) w; x++)
		for (uint y = 0; y < (uint) h; y++)
			out[y * w + x] = 255 * ((float) grid_get(grid, x, y) / m);
	stbi_write_png(name, w, h, 1, out, w * sizeof(unsigned char));
	printf("%d\n", grid_max(grid));
}

int main(int argc, char** argv) {
	stbi_write_png_compression_level = 0;
	int w, h, n;
	unsigned char* img_buf = stbi_load(argv[1], &w, &h, &n, 1);
	if (!img_buf) {
		printf("Failed to load file\n");
		return 1;
	}
	struct grid* grid = grid_init(w, h);
	for (uint x = 0; x < (uint) w; x++)
		for (uint y = 0; y < (uint) h; y++)
			if (img_buf[y * w + x])
				grid_set(grid, x, y, 1);
	prox(grid_buf(grid), grid_buf(grid), w, h);
	save_img("prox.png", grid, w, h);
	sect(grid_buf(grid), grid_buf(grid), w, h);
	save_img("sect.png", grid, w, h);
	stbi_image_free(img_buf);
}
