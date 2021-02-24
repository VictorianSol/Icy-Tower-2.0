#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <SFML\Graphics.hpp>
#include "Player.h"
#include "FrameTime.h"

using namespace sf;

class Player;
class FrameTime;

class CameraView{
public:
	CameraView(RenderWindow& window);
	void update(RenderWindow& window, Player& player, FrameTime& deltaTime);
	Vector2f getSize() const { return view.getSize(); }
	Vector2f getCenter() const { return view.getCenter(); }
	float getSpeed() const { return scrollSpeed; }
	void setViewport(FloatRect visibleArea) { view.setViewport(visibleArea); }
	void setSize(float x, float y) { view.setSize(x, y); }
	void setCenter(Vector2f viewpos) { view.setCenter(viewpos); }

	bool saveToFile();
	bool loadFromFile(RenderWindow& window);
	static bool canLoadState();

private:
	void followPlayer(Player& player);
	View view;
	float scrollSpeed;
	float scrollSpeedMax;
	int maxSpeedLevel;
	float maxSpeed;
};