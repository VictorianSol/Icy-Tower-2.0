#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <SFML\Graphics.hpp>
#include "Platforms.h"
#include "CameraView.h"
#include "Walls.h"

using namespace std;
using namespace sf;

class Platforms;
class CameraView;
class Walls;

class Player {
public:
	Player(CameraView& view);
	void draw(RenderWindow& window, string menuType);
	void draw(RenderWindow& window);
	void move(CameraView& view);
	void collidePlatforms(Platforms& platforms);
	void collideWalls(Walls& walls);
	Vector2f getPosition() { return player.getPosition(); }
	Vector2f getSize() { return playerShape; }
	Vector2f getVelocity() { return playerVelocity; }
	int getCurrentLevel() { return currentLevel; }
	bool alive(CameraView& view);
	bool alive(View& view);

	bool saveToFile();
	bool loadFromFile();

	void move(Vector2f offset) { player.move(offset); }

private:
	RectangleShape player;
	Vector2f playerShape;
	Texture playerTexture;
	Vector2u playerTextureSize;
	int animationFrame;
	int animationTexture;
	int animationType, animationTypeOld;

	Vector2f playerVelocity;
	float playerFeetOffset;
	float playerHitboxOffset;
	bool canJump;
	bool canBoost;
	bool wallStopL, wallStopR;
	bool onEdgeL;
	bool onEdgeR;
	int currentLevel;
};