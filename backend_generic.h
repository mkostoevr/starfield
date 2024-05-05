#pragma once

#include <cmath>

#include "utils.h"

class Backend_Generic {
public:
	virtual ~Backend_Generic() {};

	virtual void set_pixel(INT x, INT y, UINT8 R, UINT8 G, UINT8 B) = 0;
	virtual int width() = 0;
	virtual int height() = 0;

	virtual void fill(uint8_t R, uint8_t G, uint8_t B)
	{
		for (int x = 0; x < width(); x++) {
			for (int y = 0; y < height(); y++) {
				set_pixel(x, y, R, G, B);
			}
		}
	}

	virtual void clear()
	{
		fill(255, 255, 255);
	}

	virtual void circle(int x, int y, int r, uint8_t R, uint8_t G, uint8_t B)
	{
		for (int i = -r + x; i <= r + x; i++) {
			for (int j = -r + y; j <= r + y; j++) {
				if ((i - x) * (i - x) + (j - y) * (j - y) < r * r) {
					if (i < width() && j < height()) {
						set_pixel(i, j, R, G, B);
					}
				}
			}
		}
	}

	virtual void line(int x0, int y0, int x1, int y1, uint8_t R, uint8_t G, uint8_t B)
	{
		double x = x0;
		double y = y0;
		int x_diff = abs(x1 - x0);
		int y_diff = abs(y1 - y0);
		double x_dir = x0 < x1 ? 1.0 : -1.0;
		double y_dir = y0 < y1 ? 1.0 : -1.0;
		int step_count = max(x_diff, y_diff);
		double x_step = (double)x_diff / (double)step_count * x_dir;
		double y_step = (double)y_diff / (double)step_count * y_dir;

		for (int i = 0; i < step_count + 1; i++) {
			if (x < width() && y < height()) {
				set_pixel(x, y, R, G, B);
			}
			x += x_step;
			y += y_step;
		}
	}

	virtual void draw_text(INT x, INT y, const char *text)
	{
		/* TODO: Implement this. */
	}
};