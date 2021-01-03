#include "Menu.h"

Menu::Menu(CameraView& view, std::string type) {
	this->type = type;
	font.loadFromFile("resources\\Turtles.ttf");
	menuPos = 0;
	resCount = sizeof(resolutionS) / sizeof(resolutionS[0]);

	backGround.setSize(view.getSize());
	backGround.setPosition(0.f, view.getCenter().y - view.getSize().y / 2.f);
	backGround.setFillColor(Color(0, 0, 0, 100));

	if (type == "Title") {
		title = "Icy Tower 2.0";
		menuPosCount = 6;
		menuString[0] = "Continue";
		menuString[1] = "New Game";
		menuString[2] = "Options";
		menuString[3] = "High Scores";
		menuString[4] = "Help";
		menuString[5] = "Quit";
	}
	else if (type == "Pause") {
		title = "Paused";
		menuPosCount = 2;
		menuString[0] = "Continue";
		menuString[1] = "Save & Return";
	}
	else if (type == "Death") {
		title = "You Died";
		menuPosCount = 3;
		menuString[0] = "Respawn";
		menuString[1] = "Show Highscores";
		menuString[2] = "Return";
	}
	else if (type == "Options") {
		title = type;
		menuPosCount = 4;
		menuString[0] = "Resolution: Undefined";
		menuString[1] = "Difficulty: Undefined";
		menuString[2] = "Character: Undefined";
		menuString[3] = "Return";
	}
	else if (type == "High Scores") {
		title = type;
		menuPosCount = 1;
		menuString[0] = "Return";
	}
	else if (type == "Help") {
		title = type;
		menuPosCount = 1;
		menuString[0] = "Continue";
	}

	currentLevel.setFont(font);
	currentLevel.setFillColor(Color(181, 191, 205));
	currentLevel.setCharacterSize(45);
	currentLevel.setPosition(Vector2f(WALL_OFFSET, view.getCenter().y - view.getSize().y / 4.f - 10.f));
	for (int i = 0; i < DIFFICULTY_COUNT; i++) {
		highScores[i].setFont(font);
		highScores[i].setFillColor(Color(181, 191, 205));
		highScores[i].setCharacterSize(30);
		highScores[i].setPosition(Vector2f(WALL_OFFSET + i * 160.f, view.getCenter().y - view.getSize().y / 2.f + 103.f));
	}
	help.setFont(font);
	help.setFillColor(Color(181, 191, 205));
	help.setCharacterSize(30);
	help.setPosition(Vector2f(WALL_OFFSET, view.getCenter().y - view.getSize().y / 2.f + 103.f));
	menuTitle.setFont(font);
	menuTitle.setString(title);
	menuTitle.setFillColor(Color(181, 191, 205));
	menuTitle.setCharacterSize(60);
	menuTitle.setPosition(Vector2f(WALL_OFFSET, view.getCenter().y - view.getSize().y / 2.f));
	playtime.setFont(font);
	playtime.setFillColor(Color(181, 191, 205));
	playtime.setCharacterSize(30);
	playtime.setPosition(Vector2f(WALL_OFFSET, view.getCenter().y + view.getSize().y / 16.f - 7.f));

	for (int i = 0; i < menuPosCount; i++) {
		menu[i].setFont(font);
		menu[i].setFillColor(Color(181, 191, 205));
		menu[i].setString(menuString[i]);
		menu[i].setPosition(Vector2f(WALL_OFFSET, view.getCenter().y - view.getSize().y / 4.f + 50.f * (i + 1)));
	}
	menu[0].setCharacterSize(40);
	menu[0].move(0.f, -6.5f);
}

