#include "Player.h"

using namespace sf;

Player::Player(CameraView& view) {	// 58:44
	if (getCurrentCharacter() == characters[1])
		playerTexture.loadFromFile("resources\\discodave.png");
	else
		playerTexture.loadFromFile("resources\\player.png");
	playerTextureSize = playerTexture.getSize();
	playerTextureSize.x /= 15;
	playerTextureSize.y /= 1;

	playerShape = Vector2f(2.5f * playerTextureSize.x, 2.5f * playerTextureSize.y);
	player.setSize(playerShape);
	player.setTexture(&playerTexture);
	player.setTextureRect(IntRect(0, 0, playerTextureSize.x, playerTextureSize.y));

	player.setPosition(view.getSize().x / 2, view.getSize().y - playerShape.y / 2.f);
	playerFeetOffset = playerShape.x - playerShape.x * 33.f / 44.f;
	playerHitboxOffset = playerShape.x - playerShape.x * 38.f / 44.f;

	player.setOrigin(Vector2f(player.getSize().x / 2.f, player.getSize().y / 2.f));

	animationFrame = 0;
	animationTexture = 0;
	animationType = 0;
	animationTypeOld = 0;
	currentLevel = 0;
	canJump = true;
	canBoost = true;
	wallStopL = false;
	wallStopR = false;
	onEdgeL = false;
	onEdgeR = false;
}

void Player::draw(RenderWindow& window, string menuType) {
	if (menuType == "Pause" || menuType == "Help")
		window.draw(player);
	else
		draw(window);
}

void Player::draw(RenderWindow& window) {
	animationFrame++;
	animationType = 0;
	if (playerVelocity.x > 0.f)
		animationType += 1;
	else if (playerVelocity.x == 0.f)
		animationType += 2;
	else
		animationType += 4;
	if (playerVelocity.y < 0.f)
		animationType += 8;
	else if (playerVelocity.y == 0.f)
		animationType += 16;
	else
		animationType += 32;
	if (!canBoost)
		animationType += 64;
	if (onEdgeL || onEdgeR)
		animationType += 128;
	if (animationTypeOld != animationType)
		animationFrame = 60;

	if (canBoost)
		player.setRotation(0.f);
	if (playerVelocity == Vector2f(0.f, 0.f) && canJump) {
		// Idle
		if (animationFrame >= 60) {
			animationFrame = 0;

			if ((onEdgeL || onEdgeR) && !(wallStopL || wallStopR))
				if (animationTexture != 12)
					animationTexture = 12;
				else
					animationTexture++;
			else if (animationTexture == 11)
				animationTexture = 14;
			else if (animationTexture >= 3)
				animationTexture = 0;
			else
				animationTexture++;

			if (onEdgeL &&!(wallStopL || wallStopR))
				player.setTextureRect(
					IntRect((animationTexture + 1) * playerTextureSize.x, 0,
						-(int)playerTextureSize.x, playerTextureSize.y));
			else
				player.setTextureRect(
					IntRect(animationTexture * playerTextureSize.x, 0,
						playerTextureSize.x, playerTextureSize.y));
		}
	}
	else {
		if (!canBoost) {
			animationTexture = 11;
			player.setTextureRect(
				IntRect(animationTexture * playerTextureSize.x, 0,
					playerTextureSize.x, playerTextureSize.y)
			);
			player.rotate(5.f + abs(playerVelocity.y) / 3.f);
		}
		else if (animationFrame >= 15) {
			animationFrame = 0;

			if (playerVelocity.x != 0.f && playerVelocity.y == 0.f && canJump) {
				// Moving
				if (animationTexture >= 7 || animationTexture <= 3)
					animationTexture = 4;
				else
					animationTexture++;

				if (playerVelocity.x > 0.f)
					player.setTextureRect(
						IntRect(animationTexture * playerTextureSize.x, 0,
							playerTextureSize.x, playerTextureSize.y));
				else
					player.setTextureRect(
						IntRect((animationTexture + 1) * playerTextureSize.x, 0,
							-(int)playerTextureSize.x, playerTextureSize.y));
			}
			else if (playerVelocity.y != 0.f) {
				// Airborn
				if (playerVelocity.x == 0.f) {
					// Vertical Jump
					animationTexture = 9;
					player.setTextureRect(
						IntRect(animationTexture * playerTextureSize.x, 0,
							playerTextureSize.x, playerTextureSize.y));
				}
				else {
					// Moving Airborn
					if (playerVelocity.y < 0.f)
						animationTexture = 8;
					else
						animationTexture = 10;

					if (playerVelocity.x > 0.f)
						player.setTextureRect(
							IntRect(animationTexture * playerTextureSize.x, 0,
								playerTextureSize.x, playerTextureSize.y));
					else
						player.setTextureRect(
							IntRect((animationTexture + 1) * playerTextureSize.x, 0,
								-(int)playerTextureSize.x, playerTextureSize.y));
				}
			}
		}
	}
	animationTypeOld = animationType;

	window.draw(player);
}

