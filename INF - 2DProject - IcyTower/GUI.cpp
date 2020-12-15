#include "GUI.h"

GUI::GUI() {
	font.loadFromFile("resources\\turtles.ttf");
	currentLevel.setFont(font);
	currentLevel.setFillColor(Color(181, 191, 205));

	devToggle = false;
	for (int i = 0; i < DEV_SIZE; i++) {
		devText[i].setFont(font);
		devText[i].setFillColor(Color(181, 191, 205));
	}
}

void GUI::update(CameraView& view, Player& player) {
	sprintf_s(level, "Level: %d", player.getCurrentLevel());
	currentLevel.setString(level);
	currentLevel.setPosition(Vector2f(WALL_OFFSET, view.getCenter().y - view.getSize().y / 2.f));
	if (devToggle)
		devOptions(view, player);
}

void GUI::draw(RenderWindow& window) {
	window.draw(currentLevel);
	if (devToggle)
		for (int i = 0; i < DEV_SIZE; i++)
			window.draw(devText[i]);
}

void GUI::devOptions(CameraView& view, Player& player) {
	sprintf_s(devString[0], "playerVelocity: %.2f x %.2f", player.getVelocity().x, player.getVelocity().y);
	devText[0].setString(devString[0]);
	devText[0].setPosition(Vector2f(WALL_OFFSET, view.getCenter().y - view.getSize().y / 2.f + 40.f));
	sprintf_s(devString[1], "scrollSpeed: %.2f", view.getSpeed());
	devText[1].setString(devString[1]);
	devText[1].setPosition(Vector2f(WALL_OFFSET, view.getCenter().y - view.getSize().y / 2.f + 80.f));
}