#ifndef CHESSEBOARD_H
#include <SFML/Graphics.hpp>


class ChessBoard {

private:
    int boardSize; // Size of a box in pixels
    sf::Vector2u windowSize; // Window size
    sf::Color LIGHT_COLOR;
    sf::Color DARK_COLOR;
public:
    ChessBoard(int windowWidth, int windowHeight, int size = 8);
    void loadTextures();
    void draw(sf::RenderWindow& window);
    void drawChessPieces(sf::RenderWindow& window, uint64_t piece, sf::Sprite & sprite);
    std::map<std::string, sf::Texture> textures;
    int squareSize;
    sf::Sprite whitePawnSprite;
    sf::Sprite blackPawnSprite;
    sf::Sprite whiteTowerSprite;
    sf::Sprite blackTowerSprite;
    sf::Sprite whiteBishopSprite;
    sf::Sprite blackBishopSprite;
    sf::Sprite whiteKnightSprite;
    sf::Sprite blackKnightSprite;
    sf::Sprite whiteKingSprite;
    sf::Sprite blackKingSprite;
    sf::Sprite whiteQueenSprite;
    sf::Sprite blackQueenSprite;
    uint64_t whitePieceSelected(int &position, uint64_t whitePawns, uint64_t whiteRooks, uint64_t whiteBishops, uint64_t whiteKing, uint64_t whiteQueen, uint64_t whiteKnights);
};



#endif










