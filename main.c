#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#include "sort_r.h"
#include <sys/types.h>
#include <stdbool.h>

#define max(x,y) (((x) >= (y)) ? (x) : (y))
#define min(x,y) (((x) <= (y)) ? (x) : (y))
// I don't know whether I like this.
#define gloop(w, h)        for (int x = 0; x < w; x++)  for (int y = 0; y < h; y++)
#define gloopi(w, W, h, H) for (int x = w; x <= W; x++) for (int y = h; y <= H; y++)

int prox(int* buf, int w, int h) {
	int m_val = -INT_MAX;
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
		if (i > m_val)
			m_val = i;
	}
	return m_val;
}

int cmp(const void* a, const void* b, void* arg) {
	return (((int*)arg)[*(int*)a] < ((int*)arg)[*(int*)b]) - (((int*)arg)[*(int*)a] > ((int*)arg)[*(int*)b]);
}

int sect(int* buf, int w, int h) {
	int* tmp  = calloc(w * h, sizeof(int));
	int* indi = malloc(w * h * sizeof(int));
	for (int i = 0; i < w * h; i++)
		indi[i] = i;
	sort_r(indi, w * h, sizeof(int), cmp, buf);
	int r = 1;
	for (int i = 0; i < w * h && buf[indi[i]]; i++) {
		const int ox = indi[i] % w;
		const int oy = (indi[i] - ox) / w; 
		const int v  = buf[indi[i]];
		const int iv = tmp[indi[i]] ? tmp[indi[i]] : r;
		gloopi(max(ox - v, 0), min(ox + v, w), max(oy - v, 0), min(oy + v, h))
			if (!tmp[y * w + x])
				tmp[y * w + x] = iv;
		if (iv == r) r++;
	}
	memcpy(buf, tmp, w * h * sizeof(int));
	return r;
}

int save_img(const char* name, const int* buf, int w, int h, int m) {
	unsigned char* out = calloc(w * h, sizeof(unsigned char));
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
	int p_max = prox(buf, w, h);
	save_img("prox.png", buf, w, h, p_max);
	int s_max = sect(buf, w, h);
	save_img("sect.png", buf, w, h, s_max);
	stbi_image_free(img_buf);
}
