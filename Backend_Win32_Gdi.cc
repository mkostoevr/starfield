#include "Backend_Win32_Gdi.h"

Gdi::Gdi(HDC dc, size_t width, size_t height)
: m_dc(dc), m_width(width), m_height(height)
{

}

void Gdi::fill(uint8_t r, uint8_t g, uint8_t b)
{
	/* TODO: Can this be done once? */
    SelectObject(m_dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(m_dc, RGB(r, g, b));
    Rectangle(m_dc, 0, 0, m_width, m_height);
}

void Gdi::set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
	SetPixel(m_dc, x, y, RGB(r, g, b));
}

void Gdi::draw_text(int x, int y, const char *text)
{
	TextOutA(m_dc, x, y, text, strlen(text));
}
