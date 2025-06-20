#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#include <sys/types.h>
#include <stdbool.h>

#define max(x,y) (((x) >= (y)) ? (x) : (y))
#define min(x,y) (((x) <= (y)) ? (x) : (y))

uint gmax(const uint* buf, int w, int h) {
	int m = 0;
	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
			m = max(m, buf[y * w + x]);
	return m;
}

uint pval(const uint* buf, int w, int h, int x, int y) {
	for (int i = 0; i < min(abs(w - x), abs(h - y)); i++)
		if (!buf[(y + i) * w + (x + i)] ||
		    !buf[(y + i) * w + (x - i)] ||
		    !buf[(y - i) * w + (x + i)] ||
		    !buf[(y - i) * w + (x - i)])
			return i;
	return min(abs(w - x), abs(h - y));
}

void prox(uint* out, const uint* buf, int w, int h) {
	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
			out[y * w + x] = pval(buf, w, h, x, y);
}

struct cor {
	int x;
	int y;
	int v;
};

int cor_comp(const void* a, const void* b) {
	if (((struct cor*)a)->v > ((struct cor*)b)->v)
		return -1;
	if (((struct cor*)a)->v < ((struct cor*)b)->v)
		return 1;
	else
		return 0;
}

uint grid_vals(struct cor** cors, const uint* buf, uint w, uint h) {
	*cors = malloc(w * h * sizeof(struct cor));
	uint i = 0;
	for (uint x = 0; x < w; x++)
		for (uint y = 0; y < h; y++)
			if (buf[y * w + x] != 0) {
				(*cors)[i].x = x;
				(*cors)[i].y = y;
				(*cors)[i].v = buf[y * w + x];
				i++;
			}
	*cors = realloc(*cors, i * sizeof(struct cor));
	return i;
}

void sect_gen(uint* out, const struct cor* cors, int c, int w, int h) {
	memset(out, 0, w * h * sizeof(uint));
	int s = 1;
	for (int i = 0; i < c; i++) {
		const struct cor* cor = &cors[i];
		const bool novel = !out[cor->y * w + cor->x];
		const int val = novel ? s : out[cor->y * w + cor->x];
		for (int x = max(cor->x - cor->v, 0); x <= min(cor->x + cor->v, w); x++)
			for (int y = max(cor->y - cor->v, 0); y <= min(cor->y + cor->v, h); y++)
				if (!out[y * w + x])
					out[y * w + x] = val;
		if (novel)
			s++;
	}
}

void sect(uint* out, const uint* buf, uint w, uint h) {
	struct cor* cors;
	const uint c = grid_vals(&cors, buf, w, h);
	qsort(cors, c, sizeof(struct cor), cor_comp);
	sect_gen(out, cors, c, w, h);
	free(cors);
}

int save_img(const char* name, const uint* buf, uint w, uint h) {
	unsigned char* out = calloc(w * h, sizeof(unsigned char));
	uint m = gmax(buf, w, h);
	printf("%d\n", m);
	for (uint x = 0; x < (uint) w; x++)
		for (uint y = 0; y < (uint) h; y++)
			out[y * w + x] = 255 * ((float) buf[y * w + x] / m);
	return stbi_write_png(name, w, h, 1, out, w * sizeof(unsigned char));
}

int main(int, char** argv) {
	stbi_write_png_compression_level = 0;
	int w, h, n;
	unsigned char* img_buf = stbi_load(argv[1], &w, &h, &n, 1);
	if (!img_buf) {
		printf("Failed to load file\n");
		return 1;
	}
	uint* buf = malloc(w * h * sizeof(uint));
	for (int i = 0; i < w * h; i++)
		buf[i] = img_buf[i];
	prox(buf, buf, w, h);
	save_img("prox.png", buf, w, h);
	sect(buf, buf, w, h);
	save_img("sect.png", buf, w, h);
	stbi_image_free(img_buf);
}
