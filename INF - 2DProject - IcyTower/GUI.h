#pragma once
#include <SFML\Graphics.hpp>
#include "CameraView.h"
#include "Player.h"
#include "FrameTime.h"

#define DEV_SIZE 3
#define WALL_OFFSET 80.f

using namespace sf;

class CameraView;
class Player;
class FrameTime;

class GUI{
public:
	GUI();
	void update(CameraView& view, Player& player, FrameTime& deltaTime);
	void draw(RenderWindow& window);
	void devOptions() { devToggle = !devToggle; }

private:
	Font font;
	Text currentLevel;
	char level[30];

	bool devToggle;
	Text devText[DEV_SIZE];
	char devString[DEV_SIZE][100];
	void devOptions(CameraView& view, Player& player, FrameTime& deltaTime);
};

