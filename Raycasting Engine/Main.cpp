#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "Map.h"

extern const int SCREEN_WIDTH = 1000;
extern const int SCREEN_HEIGHT = 1000;

int main() {
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Raycasting Engine", sf::Style::Close);
	sf::Event event;
	Map map(&window);

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		map.move();

		window.clear(sf::Color(0, 0, 0, 255));
		map.drawBackground();
		map.drawWalls();
		map.drawMinimap();
		window.display();
	}
	return 0;
}