void Player::move(CameraView& view) {
	int moved = 0;

	if (player.getPosition().y + playerShape.y / 2.f >= view.getSize().y) {
		player.setPosition(Vector2f(player.getPosition().x, view.getSize().y - playerShape.y / 2.f));
		canJump = true;
	}

	if (canJump == false)
		playerVelocity.y += 0.5f;
	else {
		playerVelocity.y = 0.f;
		canBoost = true;
	}

	if ((Keyboard::isKeyPressed(Keyboard::W) ||
		Keyboard::isKeyPressed(Keyboard::Space) ||
		Keyboard::isKeyPressed(Keyboard::Up)) &&
		canJump) {
		canJump = false;
		playerVelocity.y = -17.5f;
	}

	if (!wallStopL && (Keyboard::isKeyPressed(Keyboard::A) ||
		Keyboard::isKeyPressed(Keyboard::Left))) {
		if (playerVelocity.x >= -13.3f)
			playerVelocity.x -= 0.66f;
		moved += 2;
	}
	if (!wallStopR && (Keyboard::isKeyPressed(Keyboard::D) ||
		Keyboard::isKeyPressed(Keyboard::Right))) {
		if (playerVelocity.x <= 13.3f)
			playerVelocity.x += 0.66f;
		moved += 4;
	}

	if (!moved || moved == 6)
		if (playerVelocity.x >= 0.66f || playerVelocity.x <= -0.66f)
			playerVelocity.x *= 0.93f;
		else
			playerVelocity.x = 0;

	player.move(playerVelocity.x, playerVelocity.y);
}

void Player::collidePlatforms(Platforms& platforms)
{
	for (int i = 0; i < platforms.getNoPlatforms(); i++) {
		Vector2f platformpos = platforms.getPosition(i);
		Vector2f platformlen = platforms.getSize(i);
		Vector2f playerpos = player.getPosition();
		platformlen.y /= 2.5f; // This causes velocity bug at platform 12

		if (playerpos.y + playerShape.y / 2.f >= platformpos.y &&
			playerpos.y + playerShape.y / 2.f <= platformpos.y + platformlen.y &&
			playerpos.x + playerFeetOffset >= platformpos.x &&
			playerpos.x - playerFeetOffset <= platformpos.x + platformlen.x &&
			playerVelocity.y >= 0.f) {
			player.setPosition(Vector2f(playerpos.x, platformpos.y - playerShape.y / 2.f));
			playerpos = player.getPosition();
			canJump = true;
			if (currentLevel < platforms.getPlatformLevel(i))
				currentLevel = platforms.getPlatformLevel(i);
			if (playerpos.x - playerFeetOffset <= platformpos.x
				&& playerVelocity.x == 0.f && !(wallStopL || wallStopR)) {
				player.setPosition(platformpos.x + playerFeetOffset, playerpos.y);
				onEdgeL = true;
				onEdgeR = false;
			}
			else if (playerpos.x + playerFeetOffset >= platformpos.x + platformlen.x
				&& playerVelocity.x == 0.f && !(wallStopL || wallStopR)) {
				player.setPosition(platformpos.x + platformlen.x - playerFeetOffset, playerpos.y);
				onEdgeR = true;
				onEdgeL = false;
			}
			else {
				onEdgeL = false;
				onEdgeR = false;
			}
			return;
		}
	}
	onEdgeL = false;
	onEdgeR = false;
	canJump = false;
}

void Player::collideWalls(Walls& walls) {
	Vector2f playerpos = player.getPosition();
	wallStopL = false;
	wallStopR = false;

	if (playerpos.x - playerShape.x / 2.f + playerHitboxOffset <= walls.getPosition(0).x + walls.getSize(0).x) {
		player.setPosition(walls.getPosition(0).x + walls.getSize(0).x + playerShape.x / 2.f - playerHitboxOffset, playerpos.y);
		if (playerVelocity.y < 0.f && playerVelocity.x < -13.f && canBoost) {
			playerVelocity.x = -playerVelocity.x;
			player.move(25.f, 0.f);
			playerVelocity.y *= 1.5f;
			canBoost = false;
		}
		else if (playerVelocity.x < -13.3f)
			playerVelocity.x = -playerVelocity.x;
		else {
			playerVelocity.x = 0.f;
			wallStopL = true;
		}
	}
	else if (playerpos.x + playerShape.x / 2.f - playerHitboxOffset >= walls.getPosition(1).x) {
		player.setPosition(walls.getPosition(1).x - playerShape.x / 2.f + playerHitboxOffset, playerpos.y);
		if (playerVelocity.y < 0.f && playerVelocity.x > 13.f && canBoost) {
			playerVelocity.x = -playerVelocity.x;
			player.move(-25.f, 0.f);
			playerVelocity.y *= 1.5f;
			canBoost = false;
		}
		else if (playerVelocity.x > 13.3f)
			playerVelocity.x = -playerVelocity.x;
		else {
			playerVelocity.x = 0.f;
			wallStopR = true;
		}
	}
}

