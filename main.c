#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#include <sys/types.h>
#include <stdbool.h>

#define max(x,y) (((x) >= (y)) ? (x) : (y))
#define min(x,y) (((x) <= (y)) ? (x) : (y))
// I don't know whether I like this.
#define gloop(w, h) for (int x = 0; x < w; x++) for (int y = 0; y < h; y++)

int gmax(const int* buf, int w, int h) {
	int m = 0;
	gloop(w, h)
		m = max(m, buf[y * w + x]);
	return m;
}

void prox(int* out, const int* buf, int w, int h) {
	gloop(w, h) {
		int i = 0;
		do {
			if (!buf[(y + i) * w + (x + i)] ||
			    !buf[(y + i) * w + (x - i)] ||
			    !buf[(y - i) * w + (x + i)] ||
			    !buf[(y - i) * w + (x - i)])
				break;
		} while (++i < min(abs(w - x), abs(h - y)));
		out[y * w + x] = i;
	}
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

uint grid_vals(struct cor** cors, const int* buf, int w, int h) {
	*cors = malloc(w * h * sizeof(struct cor));
	uint i = 0;
	gloop(w, h)
		if (buf[y * w + x]) {
			(*cors)[i].x = x;
			(*cors)[i].y = y;
			(*cors)[i].v = buf[y * w + x];
			i++;
		}
	*cors = realloc(*cors, i * sizeof(struct cor));
	return i;
}

void sect_gen(int* out, const struct cor* cors, int c, int w, int h) {
	memset(out, 0, w * h * sizeof(int));
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

void sect(int* out, const int* buf, uint w, uint h) {
	struct cor* cors;
	const int c = grid_vals(&cors, buf, w, h);
	qsort(cors, c, sizeof(struct cor), cor_comp);
	sect_gen(out, cors, c, w, h);
	free(cors);
}

int save_img(const char* name, const int* buf, int w, int h) {
	unsigned char* out = calloc(w * h, sizeof(unsigned char));
	int m = gmax(buf, w, h);
	printf("%d\n", m);
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
	prox(buf, buf, w, h);
	save_img("prox.png", buf, w, h);
	sect(buf, buf, w, h);
	save_img("sect.png", buf, w, h);
	stbi_image_free(img_buf);
}
