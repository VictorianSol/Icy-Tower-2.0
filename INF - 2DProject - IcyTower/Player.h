#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <SFML\Graphics.hpp>
#include "Platforms.h"
#include "CameraView.h"
#include "Walls.h"
#include "FrameTime.h"

#define CHARACTER_COUNT 2

using namespace std;
using namespace sf;

class Platforms;
class CameraView;
class Walls;
class FrameTime;

class Player {
public:
	Player(CameraView& view);
	void draw(RenderWindow& window, string menuType, FrameTime& deltaTime);
	void draw(RenderWindow& window, FrameTime& deltaTime);
	void move(CameraView& view, FrameTime& deltaTime);
	void collidePlatforms(Platforms& platforms);
	void collideWalls(Walls& walls);
	Vector2f getPosition() { return player.getPosition(); }
	Vector2f getSize() { return playerShape; }
	Vector2f getVelocity() { return playerVelocity; }
	int getCurrentLevel() { return currentLevel; }
	bool alive(CameraView& view);
	bool alive(View& view);
	string getCurrentCharacter();
	void changeCurrentCharacter();

	bool saveToFile();
	bool loadFromFile();

private:
	RectangleShape player;
	Vector2f playerShape;
	Texture playerTexture;
	Vector2u playerTextureSize;
	float animationFrame;
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

	string characters[CHARACTER_COUNT] = {
		"Icy",
		"Disco Dave"
	};
};