bool Player::alive(CameraView& view) {
	if (player.getPosition().y - playerShape.y / 2.f >= view.getCenter().y + view.getSize().y / 2.f)
		return false;
	return true;
}

bool Player::alive(View& view) {
	if (player.getPosition().y - playerShape.y / 2.f >= view.getCenter().y + view.getSize().y / 2.f)
		return false;
	return true;
}

string Player::getCurrentCharacter() {
	FILE* fp;
	string character = characters[0];
	char characterC[20];
	fp = fopen("data\\Character.dat", "r+b");
	if (fp == NULL)
		return character;
	fread(&characterC, sizeof(char[20]), 1, fp);
	fclose(fp);
	return characterC;
}

void Player::changeCurrentCharacter() {
	string tempCharacter = getCurrentCharacter();
	int j = 0;
	for (int i = 0; i < CHARACTER_COUNT; i++)
		if (tempCharacter == characters[i]) {
			j = i;
			break;
		}
		else
			j = -1;
	tempCharacter = characters[j < CHARACTER_COUNT - 1 ? j + 1 : 0];
	if (tempCharacter == characters[1])
		playerTexture.loadFromFile("resources\\discodave.png");
	else
		playerTexture.loadFromFile("resources\\player.png");
	FILE* fp;
	fp = fopen("data\\Character.dat", "w+b");
	if (fp == NULL) {
		perror("Couldn't save character data to Character.dat");
		return;
	}
	char tempCharacterC[20];
	strcpy(tempCharacterC, tempCharacter.c_str());
	fwrite(&tempCharacterC, sizeof(char[20]), 1, fp);
	fclose(fp);
}

bool Player::saveToFile() {
	FILE* fp;
	bool canRead = true;
	// -->
	Vector2f playerpos = player.getPosition();
	IntRect playerTextureRect = player.getTextureRect();
	// <--
	fp = fopen("data\\PlayerState.dat", "w+b");
	if (fp == NULL) {
		perror("Error with saving playerstate.dat");
		return false;
	}
	fwrite(&canRead, sizeof(bool), 1, fp);
	// -->
	fwrite(&currentLevel, sizeof(int), 1, fp);
	fwrite(&playerpos, sizeof(Vector2f), 1, fp);
	fwrite(&playerVelocity, sizeof(Vector2f), 1, fp);
	fwrite(&playerTextureRect, sizeof(IntRect), 1, fp);
	// <--
	fclose(fp);
	return true;
}

bool Player::loadFromFile() {
	FILE* fp;
	bool canRead;
	// -->
	int tempCurrentLevel;
	Vector2f tempPlayerpos, tempPlayerVelocity;
	IntRect tempPlayerTextureRect;
	// <--
	fp = fopen("data\\PlayerState.dat", "r+b");
	if (fp == NULL) {
		return false;
	}
	fread(&canRead, sizeof(bool), 1, fp);
	if (!canRead)
		return false;
	// -->
	fread(&tempCurrentLevel, sizeof(int), 1, fp);
	fread(&tempPlayerpos, sizeof(Vector2f), 1, fp);
	fread(&tempPlayerVelocity, sizeof(Vector2f), 1, fp);
	fread(&tempPlayerTextureRect, sizeof(IntRect), 1, fp);
	// <--
	fclose(fp);
	canRead = false;
	fp = fopen("data\\PlayerState.dat", "w+b");
	if (fp == NULL) {
		perror("Problem with overriting 'canRead' in playerstate.dat");
		return false;
	}
	fwrite(&canRead, sizeof(bool), 1, fp);
	fclose(fp);
	// -->
	player.setPosition(tempPlayerpos);
	playerVelocity = tempPlayerVelocity;
	player.setTextureRect(tempPlayerTextureRect);
	currentLevel = tempCurrentLevel;
	// <--
	return true;
}