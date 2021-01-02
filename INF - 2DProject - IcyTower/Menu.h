#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <SFML\Graphics.hpp>
#include "CameraView.h"

#define MAX_MENUPOS 5
#define DIFFICULTY_COUNT 3
#define WALL_OFFSET 80.f

using namespace sf;
using namespace std;

class CameraView;

class Menu{
public:
	Menu(CameraView& view, string type);
	bool loop(RenderWindow& window, CameraView& view, Player& player, Platforms& platforms, Walls& walls);

private:
	void draw(RenderWindow& window);
	bool move(RenderWindow& window, CameraView& view);
	string loadDifficulty();
	int getPlaycount();
	string getPlaytime();
	bool saveHighscore(Platforms& platforms, int deathLevel);

	string type;
	int menuPosCount;
	int menuPos;
	string exitTag;

	Font font;
	Text menuTitle;
	string title;
	Text menu[MAX_MENUPOS];
	string menuString[MAX_MENUPOS];
	Text currentLevel;
	Text highScores[DIFFICULTY_COUNT];
	ConvexShape star;
	Text help;
	Text playtime;
	string resolutionS[4] = {
		"Resolution: 400 x 400",
		"Resolution: 680 x 680",
		"Resolution: 850 x 850",
		"Resolution: 1000 x 1000"
	};
	unsigned int resolutionU[4] = { 400, 680, 850, 1000 };
	int resCount;
	string difficulties[DIFFICULTY_COUNT] = {
		"Normal",
		"Hard",
		"Extreme"
	};

	RectangleShape backGround;
};