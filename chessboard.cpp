#include "Headers/chessboard.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <string>

ChessBoard::ChessBoard(int windowWidth, int windowHeight, int size)
    : windowSize(windowWidth, windowHeight),
      boardSize(size),
      LIGHT_COLOR(255, 255, 255),
      DARK_COLOR(0, 0, 0) {

    squareSize = windowWidth / boardSize;

    loadTextures();
}

void ChessBoard::loadTextures() {

    std::map<std::string, std::string> textureFiles = {
        {"attaque", "images/attaque.png"},
        {"attaque_2", "images/attaque_2.png"},
        {"cavalier", "images/cavalier.png"},
        {"cavalier_b", "images/cavalier_b.png"},
        {"fou", "images/fou.png"},
        {"fou_b", "images/fou_b.png"},
        {"pion", "images/pion.png"},
        {"pion_b", "images/pion_b.png"},
        {"reine", "images/reine.png"},
        {"reine_b", "images/reine_b.png"},
        {"roi", "images/roi.png"},
        {"roi_b", "images/roi_b.png"},
        {"tour", "images/tour.png"},
        {"tour_b", "images/tour_b.png"},
        {"plateau", "images/plateau.jpg"}
    };

    for (const auto& pair : textureFiles) {
        sf::Texture texture;
        if (!texture.loadFromFile(pair.second)) {
            std::cerr << "Error: Unable to load" << pair.second << std::endl;
        } else {
            textures[pair.first] = texture;
        }
    }
    possibilityMove.setTexture(textures["attaque"]);
    whitePawnSprite.setTexture(textures["pion_b"]);
    blackPawnSprite.setTexture(textures["pion"]);
    whiteTowerSprite.setTexture(textures["tour_b"]);
    blackTowerSprite.setTexture(textures["tour"]);
    whiteBishopSprite.setTexture(textures["fou_b"]);
    blackBishopSprite.setTexture(textures["fou"]);
    whiteKnightSprite.setTexture(textures["cavalier_b"]);
    blackKnightSprite.setTexture(textures["cavalier"]);
    whiteKingSprite.setTexture(textures["roi_b"]);
    blackKingSprite.setTexture(textures["roi"]);
    whiteQueenSprite.setTexture(textures["reine_b"]);
    blackQueenSprite.setTexture(textures["reine"]);

    
}

void ChessBoard::draw(sf::RenderWindow& window) {
    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
            square.setPosition(col * squareSize, row * squareSize);
            square.setFillColor((row + col) % 2 == 0 ? LIGHT_COLOR : DARK_COLOR);
            window.draw(square);
        }
    }
}


std::vector<int> ChessBoard::getPositionsPiece(uint64_t piece) {
    std::vector<int> positions;

    while (piece) {
        int position = __builtin_ctzll(piece);
        positions.push_back(position);
        piece &= piece - 1;
    }

    return positions;
}

void ChessBoard::drawChessPieces(sf::RenderWindow& window, uint64_t piece, sf::Sprite & sprite) {

    std::vector<int> positions = getPositionsPiece(piece);

    for (int n : positions) {
        sprite.setPosition( (n % 8) * squareSize  , (squareSize * 7) - (n / 8) * squareSize);
        window.draw(sprite); 
        }
}


