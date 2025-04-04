#include <SFML/Graphics.hpp>
#include "game.h"

int main() {
    // Window size matches game boundaries (10x20 cells, 30 pixels each)
    sf::RenderWindow window(sf::VideoMode(300, 600), "Tetris");
    window.setFramerateLimit(60);

    Game game(window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Left:
                        game.moveLeft();
                        break;
                    case sf::Keyboard::Right:
                        game.moveRight();
                        break;
                    case sf::Keyboard::Down:
                        game.moveDown();
                        break;
                    case sf::Keyboard::Up:
                        game.rotate();
                        break;
                    case sf::Keyboard::Space:
                        game.dropDown();
                        break;
                }
            }
        }

        game.update();
        game.draw();
    }

    return 0;
} 