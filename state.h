#pragma once

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

struct State {
public:
	State(int count, float spread, float speed)
	: m_spread(spread)
	, m_speed(speed)
	, m_starsX(count)
	, m_starsY(count)
	, m_starsZ(count)
	{
		for (int i = 0; i < count; i++) {
			init_star(i);
		}
	}

	template<class Backend>
	void update_and_draw(Backend &backend, size_t delta_ns)
	{
		float delta = static_cast<double>(delta_ns) / 10000000.0;
		float half_width = static_cast<double>(backend.width()) / 2.0;
		float half_height = static_cast<double>(backend.height()) / 2.0;
		backend.fill(0, 0, 0);
		for (int i = 0; i < m_starsX.size(); i++) {
			m_starsZ[i] -= delta * m_speed;
			if (m_starsZ[i] <= 0) {
				init_star(i);
			}
			int x = (m_starsX[i] / m_starsZ[i]) * half_width + half_width;
			int y = (m_starsY[i] / m_starsZ[i]) * half_height + half_height;
			if (x < 0 || x >= backend.width() || y < 0 || y >= backend.height()) {
				init_star(i);
			} else {
				backend.set_pixel(x, y, 255, 255, 255);
			}
		}

		/* Draw FPS. */
		double frame_time_ms = static_cast<double>(delta_ns) / 1000000.0;
		size_t fps = 1000.0 / frame_time_ms;
		char buf[256];
		snprintf(buf, sizeof(buf), "%lu", fps);
		backend.draw_text(0, 0, buf);
	}

private:
	void init_star(size_t i)
	{
		m_starsX[i] = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5) * 2 * m_spread;
		m_starsY[i] = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5) * 2 * m_spread;
		m_starsZ[i] = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) + 0.00001) * m_spread;
	}

private:
	float m_spread;
	float m_speed;
	std::vector<float> m_starsX;
	std::vector<float> m_starsY;
	std::vector<float> m_starsZ;
};