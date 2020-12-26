#include "Platforms.h"

/*
Platform Texture Math:

x -> 25 * 20						= 500
y -> 62.5 * 16						= 1000
i -> 25

AR -> (25 * 20) / (62.5 * 16)		= 0.5
iAR -> ((37 * 16 * 0.5) / 20) / 25	= 0.592

xI -> 37 * 16 * 0.5					= 296
yI -> 37 * 16						= 592
iI -> (37 * 16 * 0.5) / 20			= 14.8

for (int i = 1, j = 125; i <= 16; i++, j += 25)
	printf("[%d]: %.2f\n", i, (float)j * ((37.f * 16.f * 0.5f) / 20.f) / 25.f - 42.f);

[1]: 32.00
[2]: 46.80
[3]: 61.60
[4]: 76.40
[5]: 91.20
[6]: 106.00
[7]: 120.80
[8]: 135.60
[9]: 150.40
[10]: 165.20
[11]: 180.00
[12]: 194.80
[13]: 209.60
[14]: 224.40
[15]: 239.20
[16]: 254.00

*/

Platforms::Platforms(CameraView& view){
	platformsTexture[0].loadFromFile("resources\\platforms.png");
	platformsTexture[1].loadFromFile("resources\\platforms_ice.png");
	platformsTexture[2].loadFromFile("resources\\platforms_wood.png");
	platformsTextureSize = platformsTexture[0].getSize();
	platformsTextureSize.x /= 20;
	platformsTextureSize.y /= 16;

	srand(time(NULL));
	for (int i = 0; i < PLATFORMS_NUMBER; i++) {
		platforms[i].setTexture(&platformsTexture[0]);
		platformLevel[i] = i + 1;
		platforms[i].setSize(Vector2f((rand() % 10) * 25 + 250, 62.5f));
		platforms[i].setTextureRect(
			IntRect(0.f, (platforms[i].getSize().x / 25 - 5) * platformsTextureSize.y,
				(platforms[i].getSize().x / 25 + 1) * platformsTextureSize.x, platformsTextureSize.y));
		platforms[i].setPosition(
			Vector2f(
				std::max(95.f, std::min(view.getSize().x - platforms[i].getSize().x - 95.f, (rand() % 20) * (view.getSize().x / 20.f))),
				view.getSize().y - view.getSize().y / (float)(PLATFORMS_NUMBER - 1) * (i + 1))
		);
	}
}

void Platforms::draw(RenderWindow& window, CameraView& view) {
	for (int i = 0; i < PLATFORMS_NUMBER; i++)
		if ((platformLevel[i] / 100) % PLATFORMS_TYPE == 1) {
			platforms[i].setTexture(&platformsTexture[1]);
			platforms[i].setFillColor(Color(220, 220, 224));
		}
		else if ((platformLevel[i] / 100) % PLATFORMS_TYPE == 2) {
			platforms[i].setTexture(&platformsTexture[2]);
			platforms[i].setFillColor(Color::White);
		}
		else {
			platforms[i].setTexture(&platformsTexture[0]);
			platforms[i].setFillColor(Color(220, 220, 224));
		}
	Randomise(view, CheckView(view));
	for (int i = 0; i < PLATFORMS_NUMBER; i++)
		window.draw(platforms[i]);
}

int Platforms::CheckView(CameraView& view) {
	for (int i = 0; i < PLATFORMS_NUMBER; i++)
		if (platforms[i].getPosition().y >= view.getCenter().y + view.getSize().y / 2.f)
			return i;
	return -1;
}

void Platforms::Randomise(CameraView& view, int i) {
	if (i == -1)
		return;
	
	int j;
	if (i == 0)
		j = PLATFORMS_NUMBER - 1;
	else
		j = i - 1;

	if (platformLevel[0] == 1)
		loadDifficulty();

	platformLevel[i] = platformLevel[j] + 1;
	platforms[i].setSize(Vector2f((rand() % diffRandNo) * 25 + diffOffset, 62.5f));
	platforms[i].setTextureRect(
		IntRect(0.f, (platforms[i].getSize().x / 25 - 5) * platformsTextureSize.y,
			(platforms[i].getSize().x / 25 + 1) * platformsTextureSize.x, platformsTextureSize.y));
	platforms[i].setPosition(
		Vector2f(
			std::max(95.f, std::min(view.getSize().x - platforms[i].getSize().x - 95.f, (rand() % 20) * (view.getSize().x / 20.f))),
			platforms[j].getPosition().y - view.getSize().y / (float)(PLATFORMS_NUMBER - 1))
		);
}

