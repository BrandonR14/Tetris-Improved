#include "game.h"

Game::Game(sf::RenderWindow& window) : window(window) {
    // Initialize grid
    grid.resize(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, 0));
    
    // Initialize tetrominoes
    initializeTetrominoes();
    
    // Spawn first piece
    spawnNewPiece();
}

void Game::initializeTetrominoes() {
    // I piece (cyan)
    tetrominoes.push_back({
        {{
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        }},
        sf::Color::Cyan
    });
    
    // O piece (yellow)
    tetrominoes.push_back({
        {{
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        }},
        sf::Color::Yellow
    });
    
    // T piece (purple)
    tetrominoes.push_back({
        {{
            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0}
        }},
        sf::Color(128, 0, 128)
    });
    
    // S piece (green)
    tetrominoes.push_back({
        {{
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0}
        }},
        sf::Color::Green
    });
    
    // Z piece (red)
    tetrominoes.push_back({
        {{
            {0, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        }},
        sf::Color::Red
    });
    
    // J piece (blue)
    tetrominoes.push_back({
        {{
            {0, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0}
        }},
        sf::Color::Blue
    });
    
    // L piece (orange)
    tetrominoes.push_back({
        {{
            {0, 0, 0, 0},
            {0, 0, 1, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0}
        }},
        sf::Color(255, 165, 0)
    });
}

void Game::spawnNewPiece() {
    currentPiece = tetrominoes[rand() % tetrominoes.size()];
    currentX = GRID_WIDTH / 2 - 2;
    currentY = 0;
    
    if (checkCollision()) {
        // Game over
        window.close();
    }
}

bool Game::checkCollision() {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (currentPiece.shape[y][x]) {
                int gridX = currentX + x;
                int gridY = currentY + y;
                
                if (gridX < 0 || gridX >= GRID_WIDTH || 
                    gridY >= GRID_HEIGHT ||
                    (gridY >= 0 && grid[gridY][gridX])) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Game::clearLines() {
    for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
        bool lineFull = true;
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (!grid[y][x]) {
                lineFull = false;
                break;
            }
        }
        
        if (lineFull) {
            for (int moveY = y; moveY > 0; moveY--) {
                grid[moveY] = grid[moveY - 1];
            }
            grid[0] = std::vector<int>(GRID_WIDTH, 0);
            y++; // Check the same line again
        }
    }
}

void Game::update() {
    static float fallTime = 0;
    static const float fallSpeed = 1.0f;
    
    fallTime += 1.0f / 60.0f;
    
    if (fallTime >= fallSpeed) {
        fallTime = 0;
        currentY++;
        
        if (checkCollision()) {
            currentY--;
            // Lock the piece
            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 4; x++) {
                    if (currentPiece.shape[y][x]) {
                        int gridY = currentY + y;
                        if (gridY >= 0) {
                            grid[gridY][currentX + x] = 1;
                        }
                    }
                }
            }
            
            clearLines();
            spawnNewPiece();
        }
    }
}

void Game::draw() {
    window.clear(sf::Color::Black);
    
    // Draw grid
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x]) {
                sf::RectangleShape block(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                block.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                block.setFillColor(sf::Color::White);
                window.draw(block);
            }
        }
    }
    
    // Draw current piece
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (currentPiece.shape[y][x]) {
                sf::RectangleShape block(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                block.setPosition((currentX + x) * CELL_SIZE, (currentY + y) * CELL_SIZE);
                block.setFillColor(currentPiece.color);
                window.draw(block);
            }
        }
    }
    
    window.display();
}

void Game::moveLeft() {
    currentX--;
    if (checkCollision()) {
        currentX++;
    }
}

void Game::moveRight() {
    currentX++;
    if (checkCollision()) {
        currentX--;
    }
}

void Game::moveDown() {
    currentY++;
    if (checkCollision()) {
        currentY--;
    }
}

void Game::rotate() {
    std::array<std::array<int, 4>, 4> rotated;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            rotated[x][3 - y] = currentPiece.shape[y][x];
        }
    }
    
    auto originalShape = currentPiece.shape;
    currentPiece.shape = rotated;
    
    if (checkCollision()) {
        currentPiece.shape = originalShape;
    }
}

void Game::dropDown() {
    while (!checkCollision()) {
        currentY++;
    }
    currentY--;
} 