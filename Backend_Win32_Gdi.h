#pragma once

#include <windows.h>
#include <cstdint>

class Gdi {
public:
	Gdi(HDC dc, size_t width, size_t height);
	void fill(uint8_t r, uint8_t g, uint8_t b);
	void set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
	int width() { return m_width; }
	int height() { return m_height; }

private:
	HDC m_dc;
	int m_width;
	int m_height;
};