bool Platforms::saveToFile() {
	FILE* fp;
	bool canRead = true;
	// -->
	tempPlatformsState tempPlatforms[PLATFORMS_NUMBER];
	char diffChar[10];
	strcpy(diffChar, diffString.c_str());
	// <--
	fp = fopen("data\\PlatformsState.dat", "w+b");
	if (fp == NULL) {
		perror("Error with saving platformsstate.dat");
		return false;
	}
	fwrite(&canRead, sizeof(bool), 1, fp);
	// -->
	for (int i = 0; i < PLATFORMS_NUMBER; i++) {
		tempPlatforms[i].size = platforms[i].getSize();
		tempPlatforms[i].pos = platforms[i].getPosition();
		tempPlatforms[i].level = platformLevel[i];
		tempPlatforms[i].textureRect = platforms[i].getTextureRect();
	}
	fwrite(&tempPlatforms, sizeof(tempPlatformsState), PLATFORMS_NUMBER, fp);
	fwrite(&diffOffset, sizeof(float), 1, fp);
	fwrite(&diffRandNo, sizeof(int), 1, fp);
	fwrite(&diffChar, sizeof(char[10]), 1, fp);
	// <--
	fclose(fp);
	return true;
}

bool Platforms::loadFromFile() {
	FILE* fp;
	bool canRead;
	// -->
	tempPlatformsState tempPlatforms[PLATFORMS_NUMBER];
	float tempDiffOffset;
	int tempDiffRandNo;
	char tempDiffChar[10] = { 0 };
	// <--
	fp = fopen("data\\PlatformsState.dat", "r+b");
	if (fp == NULL) {
		return false;
	}
	fread(&canRead, sizeof(bool), 1, fp);
	if (!canRead)
		return false;
	// -->
	fread(&tempPlatforms, sizeof(tempPlatformsState), PLATFORMS_NUMBER, fp);
	fread(&tempDiffOffset, sizeof(float), 1, fp);
	fread(&tempDiffRandNo, sizeof(int), 1, fp);
	fread(&tempDiffChar, sizeof(char[10]), 1, fp);
	// <--
	fclose(fp);
	canRead = false;
	fp = fopen("data\\PlatformsState.dat", "w+b");
	if (fp == NULL) {
		perror("Problem with overriting 'canRead' in platformsstate.dat");
		return false;
	}
	fwrite(&canRead, sizeof(bool), 1, fp);
	fclose(fp);
	// -->
	for (int i = 0; i < PLATFORMS_NUMBER; i++) {
		platforms[i].setSize(tempPlatforms[i].size);
		platforms[i].setPosition(tempPlatforms[i].pos);
		platformLevel[i] = tempPlatforms[i].level;
		platforms[i].setTextureRect(tempPlatforms[i].textureRect);
	}
	diffOffset = tempDiffOffset;
	diffRandNo = tempDiffRandNo;
	diffString = tempDiffChar;
	// <--
	return true;
}

void Platforms::loadDifficulty() {
	FILE* fp;
	diffRandNo = 10;
	char charDiff[10] = { 0 };
	fp = fopen("data\\Difficulty.dat", "r+b");
	if (fp == NULL) {
		diffString = "Normal";
		diffOffset = 250;
		return;
	}
	fread(&charDiff, sizeof(char[10]), 1, fp);
	fclose(fp);
	diffString = charDiff;
	if (diffString == "Normal")
		diffOffset = 250;
	else if (diffString == "Hard")
		diffOffset = 125;
	else if (diffString == "Extreme") {
		diffOffset = 125;
		diffRandNo /= 2;
	}
	else
		diffOffset = 250;
	return;
}