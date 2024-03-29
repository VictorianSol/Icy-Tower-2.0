#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "Platforms.h"
#include "CameraView.h"
#include "GUI.h"
#include "Walls.h"
#include "Menu.h"
#include "FrameTime.h"

#ifdef NDEBUG
#include <windows.h>
#endif

using namespace sf;
using namespace std;

int main() {

#ifdef NDEBUG
	system("mkdir data");
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	RenderWindow window(Menu::loadResolution(), "Icy Tower 2.0");
	Clock clock;
	FrameTime deltaTime;
	window.setFramerateLimit(Menu::fps() ? 240 : 120);
	bool loop;
	bool skipTitle = false;
	Image icon;
	icon.loadFromFile("resources\\icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	while (window.isOpen()) {
		CameraView* view = new CameraView(window);
		GUI* gui = new GUI;
		Player* player = new Player(*view);
		Platforms* platforms = new Platforms(*view);
		Walls* walls = new Walls(*view);

		if (!skipTitle) {
			Menu* titleMenu = new Menu(*view, "Title");
			loop = titleMenu->loop(window, *view, deltaTime,
				*player, *platforms, *walls);
			delete titleMenu;
		}
		else
			loop = true;
		clock.restart();
		deltaTime.restart();
		while (window.isOpen() && loop) {
			deltaTime++;
			Vector2u tempSize(window.getSize());
			Event event;
			while (window.pollEvent(event)) {

				if (event.type == Event::KeyPressed) {
					if (event.key.code == Keyboard::F7)
						gui->devOptions();
					if (event.key.code == Keyboard::Escape) {
						if (player->alive(*view)) {
							Menu::addPlaytime(clock);
							Menu* pauseMenu = new Menu(*view, "Pause");
							loop = pauseMenu->loop(window, *view, deltaTime,
								*player, *platforms, *walls);
							delete pauseMenu;
							clock.restart();
							if (loop == false)
								skipTitle = false;
						}
					}
					if (event.key.code == Keyboard::F1) {
						Menu::addPlaytime(clock);
						Menu* helpMenu = new Menu(*view, "Help");
						helpMenu->loop(window, *view, deltaTime,
							*player, *platforms, *walls);
						delete helpMenu;
						clock.restart();
					}
				}
				if (event.type == Event::Resized) {
					if (tempSize.x != event.size.width)
						window.setSize(Vector2u(event.size.width < VideoMode::getDesktopMode().height ? event.size.width : VideoMode::getDesktopMode().height,
							event.size.width < VideoMode::getDesktopMode().height ? event.size.width : VideoMode::getDesktopMode().height));
					else
						window.setSize(Vector2u(event.size.height, event.size.height));
					view->setViewport(FloatRect(0.f, 0.f, 1.f, 1.f));
				}
				if (event.type == Event::Closed) {
					Menu::addPlaytime(clock);
					if (player->alive(*view)) {
						player->saveToFile();
						platforms->saveToFile();
						view->saveToFile();
					}
					else
						Menu::saveHighscore(*platforms, player->getCurrentLevel());
					window.close();
				}
			}
			if (loop) {
				player->collidePlatforms(*platforms);
				player->collideWalls(*walls);

				if (player->alive(*view))
					player->move(*view, deltaTime);

				view->update(window, *player, deltaTime);
				gui->update(*view, *player, deltaTime);

				window.clear(Color(46, 54, 63));
				walls->draw(window, *view);
				platforms->draw(window, *view);
				gui->draw(window);
				player->draw(window, deltaTime);
				window.display();

				if (!player->alive(*view))
					if (view->getSpeed() == 0.f) {
						loop = false;
						Menu::addPlaytime(clock);
						Menu::addPlaycount();
						Menu* deathMenu = new Menu(*view, "Death");
						skipTitle = deathMenu->loop(window, *view, deltaTime,
							*player, *platforms, *walls);
						delete deathMenu;
					}
			}
		}
		delete view;
		delete player;
		delete gui;
		delete platforms;
		delete walls;
	}

	return 0;
}