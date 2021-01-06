#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

class FrameTime{
public:
	void restart() {
		clock.restart();
	}
	float average() {
		float avg = 0.f;
		int i = 0;
		for (auto a = frametime.begin(); a < frametime.end(); ++a, ++i)
			avg += *a;
		return avg / i;
	}
	void operator ++ (int) {
		frametime.push_back(clock.restart().asSeconds());
		if (frametime.size() > 30)
			frametime.erase(frametime.begin());
	}

private:
	vector<float> frametime;
	sf::Clock clock;
};