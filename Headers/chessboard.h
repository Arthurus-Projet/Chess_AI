#ifndef CHESSEBOARD_H
#include <SFML/Graphics.hpp>


class ChessBoard {

private:
    int boardSize;      
         // Size of a box in pixels
    sf::Vector2u windowSize; // Window size
    sf::Color LIGHT_COLOR;
    sf::Color DARK_COLOR;
public:
    ChessBoard(int windowWidth, int windowHeight, int size = 8);
    void loadTextures();
    void draw(sf::RenderWindow& window);
    std::map<std::string, sf::Texture> textures;
    int squareSize;
};



#endif