void ChessBoard::drawAllPieces(sf::RenderWindow& window) {

    // Pawns :
    drawChessPieces(window, piece.bitboards[WHITE_PAWN], whitePawnSprite);
    drawChessPieces(window, piece.bitboards[BLACK_PAWN], blackPawnSprite);

    // Rooks:
    drawChessPieces(window, piece.bitboards[WHITE_ROOK], whiteTowerSprite);
    drawChessPieces(window, piece.bitboards[BLACK_ROOK], blackTowerSprite);

    // Bishops:
    drawChessPieces(window, piece.bitboards[WHITE_BISHOP], whiteBishopSprite);
    drawChessPieces(window, piece.bitboards[BLACK_BISHOP], blackBishopSprite);

    // Knights:
    drawChessPieces(window, piece.bitboards[WHITE_KNIGHT], whiteKnightSprite);
    drawChessPieces(window, piece.bitboards[BLACK_KNIGHT], blackKnightSprite);

    //Kings:
    drawChessPieces(window, piece.bitboards[WHITE_KING], whiteKingSprite);
    drawChessPieces(window, piece.bitboards[BLACK_KING], blackKingSprite);

    //Queens:
    drawChessPieces(window, piece.bitboards[WHITE_QUEEN], whiteQueenSprite);
    drawChessPieces(window, piece.bitboards[BLACK_QUEEN], blackQueenSprite);

}



 uint64_t& ChessBoard::whitePieceSelected(int& position) {
    if ((piece.bitboards[WHITE_PAWN] >> position) & 1ULL)
        return piece.bitboards[WHITE_PAWN];
    if ((piece.bitboards[WHITE_ROOK] >> position) & 1ULL)
        return piece.bitboards[WHITE_ROOK];
    if ((piece.bitboards[WHITE_BISHOP] >> position) & 1ULL)
        return piece.bitboards[WHITE_BISHOP];
    if ((piece.bitboards[WHITE_KING] >> position) & 1ULL)
        return piece.bitboards[WHITE_KING];
    if ((piece.bitboards[WHITE_QUEEN] >> position) & 1ULL)
        return piece.bitboards[WHITE_QUEEN];
    if ((piece.bitboards[WHITE_KNIGHT] >> position) & 1ULL)
        return piece.bitboards[WHITE_KNIGHT];
    static uint64_t nullPiece = 0x0ULL;
    return nullPiece; 
 }


  uint64_t& ChessBoard::PieceSelected(int& position) {
    if ((piece.bitboards[WHITE_PAWN] >> position) & 1ULL)
        return piece.bitboards[WHITE_PAWN];
    if ((piece.bitboards[WHITE_ROOK] >> position) & 1ULL)
        return piece.bitboards[WHITE_ROOK];
    if ((piece.bitboards[WHITE_BISHOP] >> position) & 1ULL)
        return piece.bitboards[WHITE_BISHOP];
    if ((piece.bitboards[WHITE_KING] >> position) & 1ULL)
        return piece.bitboards[WHITE_KING];
    if ((piece.bitboards[WHITE_QUEEN] >> position) & 1ULL)
        return piece.bitboards[WHITE_QUEEN];
    if ((piece.bitboards[WHITE_KNIGHT] >> position) & 1ULL)
        return piece.bitboards[WHITE_KNIGHT];
    if ((piece.bitboards[BLACK_PAWN] >> position) & 1ULL)
        return piece.bitboards[BLACK_PAWN];
    if ((piece.bitboards[BLACK_ROOK] >> position) & 1ULL)
        return piece.bitboards[BLACK_ROOK];
    if ((piece.bitboards[BLACK_BISHOP] >> position) & 1ULL)
        return piece.bitboards[BLACK_BISHOP];
    if ((piece.bitboards[BLACK_KING] >> position) & 1ULL)
        return piece.bitboards[BLACK_KING];
    if ((piece.bitboards[BLACK_QUEEN] >> position) & 1ULL)
        return piece.bitboards[BLACK_QUEEN];
    if ((piece.bitboards[BLACK_KNIGHT] >> position) & 1ULL)
        return piece.bitboards[BLACK_KNIGHT];
    
    static uint64_t nullPiece = 0x0ULL;
    return nullPiece;
 }



 inline bool ChessBoard::isThereAWhitePieceAt(int position) {
    uint64_t mask = 1ULL << position;
    return mask & (piece.bitboards[WHITE_BISHOP] | piece.bitboards[WHITE_KING] |
                   piece.bitboards[WHITE_KNIGHT] | piece.bitboards[WHITE_PAWN] |
                   piece.bitboards[WHITE_QUEEN] | piece.bitboards[WHITE_ROOK]);
 }

 inline bool ChessBoard::isThereABlackPieceAt(int position) {
    uint64_t mask = 1ULL << position;
    return mask & (piece.bitboards[BLACK_BISHOP] | piece.bitboards[BLACK_KING] |
                   piece.bitboards[BLACK_KNIGHT] | piece.bitboards[BLACK_PAWN] |
                   piece.bitboards[BLACK_QUEEN] | piece.bitboards[BLACK_ROOK]);
 }


  inline PieceType ChessBoard::getPieceTypeisThereABlackPieceAt(int position) {
    uint64_t mask = 1ULL << position;

    if (mask & (piece.bitboards[BLACK_PAWN])) 
        return BLACK_PAWN;
    if (mask & (piece.bitboards[BLACK_BISHOP])) 
        return BLACK_BISHOP;
    if (mask & (piece.bitboards[BLACK_KNIGHT])) 
        return BLACK_KNIGHT;
    if (mask & (piece.bitboards[BLACK_ROOK])) 
        return BLACK_ROOK;
    if (mask & (piece.bitboards[BLACK_QUEEN])) 
        return BLACK_QUEEN;
    if (mask & (piece.bitboards[BLACK_KING])) 
        return BLACK_KING;

    return NONE;
 }
 
 
 

