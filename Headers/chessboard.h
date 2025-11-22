#ifndef CHESSEBOARD_H
#include <SFML/Graphics.hpp>

enum PieceType { 
    WHITE_PAWN,     // = 0
    WHITE_KNIGHT,   // = 1
    WHITE_BISHOP,   // = 2
    WHITE_ROOK,     // = 3
    WHITE_QUEEN,    // = 4
    WHITE_KING,     // = 5
    BLACK_PAWN,     // = 6
    BLACK_KNIGHT,   // = 7
    BLACK_BISHOP,   // = 8
    BLACK_ROOK,     // = 9
    BLACK_QUEEN,    // = 10
    BLACK_KING,     // = 11
    NONE            // = 12 (No piece)
};

struct Move {
    int from;
    int to;
    PieceType piece;          
    PieceType capturedType;   
};

struct Piece {
    
    uint64_t bitboards[12];

    Piece() {
        bitboards[WHITE_PAWN] = 0x000000000000FF00ULL;
        bitboards[WHITE_KNIGHT] = 0x0000000000000042ULL;
        bitboards[WHITE_BISHOP] = 0x0000000000000024ULL;
        bitboards[WHITE_ROOK] = 0x0000000000000081ULL;
        bitboards[WHITE_QUEEN] = 0x0000000000000008ULL;
        bitboards[WHITE_KING] = 0x0000000000000010ULL;

        bitboards[BLACK_PAWN] = 0x00FF000000000000ULL;
        bitboards[BLACK_KNIGHT] = 0x4200000000000000ULL;
        bitboards[BLACK_BISHOP] = 0x2400000000000000ULL;
        bitboards[BLACK_ROOK] = 0x8100000000000000ULL;
        bitboards[BLACK_QUEEN] = 0x0800000000000000ULL;
        bitboards[BLACK_KING] = 0x1000000000000000ULL;
    

    }
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
    sf::Sprite possibilityMove;
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

    std::vector<int> getPositionsPiece(uint64_t piece);

    int possibilityWhiteTower(int position, int* moves);
    int possibilityBlackTower(int position, int* moves);

    int possibilityWhitePawn(int position, int* moves);
    int possibilityBlackPawn(int position, int* moves);
    
    int possibilityWhiteBishop(int position, int* moves);
    int possibilityBlackBishop(int position, int* moves);

    int possibilityWhiteKnight(int position, int* moves);
    int possibilityBlackKnight(int position, int* moves);
    
    int possibilityWhiteQueen(int position, int* moves);
    int possibilityBlackQueen(int position, int* moves);

    int possibilityWhiteKing(int position, int* moves);
    int possibilityBlackKing(int position, int* moves);

    int mouseToPosition(int x, int y, sf::Vector2u& size);

    void movePiece(uint64_t* pieceFrom, uint64_t* pieceTo, int from, int to);
    void undo(int positionFrom, int positionTo, uint64_t* piece, uint64_t* pieceCaptured);
    int evaluate();
    std::vector<Move> allMovesForWhite();
    std::vector<Move> allMovesForBlack();
    inline PieceType getPieceTypeIfThereIsABlackPieceAt(int position);
    inline PieceType getPieceTypeIfThereIsAWhitePieceAt(int position);
    Move getMoveForAPosition(int position, int to, PieceType pieceType, bool white);

    int minMax(int depth, bool isWhite);
    void AI_chess();
};



#endif










