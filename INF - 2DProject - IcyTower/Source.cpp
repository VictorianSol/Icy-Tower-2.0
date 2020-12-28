/*------------------------------------------------------------

Temat projektu: Icy Tower - Gra 2D (platform�wka)

Zakres projektu:
1. klasa gracza
2. movement gracza
3. kolizja z granicami ekranu
/-/ 4. menu (osobna klasa)
5. textura sprite'u
6. p�ynne animacje textur
7. grawitacja
8. proceduralnie, losowo generowane platformy
9. kolizje z platformami
10. statyczna praca kamery powoli ruszaj�ca w g�r� wie�y
11. dynamiczna praca kamery �ledz�ca gracza
12. mechanika odbijania od �cian ze zwi�kszonym p�dem
13. klasa menu napisana od zera
14. Odczyt/zapis tabeli wynik�w z pliku z podzia�em na trudno��
15. zapis/odczyt stanu gry do/z pliku
16. wyb�r trudno�ci rozgrywki
17. textury platform
18. efekt paralaksy �cian oraz t�a
19. textury �cian oraz t�a
20. animacje wallboostu
21. textury platform zale�nie od wysoko�ci
22. wyb�r wygl�du bohatera
23. czas grania oraz ilo�� �mierci

Co zrealizowalem w biezacym tygodniu?
	- naprawione dynamiczne zmiany wielko�ci okna gry
	- dodano wyb�r wygl�du bohatera w opcjach
	- czas grania oraz ilo�� �mierci

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
#include "Misc.h"

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

	RenderWindow window(loadResolution(), "Icy Tower 2.0");
	Clock clock;
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
		clock.restart();
		while (window.isOpen() && loop) {
			Vector2u tempSize(window.getSize());
			Event event;
			while (window.pollEvent(event)) {

				if (event.type == Event::KeyPressed) {
					if (event.key.code == Keyboard::F7)
						gui->devOptions();
					if (event.key.code == Keyboard::Escape) {
						if (player->alive(*view)) {
							addPlaytime(clock);
							Menu* pauseMenu = new Menu(*view, "Pause");
							loop = pauseMenu->loop(window, *view,
								*player, *platforms, *walls);
							delete pauseMenu;
							clock.restart();
							if (loop == false)
								skipTitle = false;
						}
					}
					if (event.key.code == Keyboard::F1) {
						addPlaytime(clock);
						Menu* helpMenu = new Menu(*view, "Help");
						helpMenu->loop(window, *view,
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
					addPlaytime(clock);
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
						addPlaytime(clock);
						addPlaycount();
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