inline bool ChessBoard::isThereAPieceAt(int position) {
    uint64_t mask = 1ULL << position;
    return mask & (piece.bitboards[WHITE_PAWN] | piece.bitboards[WHITE_ROOK] | piece.bitboards[WHITE_BISHOP] |
                   piece.bitboards[WHITE_KNIGHT] | piece.bitboards[WHITE_QUEEN] | piece.bitboards[WHITE_KING] |
                   piece.bitboards[BLACK_PAWN] | piece.bitboards[BLACK_ROOK] | piece.bitboards[BLACK_BISHOP] |
                   piece.bitboards[BLACK_KNIGHT] | piece.bitboards[BLACK_QUEEN] | piece.bitboards[BLACK_KING]);
}


int ChessBoard::possibilityWhiteTower(int position, int* moves) {

    int count = 0;

    // Move right (+1)
    for (int i = 1; ((position + i) & 7) != 0 && (position + i) < 64; i++) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    // Move left (-1)
    for (int i = -1; ((position + i) & 7) != 7 && (position + i) >= 0; i--) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    // Move up (+8)
    for (int i = 8; (position + i) < 64; i += 8) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    // Move down (-8)
    for (int i = -8; (position + i) >= 0; i -= 8) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    return count;
}



int ChessBoard::possibilityBlackTower(int position, int* moves) {

    int count = 0;

    // Move right (+1)
    for (int i = 1; ((position + i) & 7) != 0 && (position + i) < 64; i++) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    // Move left (-1)
    for (int i = -1; ((position + i) & 7) != 7 && (position + i) >= 0; i--) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    // Move up (+8)
    for (int i = 8; (position + i) < 64; i += 8) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    // Move down (-8)
    for (int i = -8; (position + i) >= 0; i -= 8) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    return count;
}


int ChessBoard::possibilityWhiteKnight(int position, int* moves) {

    int count = 0;

    // 2 Up 1 Right (+17)
    if (((position + 17) < 64) && (position & 7) != 7) {
        
        if (!isThereAWhitePieceAt(position + 17)) 
            moves[count++] = position + 17;
    }

    // 2 Down 1 Right (-15)
    if (((position - 15) >= 0) && (position & 7) != 7) {
        
        if (!isThereAWhitePieceAt(position - 15)) 
            moves[count++] = position - 15;
    }

    // 2 Right 1 Up (+10)
    if (((position + 10) < 64) && (position & 7) < 6) {
        
        if (!isThereAWhitePieceAt(position + 10)) 
            moves[count++] = position + 10;
    }

    // 2 Right 1 Down (-6)
    if (((position - 6) >= 0) && (position & 7) < 6) {
        
        if (!isThereAWhitePieceAt(position - 6)) 
            moves[count++] = position - 6;
    }


    // 2 Left 1 Down (-10)
    if (((position - 10) >= 0) && (position & 7) > 1) {
        
        if (!isThereAWhitePieceAt(position - 10)) 
            moves[count++] = position - 10;
    }

    // 2 Left 1 Up (+6)
    if (((position + 6) < 64) && (position & 7) > 1) {
        
        if (!isThereAWhitePieceAt(position + 6)) 
            moves[count++] = position + 6;
    }


    // 2 Up 1 Left (+15)
    if (((position + 15) < 64) && (position & 7) != 0) {
        
        if (!isThereAWhitePieceAt(position + 15)) 
            moves[count++] = position + 15;
    }


    // 2 Down 1 Left (-17)
    if (((position - 17) >= 0) && (position & 7) != 0) {
        
        if (!isThereAWhitePieceAt(position - 17)) 
            moves[count++] = position - 17;
    }

    
    return count;

}



