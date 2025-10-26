#ifndef CHESSEBOARD_H
#include <SFML/Graphics.hpp>


class ChessBoard {

private:
    int boardSize;      // Nombre de cases (8)
    int squareSize;     // Taille d'une case en pixels
    sf::Vector2u windowSize; // Taille de la fenêtre
    sf::Color LIGHT_COLOR;
    sf::Color DARK_COLOR;
public:
    ChessBoard(int windowWidth, int windowHeight, int size = 8);

    void draw(sf::RenderWindow& window);
};



#endif