void Menu::draw(RenderWindow& window) {
	window.draw(backGround);

	if (menu[menuPos].getCharacterSize() < 40) {
		menu[menuPos].setCharacterSize(menu[menuPos].getCharacterSize() + 1);
		menu[menuPos].move(0.f, -0.65f);
	}
	for (int i = 0; i < menuPosCount; i++)
		if (menuPos != i)
			if (menu[i].getCharacterSize() > 30) {
				menu[i].setCharacterSize(menu[i].getCharacterSize() - 1);
				menu[i].move(0.f, 0.65f);
			}
	for (int i = 0; i < menuPosCount; i++)
		window.draw(menu[i]);
	window.draw(currentLevel);
	for (int i = 0; i < DIFFICULTY_COUNT; i++)
		window.draw(highScores[i]);
	window.draw(help);
	window.draw(playtime);
	window.draw(menuTitle);
}

bool Menu::move(RenderWindow& window, CameraView& view) {
	exitTag = "";
	Vector2u tempSize(window.getSize());
	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::KeyPressed) {
			if (event.key.code == Keyboard::W ||
				event.key.code == Keyboard::Up)
				menuPos = menuPos > 0 ? menuPos - 1 : menuPosCount - 1;
			if (event.key.code == Keyboard::S ||
				event.key.code == Keyboard::Down)
				menuPos = menuPos < menuPosCount - 1 ? menuPos + 1 : 0;
			if (event.key.code == Keyboard::Enter ||
				event.key.code == Keyboard::Space)
				exitTag = menu[menuPos].getString();
			if (event.key.code == Keyboard::Escape) {
				if (type == "Pause")
					exitTag = "Continue";
				else if (type == "Options")
					exitTag = "Return";
				else if (type == "Help")
					exitTag = "Continue";
			}
			if (event.key.code == Keyboard::F1)
				if (type == "Help")
					exitTag = "Continue";
		}
		if (event.type == Event::Resized) {
			if (tempSize.x != event.size.width)
				window.setSize(Vector2u(event.size.width < VideoMode::getDesktopMode().height ? event.size.width : VideoMode::getDesktopMode().height,
					event.size.width < VideoMode::getDesktopMode().height ? event.size.width : VideoMode::getDesktopMode().height));
			else
				window.setSize(Vector2u(event.size.height, event.size.height));
			view.setViewport(FloatRect(0.f, 0.f, 1.f, 1.f));
		}
		if (event.type == Event::Closed)
			return false;
	}
	return true;
}

