#ifndef CHESSEBOARD_H
#include <SFML/Graphics.hpp>


struct Piece {
    uint64_t whitePawns;
    uint64_t blackPawns;

    uint64_t whiteRooks;
    uint64_t blackRooks;

    uint64_t whiteKnights;
    uint64_t blackKnights;

    uint64_t whiteBishops;
    uint64_t blackBishops;

    uint64_t whiteQueen;
    uint64_t blackQueen;

    uint64_t whiteKing;
    uint64_t blackKing;

    Piece()
        : whitePawns(0x000000000000FF00ULL),
          blackPawns(0x00FF000000000000ULL),
          whiteRooks(0x0000000000000081ULL),
          blackRooks(0x8100000000000000ULL),
          whiteKnights(0x0000000000000042ULL),
          blackKnights(0x4200000000000000ULL),
          whiteBishops(0x0000000000000024ULL),
          blackBishops(0x2400000000000000ULL),
          whiteQueen(0x0000000000000008ULL),
          blackQueen(0x0800000000000000ULL),
          whiteKing(0x0000000000000010ULL),
          blackKing(0x1000000000000000ULL)
    {}
};

class ChessBoard {

private:
    int boardSize; // Size of a box in pixels
    sf::Vector2u windowSize; // Window size
    sf::Color LIGHT_COLOR;
    sf::Color DARK_COLOR;
public:
    Piece piece;
    ChessBoard(int windowWidth, int windowHeight, int size = 8);
    void loadTextures();
    void draw(sf::RenderWindow& window);
    void drawChessPieces(sf::RenderWindow& window, uint64_t piece, sf::Sprite& sprite);
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
    uint64_t& whitePieceSelected(int &position);
    uint64_t& PieceSelected(int& position);
    void drawAllPieces(sf::RenderWindow& window);

    inline bool isThereAWhitePieceAt(int position);
    inline bool isThereABlackPieceAt(int position);
    inline bool isThereAPieceAt(int position);

    int possibilityWhitePawn(int position, int* moves);
    int mouseToPosition(int x, int y, sf::Vector2u& size);
};



#endif










