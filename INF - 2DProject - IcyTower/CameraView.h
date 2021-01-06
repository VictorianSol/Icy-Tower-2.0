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
	void update(RenderWindow& window, Player& player, FrameTime& deltaTIme);
	Vector2f getSize() { return view.getSize(); }
	Vector2f getCenter() { return view.getCenter(); }
	float getSpeed() { return scrollSpeed; }
	void setViewport(FloatRect visibleArea) { view.setViewport(visibleArea); }
	void setSize(float x, float y) { view.setSize(x, y); }
	void setCenter(Vector2f viewpos) { view.setCenter(viewpos); }

	bool saveToFile();
	bool loadFromFile(RenderWindow& window);

private:
	void followPlayer(Player& player);
	View view;
	float scrollSpeed;
	float scrollSpeedMax;
	int maxSpeedLevel;
	float maxSpeed;
};