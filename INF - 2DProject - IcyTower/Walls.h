#pragma once
#include <SFML\Graphics.hpp>
#include "CameraView.h"

#define WALLS_NUMBER 2

using namespace sf;

class CameraView;

class Walls{
public:
	Walls(CameraView& view);
	void draw(RenderWindow& window, CameraView& view);
	Vector2f getPosition(int i) const { return walls[i].getPosition(); }
	Vector2f getSize(int i) const { return walls[i].getSize(); }

private:
	RectangleShape walls[WALLS_NUMBER];
	Texture wallsTexture;
	IntRect wallsTextureRect;

	RectangleShape backGround;
	Texture bgTexture;
};