bool Menu::loop(RenderWindow& window, CameraView& view,
	Player& player, Platforms& platforms, Walls& walls) {
	while (window.isOpen()) {

		if (!move(window, view)) {
			if (type == "Pause") {
				player.saveToFile();
				platforms.saveToFile();
				view.saveToFile();
			}
			else if (type == "Death")
				saveHighscore(platforms, player.getCurrentLevel());
			window.close();
		}

		window.clear(Color(46, 54, 63));
		walls.draw(window, view);
		platforms.draw(window, view);
		player.draw(window, type);

		if (type == "Title") {
			if (exitTag == "Continue") {
				if (view.loadFromFile(window) &&
					platforms.loadFromFile() &&
					player.loadFromFile()) {
					Menu* pauseMenu = new Menu(view, "Pause");
					bool exitMenu = pauseMenu->loop(window, view,
						player, platforms, walls);
					delete pauseMenu;
					return exitMenu;
				}
			}
			else if (exitTag == "New Game")
				return true;
			else if (exitTag == "Options") {
				Menu* optionsMenu = new Menu(view, "Options");
				optionsMenu->loop(window, view,
					player, platforms, walls);
				delete optionsMenu;
			}
			else if (exitTag == "High Scores") {
				Menu* hsMenu = new Menu(view, "High Scores");
				hsMenu->loop(window, view,
					player, platforms, walls);
				delete hsMenu;
			}
			else if (exitTag == "Help") {
				Menu* helpMenu = new Menu(view, "Help");
				helpMenu->loop(window, view,
					player, platforms, walls);
				delete helpMenu;
			}
			else if (exitTag == "Quit")
				window.close();
		}
		else if (type == "Pause") {
			char temp[30];
			sprintf_s(temp, "Level: %d", player.getCurrentLevel());
			currentLevel.setString(temp);

			if (exitTag == "Continue")
				return true;
			else if (exitTag == "Save & Return") {
				if (player.saveToFile() &&
				platforms.saveToFile() &&
				view.saveToFile())
					return false;
			}
		}
		else if (type == "Death") {
			char temp[50];
			int deathLevel = player.getCurrentLevel();
			sprintf_s(temp, "Level: %d", deathLevel);
			currentLevel.setString(temp);

			if (exitTag != "")
				saveHighscore(platforms, deathLevel);
			if (exitTag == "Respawn")
				return true;
			else if (exitTag == "Show Highscores") {
				Menu* hsMenu = new Menu(view, "High Scores");
				hsMenu->loop(window, view,
					player, platforms, walls);
				delete hsMenu;
				return false;
			}
			else if (exitTag == "Return")
				return false;
		}
		else if (type == "Options") {
			char temp[30];
			menu[0].setString("Difficulty: " + loadDifficulty());
			sprintf_s(temp, "Resolution: %d x %d", window.getSize().x, window.getSize().y);
			menu[1].setString(temp);
			menu[2].setString("Character: " + player.getCurrentCharacter());

			if (!exitTag.compare(0, 12, "Difficulty: ")) {
				int j = 0;
				for (int i = 0; i < DIFFICULTY_COUNT; i++)
					if (exitTag == "Difficulty: " + difficulties[i]) {
						j = i;
						break;
					}
					else
						j = -1;
				FILE* fp;
				fp = fopen("data\\Difficulty.dat", "w+b");
				if (fp == NULL)
					perror("Couldn't save difficulty to difficulty.dat");
				else {
					char charDiff[10];
					strcpy(charDiff, difficulties[j < DIFFICULTY_COUNT - 1 ? j + 1 : 0].c_str());
					fwrite(&charDiff, sizeof(char[10]), 1, fp);
					fclose(fp);
				}
			}
			else if (!exitTag.compare(0, 12, "Resolution: ")) {
				int j = 0;
				for (int i = 0; i < resCount; i++)
					if (exitTag == resolutionS[i]) {
						j = i;
						break;
					}
					else
						j = -1;
				window.setSize(Vector2u(
					resolutionU[j < resCount - 1 ? j + 1 : 0],
					resolutionU[j < resCount - 1 ? j + 1 : 0])
				);
				window.setPosition(Vector2i(VideoMode::getDesktopMode().width / 2.f - window.getSize().x / 2.f,
					VideoMode::getDesktopMode().height / 2.f - window.getSize().y / 2.f));
				FILE* fp;
				fp = fopen("data\\Resolution.dat", "w+b");
				if (fp == NULL)
					perror("Couldn't save resolution to resolution.dat");
				else {
					VideoMode tempRes = VideoMode(window.getSize().x, window.getSize().y);
					fwrite(&tempRes, sizeof(VideoMode), 1, fp);
					fclose(fp);
				}
			}
			else if (!exitTag.compare(0, 11, "Character: "))
				player.changeCurrentCharacter();
			else if (exitTag == "Return")
				return false;
		}
		else if (type == "High Scores") {
			char temp[100];
			sprintf_s(temp, "Times played: %d\nTime spent: %s", getPlaycount(), getPlaytime().c_str());
			playtime.setString(temp);

			for (int j = 0; j < DIFFICULTY_COUNT; j++) {
				FILE* fp;
				int hsNo, hs[10];
				char highscores[30];
				string tempString = difficulties[j] + ":\n";
				char temp[50];
				char hsDiff[30];
				strcpy(hsDiff, difficulties[j].c_str());
				sprintf_s(temp, "%s%s%s%s", "data\\", "Highscores_", hsDiff, ".dat");
				fp = fopen(temp, "r+b");
				if (!fp == NULL) {
					fread(&hsNo, sizeof(int), 1, fp);
					fread(&hs, sizeof(int), 10, fp);
					fclose(fp);
					for (int i = 0; i < hsNo; i++) {
						sprintf(highscores, "#%d: %d\n", i + 1, hs[i]);
						tempString += highscores;
					}
					highScores[j].setString(tempString);
				}
			}
			menu[0].setPosition(Vector2f(WALL_OFFSET, view.getCenter().y - view.getSize().y / 4.f + 223.f));

			if (exitTag == "Return")
				return false;
		}
		else if (type == "Help") {
			help.setString(
				"Jump highest you can upward the tower\n\n"
				"   [W]     [Spacebar]    [^]      \n"
				"[A][ ][D]              [<][ ][>]   <- move using these\n\n"
				"PS: You can bouce off the walls");
			menu[0].setPosition(Vector2f(WALL_OFFSET, view.getCenter().y - view.getSize().y / 4.f + 80.f));

			if (exitTag == "Continue")
				return true;
		}

		draw(window);
		window.display();
	}
}

