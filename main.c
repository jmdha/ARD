#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#include <sys/types.h>
#include <stdbool.h>

#define max(x,y) (((x) >= (y)) ? (x) : (y))
#define min(x,y) (((x) <= (y)) ? (x) : (y))
// I don't know whether I like this.
#define gloop(w, h)          for (int x = 0; x < w; x++) for (int y = 0; y < h; y++)
#define gloopi(xi, w, yi, h) for (int x = xi; x <= w; x++) for (int y = yi; y <= h; y++)

int gmax(int* ox, int* oy, const int* buf, int w, int h) {
	int m = 0;
	gloop(w, h)
		if (buf[y * w + x] > m) {
			m   = buf[y * w + x];
			*ox = x;
			*oy = y;
		}
	return m;
}

void prox(int* buf, int w, int h) {
	gloop(w, h) {
		int i = 0;
		do {
			if (!buf[(y + i) * w + (x + i)] ||
			    !buf[(y + i) * w + (x - i)] ||
			    !buf[(y - i) * w + (x + i)] ||
			    !buf[(y - i) * w + (x - i)])
				break;
		} while (++i < min(abs(w - x), abs(h - y)));
		buf[y * w + x] = i;
	}
}

void sect(int* buf, int w, int h) {
	int* tmp = calloc(w * h, sizeof(int));
	int i = 1;
	int v, gx, gy;
	while ((v = gmax(&gx, &gy, buf, w, h)) > 0) {
		int iv = tmp[gy * w + gx] ? tmp[gy * w + gx] : i;
		gloopi(max(gx - v, 0), min(gx + v, w), max(gy - v, 0), min(gy + v, h))
			if (!tmp[y * w + x])
				tmp[y * w + x] = iv;
		buf[gy * w + gx] = 0;
		if (iv == i)
			i++;
	}
	memcpy(buf, tmp, w * h * sizeof(int));
}

int save_img(const char* name, const int* buf, int w, int h) {
	unsigned char* out = calloc(w * h, sizeof(unsigned char));
	int gx, gy;
	int m = gmax(&gx, &gy, buf, w, h);
	gloop(w, h)
		out[y * w + x] = 255 * ((float) buf[y * w + x] / m);
	return stbi_write_png(name, w, h, 1, out, w * sizeof(unsigned char));
}

int main(__attribute__((unused)) int argc, char** argv) {
	stbi_write_png_compression_level = 0;
	int w, h, n;
	unsigned char* img_buf = stbi_load(argv[1], &w, &h, &n, 1);
	if (!img_buf) {
		printf("Failed to load file\n");
		return 1;
	}
	int* buf = malloc(w * h * sizeof(uint));
	for (int i = 0; i < w * h; i++)
		buf[i] = img_buf[i];
	prox(buf, w, h);
	save_img("prox.png", buf, w, h);
	sect(buf, w, h);
	save_img("sect.png", buf, w, h);
	stbi_image_free(img_buf);
}
