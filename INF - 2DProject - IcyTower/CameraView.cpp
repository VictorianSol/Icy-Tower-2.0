#include "CameraView.h"

CameraView::CameraView(RenderWindow& window) {
	view.reset(FloatRect(0.f, 0.f, 1000.f, 1000.f));
	view.setViewport(FloatRect(0.f, 0.f, 1.f, 1.f));
	window.setView(view);
	scrollSpeed = 0;
	scrollSpeedMax = 0;
	maxSpeedLevel = 100;
	maxSpeed = 5.f;
}

void CameraView::update(RenderWindow& window, Player& player) {
	scrollSpeedMax = std::max(-player.getCurrentLevel() / (maxSpeedLevel / maxSpeed), -maxSpeed);
	if (!player.alive(view))
		scrollSpeed <= -0.025f ? scrollSpeed *= 0.97f : scrollSpeed = 0.f;
	else {
		if (player.getPosition().y - player.getSize().y <= view.getCenter().y - view.getSize().y / 4.f)
			scrollSpeed = std::min(scrollSpeed * 0.995f, scrollSpeedMax);
		else if (scrollSpeed < -4.5f)
			scrollSpeed = std::min(scrollSpeed * 0.96f, scrollSpeedMax);
		else if (scrollSpeed < scrollSpeedMax)
			scrollSpeed = std::min(scrollSpeed * 0.98f, scrollSpeedMax);
		else
			scrollSpeed *= 1.01f;
		followPlayer(player);
	}
	view.move(0.f, scrollSpeed);

	window.setView(view);
}

void CameraView::followPlayer(Player& player) {
	float temp;
	if (player.getPosition().y + player.getSize().y / 2.f <= view.getCenter().y &&
		player.getVelocity().y < 0.f) {
		temp = -pow(abs((player.getPosition().y - player.getSize().y / 2.f - view.getCenter().y) / 250.f), 3);
		if (scrollSpeed > temp)
			scrollSpeed = temp;
	}
}

bool CameraView::saveToFile() {
	FILE* fp;
	bool canRead = true;
	// -->
	Vector2f viewpos = view.getCenter();
	// <--
	fp = fopen("data\\CameraViewState.dat", "w+b");
	if (fp == NULL) {
		perror("Error with saving cameraviewstate.dat");
		return false;
	}
	fwrite(&canRead, sizeof(bool), 1, fp);
	// -->
	fwrite(&viewpos, sizeof(Vector2f), 1, fp);
	fwrite(&scrollSpeed, sizeof(float), 1, fp);
	// <--
	fclose(fp);
	return true;
}

bool CameraView::loadFromFile(RenderWindow& window) {
	FILE* fp;
	bool canRead;
	// -->
	float tempScrollSpeed;
	Vector2f tempViewpos;
	// <--
	fp = fopen("data\\CameraViewState.dat", "r+b");
	if (fp == NULL) {
		return false;
	}
	fread(&canRead, sizeof(bool), 1, fp);
	if (!canRead)
		return false;
	// -->
	fread(&tempViewpos, sizeof(Vector2f), 1, fp);
	fread(&tempScrollSpeed, sizeof(float), 1, fp);
	// <--
	fclose(fp);
	canRead = false;
	fp = fopen("data\\CameraViewState.dat", "w+b");
	if (fp == NULL) {
		perror("Problem with overriting 'canRead' in cameraviewstate.dat");
		return false;
	}
	fwrite(&canRead, sizeof(bool), 1, fp);
	fclose(fp);
	// -->
	view.setCenter(tempViewpos);
	scrollSpeed = tempScrollSpeed;
	window.setView(view);
	// <--
	return true;
}