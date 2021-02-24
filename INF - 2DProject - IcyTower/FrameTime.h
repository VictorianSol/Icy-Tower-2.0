#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

class FrameTime{
public:
	void restart() { clock.restart(); }
	float average() const {
		float avg = 0.f;
		int i = 0;
		for (auto a = frametime.begin(); a < frametime.end(); ++a, ++i)
			avg += *a;
		return avg / i;
	}
	void operator ++ (int) {
		frametime.push_back(std::min(clock.restart().asSeconds(), 0.0333333333f));
		if (frametime.size() > 30)
			frametime.erase(frametime.begin());
	}
	float avgConv() const { return 111.f * average(); }
	float fps() const { return 1.f / average(); }

private:
	std::vector<float> frametime;
	sf::Clock clock;
};