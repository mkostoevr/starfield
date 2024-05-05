#include "backend_generic.h"

class Backend_Win32_Gdi32: public Backend_Generic {
	HDC m_dc;
	INT m_w, m_h;

public:
	Backend_Win32_Gdi32(HDC dc, INT w, INT h)
	: m_dc(dc), m_w(w), m_h(h) {}

	INT width() override
	{
		return m_w;
	}

	INT height() override
	{
		return m_h;
	}

	VOID set_pixel(INT x, INT y, UINT8 R, UINT8 G, UINT8 B) override
	{
		SetPixel(m_dc, x, y, RGB(R, G, B));
	}

	VOID fill(UINT8 R, UINT8 G, UINT8 B) override
	{
		/* TODO: Can this be done once? */
		SelectObject(m_dc, GetStockObject(DC_BRUSH));
		SetDCBrushColor(m_dc, RGB(R, G, B));
		Rectangle(m_dc, 0, 0, m_w, m_h);
	}

	VOID draw_text(INT x, INT y, LPCSTR text) override
	{
		TextOutA(m_dc, x, y, text, strlen(text));
	}
};