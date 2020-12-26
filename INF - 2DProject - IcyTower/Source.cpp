/*------------------------------------------------------------

Temat projektu: Icy Tower - Gra 2D (platformówka)

Zakres projektu:
1. klasa gracza
2. movement gracza
3. kolizja z granicami ekranu
/-/ 4. menu (osobna klasa)
5. textura sprite'u
6. p³ynne animacje textur
7. grawitacja
8. proceduralnie, losowo generowane platformy
9. kolizje z platformami
10. statyczna praca kamery powoli ruszaj¹ca w górê wie¿y
11. dynamiczna praca kamery œledz¹ca gracza
12. mechanika odbijania od œcian ze zwiêkszonym pêdem
13. klasa menu napisana od zera
14. Odczyt/zapis tabeli wyników z pliku z podzia³em na trudnoœæ
15. zapis/odczyt stanu gry do/z pliku
16. wybór trudnoœci rozgrywki
17. textury platform
18. efekt paralaksy œcian oraz t³a
19. textury œcian oraz t³a
20. animacje wallboostu
21. textury platform zale¿nie od wysokoœci
22. wybór wygl¹du bohatera

Co zrealizowalem w biezacym tygodniu?
	- naprawione dynamiczne zmiany wielkoœci okna gry
	- dodano wybór wygl¹du bohatera w opcjach

Co planuje na kolejny tydzien?
	

------------------------------------------------------------*/


	
/*------------------------------------------------------------

Program glowny	

------------------------------------------------------------*/

#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "Platforms.h"
#include "CameraView.h"
#include "GUI.h"
#include "Walls.h"
#include "Menu.h"

#ifdef NDEBUG
#include <windows.h>
#endif

using namespace sf;
using namespace std;

VideoMode loadResolution() {
	FILE* fp;
	VideoMode windowres = VideoMode(400, 400);
	fp = fopen("data\\resolution.dat", "r+b");
	if (fp == NULL)
		return windowres;
	fread(&windowres, sizeof(VideoMode), 1, fp);
	fclose(fp);
	return windowres;
}

int main() {

#ifdef NDEBUG
	system("mkdir data");
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	RenderWindow window(loadResolution(), "Icy Tower 2.0");
	//window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(111);
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
			loop = titleMenu->loop(window, *view,
				*player, *platforms, *walls);
			delete titleMenu;
		}
		else
			loop = true;

		while (window.isOpen() && loop) {
			Vector2u tempSize(window.getSize());
			Event event;
			while (window.pollEvent(event)) {

				if (event.type == Event::KeyPressed) {
					if (event.key.code == Keyboard::F7)
						gui->devOptions();
					if (event.key.code == Keyboard::Escape) {
						if (player->alive(*view)) {
							Menu* pauseMenu = new Menu(*view, "Pause");
							loop = pauseMenu->loop(window, *view,
								*player, *platforms, *walls);
							delete pauseMenu;
							if (loop == false)
								skipTitle = false;
						}
					}
					if (event.key.code == Keyboard::F1) {
						Menu* helpMenu = new Menu(*view, "Help");
						helpMenu->loop(window, *view,
							*player, *platforms, *walls);
						delete helpMenu;
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
					player->saveToFile();
					platforms->saveToFile();
					view->saveToFile();
					window.close();
				}
			}
			if (loop) {
				player->collidePlatforms(*platforms);
				player->collideWalls(*walls);

				if (player->alive(*view))
					player->move(*view);

				view->update(&window, *player);
				gui->update(*view, *player);

				window.clear(Color(46, 54, 63));
				walls->draw(window, *view);
				platforms->draw(window, *view);
				gui->draw(window);
				player->draw(window);
				window.display();

				if (!player->alive(*view))
					if (view->getSpeed() == 0.f) {
						loop = false;
						Menu* deathMenu = new Menu(*view, "Death");
						skipTitle = deathMenu->loop(window, *view,
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