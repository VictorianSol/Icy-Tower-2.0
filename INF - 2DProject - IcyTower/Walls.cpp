#include "Walls.h"

Walls::Walls(CameraView& view) {
	backGround.setSize(Vector2f(view.getSize().x * 9.f / 10.f, view.getSize().y));
	bgTexture.setRepeated(true);
	bgTexture.loadFromFile("resources\\background.png");
	backGround.setTexture(&bgTexture);

	walls[0].setSize(Vector2f(70.f, view.getSize().y));
	walls[1].setSize(Vector2f(70.f, view.getSize().y));
	walls[0].setPosition(0.f, view.getCenter().y - view.getSize().y / 2.f);
	walls[1].setPosition(view.getCenter().x + view.getSize().x / 2.f - walls[1].getSize().x, view.getCenter().y - view.getSize().y / 2.f);

	wallsTexture.loadFromFile("resources\\walls.png");
	wallsTexture.setRepeated(true);
	walls[0].setTexture(&wallsTexture);
	walls[1].setTexture(&wallsTexture);
}

void Walls::draw(RenderWindow& window, CameraView& view) {
	backGround.setPosition(50.f, view.getCenter().y - view.getSize().y / 2.f);
	backGround.setTextureRect(
		IntRect(0.f, view.getCenter().y / 5.f - view.getSize().y * 11.f / 10.f,
			bgTexture.getSize().x, view.getSize().y)
	);
	window.draw(backGround);

	walls[0].setPosition(0.f, view.getCenter().y - view.getSize().y / 2.f);
	walls[1].setPosition(view.getCenter().x + view.getSize().x / 2.f - walls[1].getSize().x, view.getCenter().y - view.getSize().y / 2.f);

	wallsTextureRect = IntRect(0.f, view.getCenter().y / 1.15f + 10.f, wallsTexture.getSize().x, walls[0].getSize().y * 50.f / walls[0].getSize().x);
	walls[1].setTextureRect(wallsTextureRect);
	wallsTextureRect.width = -wallsTextureRect.width;
	walls[0].setTextureRect(wallsTextureRect);

	window.draw(walls[0]);
	window.draw(walls[1]);
}