string Menu::loadDifficulty() {
	FILE* fp;
	char charDiff[10] = { 0 };
	string difficulty = "Normal";
	fp = fopen("data\\Difficulty.dat", "r+b");
	if (fp == NULL)
		return difficulty;
	fread(&charDiff, sizeof(char[10]), 1, fp);
	fclose(fp);
	return charDiff;
}

int Menu::getPlaycount() {
	FILE* fp;
	int playcount = 0;
	fp = fopen("data\\Playtime.dat", "r+b");
	if (fp == NULL)
		return playcount;
	fread(&playcount, sizeof(int), 1, fp);
	fclose(fp);
	return playcount;
}

string Menu::getPlaytime() {
	FILE* fp;
	char temp[100];
	double playtime = 0;
	fp = fopen("data\\Playtime.dat", "r+b");
	if (fp == NULL)
		return "0s";
	fseek(fp, sizeof(int), SEEK_SET);
	fread(&playtime, sizeof(double), 1, fp);
	fclose(fp);

	if (((int)playtime / 3600) != 0) {
		sprintf_s(
			temp,
			"%dh %dm %ds\n",
			(int)playtime / 3600,
			((int)playtime - ((int)playtime / 3600) * 3600) / 60,
			(int)playtime - ((int)playtime / 60) * 60
		);
	}
	else if (((int)playtime / 60) != 0) {
		sprintf_s(
			temp,
			"%dm %ds\n",
			(int)playtime / 60,
			(int)playtime - ((int)playtime / 60) * 60
		);
	}
	else {
		sprintf_s(
			temp,
			"%ds\n",
			(int)playtime
		);
	}
	return temp;
}

bool Menu::saveHighscore(Platforms& platforms, int deathLevel) {
	FILE* fp;
	int hsNo = 0, highscores[10] = { 0 };
	char temp[50];
	char hsDiff[10];
	strcpy(hsDiff, platforms.getDifficulty().c_str());
	sprintf_s(temp, "%s%s%s%s", "data\\", "Highscores_", hsDiff, ".dat");
	fp = fopen(temp, "r+b");
	if (!fp == NULL) {
		fread(&hsNo, sizeof(int), 1, fp);
		fread(&highscores, sizeof(int), 10, fp);
		fclose(fp);
	}
	fp = fopen(temp, "w+b");
	if (fp == NULL) {
		perror("Error with saving highscore");
		return false;
	}
	if (hsNo < 10)
		hsNo++;
	fwrite(&hsNo, sizeof(int), 1, fp);
	if (highscores[9] < deathLevel)
		highscores[9] = deathLevel;
	sort(highscores, highscores + sizeof(highscores) / sizeof(int), greater<int>());
	fwrite(&highscores, sizeof(int), 10, fp);
	fclose(fp);
	return true;
}