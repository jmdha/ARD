#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>
#include <sys/types.h>

#include "layer.h"

uint max(const uint* buf, uint w, uint h) {
	uint m = 0;
	for (uint x = 0; x < w; x++)
		for (uint y = 0; y < h; y++)
			if (buf[y * w + x] > m)
				m = buf[y * w + x];
	return m;
}

int save_img(const char* name, const uint* buf, uint w, uint h) {
	unsigned char* out = calloc(w * h, sizeof(unsigned char));
	uint m = max(buf, w, h);
	for (uint x = 0; x < (uint) w; x++)
		for (uint y = 0; y < (uint) h; y++)
			out[y * w + x] = 255 * ((float) buf[y * w + x] / m);
	return stbi_write_png(name, w, h, 1, out, w * sizeof(unsigned char));
}

int main(int argc, char** argv) {
	stbi_write_png_compression_level = 0;
	int w, h, n;
	unsigned char* img_buf = stbi_load(argv[1], &w, &h, &n, 1);
	if (!img_buf) {
		printf("Failed to load file\n");
		return 1;
	}
	uint* buf = calloc(w * h, sizeof(uint));
	for (uint x = 0; x < (uint) w; x++)
		for (uint y = 0; y < (uint) h; y++)
			if (img_buf[y * w + x])
				buf[y * w + x] = 1;
	prox(buf, buf, w, h);
	save_img("prox.png", buf, w, h);
	sect(buf, buf, w, h);
	save_img("sect.png", buf, w, h);
	stbi_image_free(img_buf);
}
