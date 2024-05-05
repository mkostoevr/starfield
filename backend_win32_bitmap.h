#include "backend_generic.h"

class Backend_Win32_Bitmap: public Backend_Generic {
	UINT8 *m_buf;
	SIZE_T m_buf_size;
	INT m_w, m_h, m_rw;

public:
	Backend_Win32_Bitmap(UINT8 *buf, SIZE_T buf_size, INT w, INT h, INT rw)
	: m_buf(buf), m_buf_size(buf_size), m_w(w), m_h(h), m_rw(rw) {}

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
		m_buf[y * m_rw * 4 + x * 4] = B;
		m_buf[y * m_rw * 4 + x * 4 + 1] = G;
		m_buf[y * m_rw * 4 + x * 4 + 2] = R;
		m_buf[y * m_rw * 4 + x * 4 + 3] = 0;
	}

	VOID clear() override
	{
		memset(m_buf, 0xff, m_buf_size);
	}
};