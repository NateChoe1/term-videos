#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

struct meta {
	int width;
	int height;
	int max;
};

static int convert(FILE *in, FILE *out);
static int convert_func(FILE *in, FILE *out, int (*func)(FILE *file));
static int read_meta(FILE *file, struct meta *ret);
static int read_num(FILE *file);
static int change_clr(unsigned long clr, FILE *out);

int main(int argc, char **argv) {
	FILE *in, *out;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s [file.ppm] [output.txt]\n", argv[0]);
		return 1;
	}

	in = fopen(argv[1], "rb");
	out = fopen(argv[2], "wb");

	if (in == NULL || out == NULL) {
		fputs("Failed to open file\n", stderr);
		return 1;
	}

	return convert(in, out);
}

static int convert(FILE *in, FILE *out) {
	char magic[2];

	if (fread(magic, 1, sizeof magic, in) < sizeof magic) {
		fputs("Failed to read magic bytes\n", stderr);
		return 1;
	}

	if (memcmp(magic, "P6", sizeof magic) == 0) {
		return convert_func(in, out, fgetc);
	}
	else if (memcmp(magic, "P3", sizeof magic) == 0) {
		return convert_func(in, out, read_num);
	}
	else {
		fprintf(stderr, "Unrecognized magic bytes %c%c\n",
				magic[0], magic[1]);
		return 1;
	}
}

#define CHANGE_CLR(clr) \
	if (change_clr(clr, out)) { \
		fputs("Failed to change char color\n", stderr); \
		return 1; \
	}
static int convert_func(FILE *in, FILE *out, int (*func)(FILE *file)) {
	struct meta meta;
	int i, j;
	unsigned long last_clr;
	const unsigned long default_clr = 0x000000;
	if (read_meta(in, &meta)) {
		return 1;
	}

	if (meta.max != 255) {
		fputs("Strange pgm scale detected, quitting\n", stderr);
		return 1;
	}

	last_clr = default_clr;

	fputs("\033[0;0H", out);

	CHANGE_CLR(default_clr);

	for (i = 0; i < meta.height; ++i) {
		for (j = 0; j < meta.width; ++j) {
			int r, g, b;
			unsigned long color;
#define GET_COLOR(var) \
			if ((var = func(in)) == -1) { \
				fputs("Failed to get color value\n", stderr); \
				return 1; \
			}
			GET_COLOR(r);
			GET_COLOR(g);
			GET_COLOR(b);
#undef GET_COLOR

			color = (r << 16) | (b << 8) | g;
			if (color != last_clr) {
				last_clr = color;
				CHANGE_CLR(color);
			}

			fputc(' ', out);
		}
		fputc('\n', out);
	}

	CHANGE_CLR(default_clr);

	return 0;
}
#undef CHANGE_CLR

/* God I love the preprocessor */
#define READ_NUM(var) \
	if ((var = read_num(file)) == -1) { \
		fputs("Failed to read metadata\n", stderr); \
		return 1; \
	}
static int read_meta(FILE *file, struct meta *ret) {
	READ_NUM(ret->width);
	READ_NUM(ret->height);
	READ_NUM(ret->max);
	return 0;
}
#undef READ_NUM

static int read_num(FILE *file) {
	int val;
	for (;;) {
		int c = fgetc(file);
		if (c == EOF) {
			return -1;
		}
		if (c == '#') {
			while (c != '\r' && c != '\n') {
				c = fgetc(file);
			}
		}
		if (isdigit(c)) {
			ungetc(c, file);
			break;
		}
	}

	val = 0;

	for (;;) {
		int c = fgetc(file);
		char *digits = "0123456789";
		if (!isdigit(c)) {
			return val;
		}
		val *= 10;
		/* c - '0' is implementation defined behavior in C :/ */
		val += strchr(digits, c) - digits;
	}
}

static int change_clr(unsigned long clr, FILE *out) {
	unsigned char r, g, b;
	r = (clr >> 16) & 0xff;
	g = (clr >> 8)  & 0xff;
	b = (clr)       & 0xff;

	fprintf(out, "\033]P0%02X%02X%02X", r, g, b);

	return 0;
}