int ChessBoard::possibilityBlackKnight(int position, int* moves) {

    int count = 0;

    // 2 Up 1 Right (+17)
    if (((position + 17) < 64) && (position & 7) != 7) {
        
        if (!isThereABlackPieceAt(position + 17)) 
            moves[count++] = position + 17;
    }

    // 2 Down 1 Right (-15)
    if (((position - 15) >= 0) && (position & 7) != 7) {
        
        if (!isThereABlackPieceAt(position - 15)) 
            moves[count++] = position - 15;
    }

    // 2 Right 1 Up (+10)
    if (((position + 10) < 64) && (position & 7) < 6) {
        
        if (!isThereABlackPieceAt(position + 10)) 
            moves[count++] = position + 10;
    }

    // 2 Right 1 Down (-6)
    if (((position - 6) >= 0) && (position & 7) < 6) {
        
        if (!isThereABlackPieceAt(position - 6)) 
            moves[count++] = position - 6;
    }


    // 2 Left 1 Down (-10)
    if (((position - 10) >= 0) && (position & 7) > 1) {
        
        if (!isThereABlackPieceAt(position - 10)) 
            moves[count++] = position - 10;
    }

    // 2 Left 1 Up (+6)
    if (((position + 6) < 64) && (position & 7) > 1) {
        
        if (!isThereABlackPieceAt(position + 6)) 
            moves[count++] = position + 6;
    }


    // 2 Up 1 Left (+15)
    if (((position + 15) < 64) && (position & 7) != 0) {
        
        if (!isThereABlackPieceAt(position + 15)) 
            moves[count++] = position + 15;
    }


    // 2 Down 1 Left (-17)
    if (((position - 17) >= 0) && (position & 7) != 0) {
        
        if (!isThereABlackPieceAt(position - 17)) 
            moves[count++] = position - 17;
    }

    
    return count;

}



int ChessBoard::possibilityWhiteBishop(int position, int* moves) {

    int count = 0;

    // Move up right (+9)
    for (int i = 9; ((position + i) & 7) != 0 && (position + i) < 64; i+= 9) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    // Move up left (+7)
    for (int i = 7; (((position + i) & 7) != 7) && (position + i) < 64; i+= 7) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    // Move down right (-7)
    for (int i = -7; (((position + i) & 7) != 0) && (position + i) >= 0; i-= 7) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    // Move down left (-9)
    for (int i = -9; (((position + i) & 7) != 7) && (position + i) >= 0; i-= 9) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    return count;
}



int ChessBoard::possibilityBlackBishop(int position, int* moves) {

    int count = 0;
    
    // Move up right (+9)
    for (int i = 9; (((position + i) & 7) != 0) && (position + i) < 64; i+= 9) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    // Move up left (+7)
    for (int i = 7; (((position + i) & 7) != 7) && (position + i) < 64; i+= 7) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    // Move down right (-7)
    for (int i = -7; (((position + i) & 7) != 0) && (position + i) >= 0; i-= 7) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    // Move down left (-9)
    for (int i = -9; (((position + i) & 7) != 7) && (position + i) >= 0; i-= 9) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    return count;
}



int ChessBoard::possibilityWhitePawn(int position, int* moves) {
   
    int count = 0;
    if (!isThereAPieceAt(position + 8)) {
        moves[count++] = position + 8;

        if (((position >> 3) == 1) && !isThereAPieceAt(position + 16)) // (position >> 3) == 1  [position < 16 && position > 7]
            moves[count++] = position + 16;                            // 8 = 1000, 15 = 1111
    }

    // left part of the board
    if (((position & 7) != 0) && isThereABlackPieceAt(position + 7)) // 7 = 111 [position & 7 != 0] = [position % 8 != 0] 
        moves[count++] = position + 7;

    // right part of the board
    if (((position & 7) != 7) && isThereABlackPieceAt(position + 9))
        moves[count++] = position + 9;

    return count;
 }


 
int ChessBoard::possibilityBlackPawn(int position, int* moves) {
   
    int count = 0;
    if (!isThereAPieceAt(position - 8)) {
        moves[count++] = position - 8;

        if ((position >> 3) == 6 && !isThereAPieceAt(position - 16)) // (position >> 3) == 1  [position < 16 && position > 7]
            moves[count++] = position - 16;                            // 8 = 1000, 15 = 1111
    }

    // left part of the board
    if (((position & 7) != 0) && isThereAWhitePieceAt(position - 7)) 
        moves[count++] = position - 7;

    // right part of the board
    if (((position & 7) != 7) && isThereAWhitePieceAt(position - 9))
        moves[count++] = position - 9;

    return count;
 }



 
