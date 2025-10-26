#include "Headers/chessboard.h"
#include <SFML/Graphics.hpp>





ChessBoard::ChessBoard(int windowWidth, int windowHeight, int size)
    : windowSize(windowWidth, windowHeight), 
      boardSize(size), 
      LIGHT_COLOR(255, 165, 0), 
      DARK_COLOR(0, 0, 0) {
          squareSize = windowWidth / boardSize; 
}

void ChessBoard::draw(sf::RenderWindow& window) {
    for (int row = 0; row < boardSize; ++row) {    // boardSize = 8 (by default)
        for (int col = 0; col < boardSize; ++col) {
            sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
            square.setPosition(col * squareSize, row * squareSize);
            

            square.setFillColor((row + col) % 2 == 0 ? LIGHT_COLOR : DARK_COLOR);
            window.draw(square);
        }
    }
}






