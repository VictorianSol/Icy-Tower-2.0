#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <SFML\Graphics.hpp>
#include "CameraView.h"

#define PLATFORMS_NUMBER 5
#define PLATFORMS_TYPE 4

using namespace sf;
using namespace std;

class CameraView;

class Platforms{
public:
	Platforms(CameraView& view);
	void draw(RenderWindow& window, CameraView& view);
	int getNoPlatforms() const { return PLATFORMS_NUMBER; }
	Vector2f getPosition(int i) const { return platforms[i].getPosition(); };
	Vector2f getSize(int i) const { return platforms[i].getSize(); }
	int getPlatformLevel(int i) const { return platformLevel[i]; }
	string getDifficulty() const { return diffString; };

	bool saveToFile() const;
	bool loadFromFile();
	static bool canLoadState();

private:
	void loadDifficulty();
	int CheckView(CameraView& view) const;
	void Randomise(CameraView& view, int i);
	RectangleShape platforms[PLATFORMS_NUMBER];
	Texture platformsTexture[PLATFORMS_TYPE];
	Vector2u platformsTextureSize;
	int platformLevel[PLATFORMS_NUMBER];
	string diffString;
	float diffOffset;
	int diffRandNo;

	typedef struct {
		Vector2f size;
		Vector2f pos;
		int level;
		IntRect textureRect;
	} tempPlatformsState;
};