int ChessBoard::possibilityWhiteQueen(int position, int* moves) {

    int count = 0;

    // Like Bishop :

    // Move up right (+9)
    for (int i = 9; ((position + i) & 7) != 0 && (position + i) < 64; i+= 9) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    // Move up left (+7)
    for (int i = 7; (((position + i) & 7) != 7) && (position + i) < 64; i+= 7) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    // Move down right (-7)
    for (int i = -7; (((position + i) & 7) != 0) && (position + i) >= 0; i-= 7) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    // Move down left (-9)
    for (int i = -9; (((position + i) & 7) != 7) && (position + i) >= 0; i-= 9) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    // like Tower :
    // Move right (+1)
    for (int i = 1; ((position + i) & 7) != 0 && (position + i) < 64; i++) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    // Move left (-1)
    for (int i = -1; ((position + i) & 7) != 7 && (position + i) >= 0; i--) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    // Move up (+8)
    for (int i = 8; (position + i) < 64; i += 8) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    // Move down (-8)
    for (int i = -8; (position + i) >= 0; i -= 8) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    return count;
}



int ChessBoard::possibilityBlackQueen(int position, int* moves) {

    int count = 0;

    // Like Bishop :

    // Move up right (+9)
    for (int i = 9; (((position + i) & 7) != 0) && (position + i) < 64; i+= 9) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    // Move up left (+7)
    for (int i = 7; (((position + i) & 7) != 7) && (position + i) < 64; i+= 7) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    // Move down right (-7)
    for (int i = -7; (((position + i) & 7) != 0) && (position + i) >= 0; i-= 7) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    // Move down left (-9)
    for (int i = -9; (((position + i) & 7) != 7) && (position + i) >= 0; i-= 9) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    // like Tower :
    // Move right (+1)
    for (int i = 1; ((position + i) & 7) != 0 && (position + i) < 64; i++) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    // Move left (-1)
    for (int i = -1; ((position + i) & 7) != 7  && (position + i) >= 0; i--) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    // Move up (+8)
    for (int i = 8; (position + i) < 64; i += 8) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    // Move down (-8)
    for (int i = -8; (position + i) >= 0; i -= 8) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    return count;
}


int ChessBoard::possibilityWhiteKing(int position, int* moves) {

    int count = 0;

    // Move up right (+9)
    if (((position & 7) != 7) && (position + 9) < 64 && !isThereAWhitePieceAt(position + 9)) 
        moves[count++] = position + 9;


    // Move up left (+7)
    if ((position & 7) != 0 && (position + 7) < 64 &&  !isThereAWhitePieceAt(position + 7)) 
        moves[count++] = position + 7;

    // Move down right (-7)
    if ((position & 7) != 7 && (position - 7) >= 0 &&  !isThereAWhitePieceAt(position - 7)) 
        moves[count++] = position - 7;

    // Move down left (-9)
    if ((position & 7) != 0 && (position - 9) >= 0 &&  !isThereAWhitePieceAt(position - 9)) 
        moves[count++] = position - 9;

    // Move right (+1)
    if (((position & 7) != 7) && (position + 1) < 64 && !isThereAWhitePieceAt(position + 1)) 
        moves[count++] = position + 1;
    
    // Move left (-1)
    if (((position & 7) != 0) && (position - 1) >= 0 && !isThereAWhitePieceAt(position - 1)) 
        moves[count++] = position - 1;
    
    // Move up (+8)
    if ((position + 8) < 64 && !isThereAWhitePieceAt(position + 8)) 
        moves[count++] = position + 8;
    
    // Move down (-8)
    if ((position - 8) >= 0 && !isThereAWhitePieceAt(position - 8)) 
        moves[count++] = position - 8;
   

    return count;
}


