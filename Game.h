#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>

class Game {
private:
    static const int GRID_WIDTH = 10;
    static const int GRID_HEIGHT = 20;
    static const int CELL_SIZE = 30;
    
    sf::RenderWindow& window;
    std::vector<std::vector<int>> grid;
    std::vector<sf::RectangleShape> blocks;
    
    // Tetromino shapes
    struct Tetromino {
        std::array<std::array<int, 4>, 4> shape;
        sf::Color color;
    };
    
    std::vector<Tetromino> tetrominoes;
    Tetromino currentPiece;
    int currentX, currentY;
    
    void initializeTetrominoes();
    void spawnNewPiece();
    bool checkCollision();
    void clearLines();
    void drawGrid();
    void drawCurrentPiece();

public:
    Game(sf::RenderWindow& window);
    void update();
    void draw();
    void moveLeft();
    void moveRight();
    void moveDown();
    void rotate();
    void dropDown();
}; 