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

void GUI::update(CameraView& view, Player& player, FrameTime& deltaTime) {
	currentLevel.setPosition(Vector2f(WALL_OFFSET, view.getCenter().y - view.getSize().y / 2.f));
	sprintf_s(level, "Level: %d", player.getCurrentLevel());
	currentLevel.setString(level);
	if (devToggle)
		devOptions(view, player, deltaTime);
}

void GUI::draw(RenderWindow& window) const{
	window.draw(currentLevel);
	if (devToggle)
		for (int i = 0; i < DEV_SIZE; i++)
			window.draw(devText[i]);
}

void GUI::devOptions(CameraView& view, Player& player, FrameTime& deltaTime) {
	for (int i = 0; i < DEV_SIZE; i++)
		devText[i].setPosition(Vector2f(WALL_OFFSET, view.getCenter().y - view.getSize().y / 2.f + (i + 1) * 40.f));
	sprintf_s(devString[0], "playerVelocity: %.2f x %.2f", player.getVelocity().x, player.getVelocity().y);
	devText[0].setString(devString[0]);
	sprintf_s(devString[1], "scrollSpeed: %.2f", view.getSpeed());
	devText[1].setString(devString[1]);
	sprintf_s(devString[2], "fps: %.0f", deltaTime.fps());
	devText[2].setString(devString[2]);
}