int ChessBoard::possibilityBlackKing(int position, int* moves) {

    int count = 0;

    // Move up right (+9)
    if (((position & 7) != 7) && (position + 9) < 64 && !isThereABlackPieceAt(position + 9)) 
        moves[count++] = position + 9;


    // Move up left (+7)
    if ((position & 7) != 0 && (position + 7) < 64 && !isThereABlackPieceAt(position + 7)) 
        moves[count++] = position + 7;

    // Move down right (-7)
    if ((position & 7) != 7 && (position - 7) >= 0 && !isThereABlackPieceAt(position - 7)) 
        moves[count++] = position - 7;

    // Move down left (-9)
    if ((position & 7) != 0 && (position - 9) >= 0 && !isThereABlackPieceAt(position - 9)) 
        moves[count++] = position - 9;

    // Move right (+1)
    if (((position & 7) != 7) && (position + 1) < 64 && !isThereABlackPieceAt(position + 1)) 
        moves[count++] = position + 1;
    
    // Move left (-1)
    if (((position & 7) != 0) && (position - 1) >= 0 && !isThereABlackPieceAt(position - 1)) 
        moves[count++] = position - 1;
    
    // Move up (+8)
    if ((position + 8) < 64 && !isThereABlackPieceAt(position + 8)) 
        moves[count++] = position + 8;
    
    // Move down (-8)
    if ((position - 8) >= 0 && !isThereABlackPieceAt(position - 8)) 
        moves[count++] = position - 8;
   

    return count;
}

void ChessBoard::undo(int positionFrom, int positionTo, uint64_t* piece, uint64_t* pieceCaptured) {
    *piece &= ~(1ULL << positionTo); // delete the old position of the piece
    *piece |= (1ULL << positionFrom); // add the old position of the piece
    if (*pieceCaptured != 0x0ULL)
        *pieceCaptured |= (1ULL << positionTo); // add a piece if there is a piece
}

int ChessBoard::evaluate() {
    int score = 0;

    score += __builtin_popcountll(piece.bitboards[WHITE_PAWN]);
    score += __builtin_popcountll(piece.bitboards[WHITE_BISHOP]) * 3;
    score += __builtin_popcountll(piece.bitboards[WHITE_KNIGHT]) * 3;
    score += __builtin_popcountll(piece.bitboards[WHITE_ROOK]) * 5;
    score += __builtin_popcountll(piece.bitboards[WHITE_QUEEN]) * 9;
    score += __builtin_popcountll(piece.bitboards[WHITE_KING]) * 20;

    score -= __builtin_popcountll(piece.bitboards[BLACK_PAWN]);
    score -= __builtin_popcountll(piece.bitboards[BLACK_BISHOP]) * 3;
    score -= __builtin_popcountll(piece.bitboards[BLACK_KNIGHT]) * 3;
    score -= __builtin_popcountll(piece.bitboards[BLACK_ROOK]) * 5;
    score -= __builtin_popcountll(piece.bitboards[BLACK_QUEEN]) * 9;
    score -= __builtin_popcountll(piece.bitboards[BLACK_KING]) * 20;

    return score;
}
 

int ChessBoard::mouseToPosition(int x, int y, sf::Vector2u& size) {
    float square_x = static_cast<float>(size.x) / 8.f;
    float square_y = static_cast<float>(size.y) / 8.f;

    int y_ = 7 - (static_cast<int>(y / square_y) );

    int position =  y_ * 8 +  static_cast<int>(x / square_x);

    return position;
 }

 void ChessBoard::movePiece(uint64_t* pieceFrom, uint64_t* pieceTo, int from, int to) {
    *pieceFrom &= ~(1ULL << from); // delete the piece
    *pieceFrom |= (1ULL << to); // add the new position of the piece
    *pieceTo &= ~(1ULL << to); // delete a piece if there is a piece
 }


 std::vector<Move> ChessBoard::allMovesForWhite() {
    std::vector<Move> moves;

    std::vector<int> positions = getPositionsPiece(piece.bitboards[WHITE_PAWN]);

    for (int position : positions) {

        int moves[4];
        int counts = possibilityWhitePawn(position, moves);

        for (int i = 0; i < counts; ++i) {
            Move move;
            move.from = (uint8_t) position;
            move.to = (uint8_t) moves[i];
            move.piece = WHITE_PAWN;
            //if ()
        }

    }

    return moves;

    


 }


int ChessBoard::minMax(int depth, bool isWhite) {
    if (depth == 0)
        return evaluate();

    if (isWhite) {
        int max_ = 1000;

        //possibilityWhitePawn



        return max_;
    } else {
        int min_ = -1000;

        return min_;
    }
 }
 




