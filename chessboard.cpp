#include "Headers/chessboard.h"
#include "Headers/ZobristHashing.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <string>
#include <chrono>

ChessBoard::ChessBoard(int windowWidth, int windowHeight, int size, sf::RenderWindow& window)
    : windowSize(windowWidth, windowHeight),
      boardSize(size),
      LIGHT_COLOR(223, 227, 185),
      DARK_COLOR(156, 125, 94),
      zobrist(0x123456789ABCDEF0ULL),
      currentHash(0ULL),
      transpositionTable(),
      hashHistory(),
      window(window) {

      
      squareSize = windowWidth / boardSize;
      loadTextures();
      currentHash = computeInitialHash();
}


uint64_t ChessBoard::computeInitialHash() {

    uint64_t hash = 0ULL;

    for (int i = 0; i < 64; ++i) {
        for (int pieceType = 0; pieceType < 12; ++pieceType) {
            if ((piece.bitboards[pieceType] >> i) & 1ULL) {
                hash ^= zobrist.pieceSquare[pieceType][i];
                break;
            }
        }
    }

    // Castling
    int num =  whiteKingSideCastling |
          (whiteQueenSideCastling << 1) |
          (blackKingSideCastling  << 2) |
           (blackQueenSideCastling << 3);

    hash ^= zobrist.castlingRights[num];

    return hash;
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

void ChessBoard::draw() {
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

void ChessBoard::drawChessPieces(uint64_t piece, sf::Sprite& sprite) {

    std::vector<int> positions = getPositionsPiece(piece);

    for (int n : positions) {
        sprite.setPosition( (n % 8) * squareSize  , (squareSize * 7) - (n / 8) * squareSize);
        window.draw(sprite); 
        }
}


void ChessBoard::drawAllPieces() {
    const std::pair<int, sf::Sprite&> pieces[] = {
       {WHITE_PAWN, whitePawnSprite}, 
       {BLACK_PAWN, blackPawnSprite}, 
       {WHITE_ROOK, whiteTowerSprite}, 
       {BLACK_ROOK, blackTowerSprite}, 
       {WHITE_BISHOP, whiteBishopSprite}, 
       {BLACK_BISHOP, blackBishopSprite}, 
       {WHITE_KNIGHT, whiteKnightSprite}, 
       {BLACK_KNIGHT, blackKnightSprite}, 
       {WHITE_KING, whiteKingSprite}, 
       {BLACK_KING, blackKingSprite}, 
       {WHITE_QUEEN, whiteQueenSprite}, 
       {BLACK_QUEEN, blackQueenSprite}
  
    };

    for (const auto& [pieceType, sprite] : pieces) {
        drawChessPieces(piece.bitboards[pieceType], sprite);
    }

}



uint64_t& ChessBoard::whitePieceSelected(int& position) {

    for (int i = 0; i < 6; ++i) {
        if ((piece.bitboards[i] >> position) & 1ULL)
        return piece.bitboards[i];
    }

    static uint64_t nullPiece = 0x0ULL;
    return nullPiece; 
 }


  uint64_t& ChessBoard::PieceSelected(int& position) {
   
    for (int i = 0; i < 12; ++i) {
        if ((piece.bitboards[i] >> position) & 1ULL)
        return piece.bitboards[i];
    }
    
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


inline PieceType ChessBoard::getPieceTypeIfThereIsAWhitePieceAt(int position) {
    uint64_t mask = 1ULL << position;

    if (mask & (piece.bitboards[WHITE_PAWN])) 
        return WHITE_PAWN;
    if (mask & (piece.bitboards[WHITE_BISHOP])) 
        return WHITE_BISHOP;
    if (mask & (piece.bitboards[WHITE_KNIGHT])) 
        return WHITE_KNIGHT;
    if (mask & (piece.bitboards[WHITE_ROOK])) 
        return WHITE_ROOK;
    if (mask & (piece.bitboards[WHITE_QUEEN])) 
        return WHITE_QUEEN;
    if (mask & (piece.bitboards[WHITE_KING])) 
        return WHITE_KING;

    for (int i = 0; i < 6; ++i) {
        if (mask & (piece.bitboards[i])) 
        return WHITE_KING;
    }

    return NONE;

  }

inline PieceType ChessBoard::getPieceTypeIfThereIsABlackPieceAt(int position) {
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

    std::vector<int> directions = {1, - 1, 8, -8};

    for (int direction : directions) {

        for (int i = direction; ; i+= direction) {

            if ((position + i) > 63) break; 
            if ((position + i) < 0) break;

            if ((((position + i) & 7) == 0) && (direction == 1))  break;
            if ((((position + i) & 7) == 7) && (direction == -1)) break;

            if (isThereAWhitePieceAt(position + i))
                break;

            moves[count++] = position + i;
            if (isThereABlackPieceAt(position + i)) { 
                break;
                }
        
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

    // en Passant
    // right part of the board
    if (((position & 7) != 7) && enPassant == (position + 9))
        moves[count++] = position + 9;

    // left part of the board
    if (((position & 7) != 0) && enPassant == (position + 7)) 
        moves[count++] = position + 7;

    return count;
 }


 
int ChessBoard::possibilityBlackPawn(int position, int* moves) {
   
    int count = 0;
    if (!isThereAPieceAt(position - 8)) {
        moves[count++] = position - 8;

        if ((position >> 3) == 6 && !isThereAPieceAt(position - 16)) // (position >> 3) == 6  [position < 16 && position > 7]
            moves[count++] = position - 16;                            // 8 = 1000, 15 = 1111
    }

    // right part of the board
    if (((position & 7) != 7) && isThereAWhitePieceAt(position - 7)) 
        moves[count++] = position - 7;

    // left part of the board
    if (((position & 7) != 0) && isThereAWhitePieceAt(position - 9))
        moves[count++] = position - 9;
    
    // en passant
    // left part of the board
    if (((position & 7) != 0) && enPassant == (position - 9))
        moves[count++] = position - 9;

    if (((position & 7) != 7) && enPassant == (position - 7)) 
        moves[count++] = position - 7;

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

int ChessBoard::evaluatePawnPower() {
    int score = 0;
    int coef = 1;


    uint64_t pawnWhiteBitboard = piece.bitboards[WHITE_PAWN];

    while (pawnWhiteBitboard) {
        int position = __builtin_ctzll(pawnWhiteBitboard);

        score += (position >> 3);  // = /8    
        // pawn = 3, then 4 when he move then 5 ...
   
        pawnWhiteBitboard &= pawnWhiteBitboard - 1;
    }
    
    score += __builtin_popcountll(piece.bitboards[WHITE_BISHOP]) * 3 * coef;
    score += __builtin_popcountll(piece.bitboards[WHITE_KNIGHT]) * 3 * coef;
    score += __builtin_popcountll(piece.bitboards[WHITE_ROOK]) * 5 * coef;
    score += __builtin_popcountll(piece.bitboards[WHITE_QUEEN]) * 9 * coef;
    score += __builtin_popcountll(piece.bitboards[WHITE_KING]) * 20 * coef;

    score -= __builtin_popcountll(piece.bitboards[BLACK_BISHOP]) * 3 * coef;
    score -= __builtin_popcountll(piece.bitboards[BLACK_KNIGHT]) * 3 * coef;
    score -= __builtin_popcountll(piece.bitboards[BLACK_ROOK]) * 5 * coef;
    score -= __builtin_popcountll(piece.bitboards[BLACK_QUEEN]) * 9 * coef;
    score -= __builtin_popcountll(piece.bitboards[BLACK_KING]) * 20 * coef;

    uint64_t pawnBlackBitboard = piece.bitboards[BLACK_PAWN];

    while (pawnBlackBitboard) {
        int position = __builtin_ctzll(pawnBlackBitboard);

        score -= 7 - (position >> 3);      

        pawnBlackBitboard &= pawnBlackBitboard - 1;
    }

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

 void ChessBoard::unMovePiece(uint64_t* pieceFrom, uint64_t* pieceTo, int from, int to) {
    *pieceFrom |= (1ULL << from); // get back the old piece
    *pieceFrom &= ~(1ULL << to); // delete the new position of the piece
    *pieceTo |= (1ULL << to); // add a piece if there is a piece
 }

 Move ChessBoard::getMoveForAPosition(int position, int to, PieceType pieceType, bool white) {
    Move move;
    move.from = position;
    move.to =  to;
    move.piece = pieceType;
    move.moveType = NORMAL_MOVE;        
    move.castlingType = KINGSIDE;  

    if (pieceType == WHITE_PAWN && to == enPassant) {
        move.moveType = EN_PASSANT;
        move.capturedType = BLACK_PAWN;
    } 
    else if (pieceType == BLACK_PAWN && to == enPassant) {
        move.moveType = EN_PASSANT;
        move.capturedType = WHITE_PAWN;
    }
    else {
        move.moveType = NORMAL_MOVE;
        if (white)
            move.capturedType = getPieceTypeIfThereIsABlackPieceAt(to);
        else
            move.capturedType = getPieceTypeIfThereIsAWhitePieceAt(to);
    }
    return move;
 }


bool ChessBoard::isInCheck(bool isWhite) { 
    
    // Find position of the King 
    int position = __builtin_ctzll(piece.bitboards[isWhite? WHITE_KING : BLACK_KING]); 
    uint64_t mask = 1ULL << position; 
    
    // Tower 
    
    // Move right (+1) 
    for (int i = 1; ((position + i) & 7) != 0 && (position + i) < 64; i++) { 
        if (isThereAPieceAt(position + i)) { 
            mask = 1ULL << (position + i); 
            if ((mask & piece.bitboards[isWhite? BLACK_ROOK : WHITE_ROOK]) || (mask & piece.bitboards[isWhite? BLACK_QUEEN : WHITE_QUEEN]))
                return true; 
            break; 
        }
    }

    // Move left (-1)
    for (int i = -1; ((position + i) & 7) != 7 && (position + i) >= 0; i--) {
        if (isThereAPieceAt(position + i)) { 
            mask = 1ULL << (position + i); 
            if ((mask & piece.bitboards[isWhite? BLACK_ROOK : WHITE_ROOK]) || (mask & piece.bitboards[isWhite? BLACK_QUEEN : WHITE_QUEEN]))
                return true; 
            break; 
        }
    }

    // Move up (+8)
    for (int i = 8; (position + i) < 64; i += 8) {
        if (isThereAPieceAt(position + i)) { 
            mask = 1ULL << (position + i); 
            if ((mask & piece.bitboards[isWhite? BLACK_ROOK : WHITE_ROOK]) || (mask & piece.bitboards[isWhite? BLACK_QUEEN : WHITE_QUEEN]))
                return true; 
            break; 
        }
    }

    // Move down (-8)
    for (int i = -8; (position + i) >= 0; i -= 8) {
        if (isThereAPieceAt(position + i)) { 
            mask = 1ULL << (position + i); 
            if ((mask & piece.bitboards[isWhite? BLACK_ROOK : WHITE_ROOK]) || (mask & piece.bitboards[isWhite? BLACK_QUEEN : WHITE_QUEEN]))
                return true; 
            break; 
        }
    }


    // Bishop

    // Move up right (+9)
    for (int i = 9; ((position + i) & 7) != 0 && (position + i) < 64; i+= 9) {
        if (isThereAPieceAt(position + i)) { 
            mask = 1ULL << (position + i); 
            if ((mask & piece.bitboards[isWhite? BLACK_BISHOP : WHITE_BISHOP]) || (mask & piece.bitboards[isWhite? BLACK_QUEEN : WHITE_QUEEN]))
                return true; 
            break; 
        }
    }

    // Move up left (+7)
    for (int i = 7; (((position + i) & 7) != 7) && (position + i) < 64; i+= 7) {
       if (isThereAPieceAt(position + i)) { 
            mask = 1ULL << (position + i); 
            if ((mask & piece.bitboards[isWhite? BLACK_BISHOP : WHITE_BISHOP]) || (mask & piece.bitboards[isWhite? BLACK_QUEEN : WHITE_QUEEN]))
                return true; 
            break; 
        }
    }

    // Move down right (-7)
    for (int i = -7; (((position + i) & 7) != 0) && (position + i) >= 0; i-= 7) {
        if (isThereAPieceAt(position + i)) { 
            mask = 1ULL << (position + i); 
            if ((mask & piece.bitboards[isWhite? BLACK_BISHOP : WHITE_BISHOP]) || (mask & piece.bitboards[isWhite? BLACK_QUEEN : WHITE_QUEEN]))
                return true; 
            break; 
        }
    }

    // Move down left (-9)
    for (int i = -9; (((position + i) & 7) != 7) && (position + i) >= 0; i-= 9) {
        if (isThereAPieceAt(position + i)) { 
            mask = 1ULL << (position + i); 
            if ((mask & piece.bitboards[isWhite? BLACK_BISHOP : WHITE_BISHOP]) || (mask & piece.bitboards[isWhite? BLACK_QUEEN : WHITE_QUEEN]))
                return true; 
            break; 
        }
    }

    // Knight

    // 2 Up 1 Right (+17)
    if (((position + 17) < 64) && (position & 7) != 7) {
        if (isThereAPieceAt(position + 17)) { 
            mask = 1ULL << (position + 17); 
            if (mask & piece.bitboards[isWhite? BLACK_KNIGHT : WHITE_KNIGHT])
                return true; 
    
        }
    }

    // 2 Down 1 Right (-15)
    if (((position - 15) >= 0) && (position & 7) != 7) {
        if (isThereAPieceAt(position - 15)) { 
            mask = 1ULL << (position - 15); 
            if (mask & piece.bitboards[isWhite? BLACK_KNIGHT : WHITE_KNIGHT]) 
                return true; 
     
        }
    }

    // 2 Right 1 Up (+10)
    if (((position + 10) < 64) && (position & 7) < 6) {
        if (isThereAPieceAt(position + 10)) { 
            mask = 1ULL << (position + 10); 
            if (mask & piece.bitboards[isWhite? BLACK_KNIGHT : WHITE_KNIGHT])
                return true; 
       
        }
    }

    // 2 Right 1 Down (-6)
    if (((position - 6) >= 0) && (position & 7) < 6) {
        if (isThereAPieceAt(position - 6)) { 
            mask = 1ULL << (position - 6); 
            if (mask & piece.bitboards[isWhite? BLACK_KNIGHT : WHITE_KNIGHT])
                return true; 
    
        }
    }


    // 2 Left 1 Down (-10)
    if (((position - 10) >= 0) && (position & 7) > 1) {
        if (isThereAPieceAt(position - 10)) { 
            mask = 1ULL << (position - 10); 
            if (mask & piece.bitboards[isWhite? BLACK_KNIGHT : WHITE_KNIGHT])
                return true; 
       
        }
    }

    // 2 Left 1 Up (+6)
    if (((position + 6) < 64) && (position & 7) > 1) {
        if (isThereAPieceAt(position + 6)) { 
            mask = 1ULL << (position + 6); 
            if (mask & piece.bitboards[isWhite? BLACK_KNIGHT : WHITE_KNIGHT]) 
                return true; 
       
        }
    }


    // 2 Up 1 Left (+15)
    if (((position + 15) < 64) && (position & 7) != 0) {
        if (isThereAPieceAt(position + 15)) { 
            mask = 1ULL << (position + 15); 
            if (mask & piece.bitboards[isWhite? BLACK_KNIGHT : WHITE_KNIGHT])
                return true; 
        
        }
    }


    // 2 Down 1 Left (-17)
    if (((position - 17) >= 0) && (position & 7) != 0) {
        if (isThereAPieceAt(position - 17)) { 
            mask = 1ULL << (position - 17); 
            if (mask & piece.bitboards[isWhite? BLACK_KNIGHT : WHITE_KNIGHT])
                return true; 
            
        }
    }


    //  Pawn
    if (isWhite) {

        // left part of the board
        if (((position & 7) != 0) && isThereABlackPieceAt(position + 7)) {
            mask = 1ULL << (position + 7); 
            if (mask & piece.bitboards[BLACK_PAWN]) 
                return true; 
        }

        // right part of the board
        if (((position & 7) != 7) && isThereABlackPieceAt(position + 9)) {
            mask = 1ULL << (position + 9); 
            if (mask & piece.bitboards[BLACK_PAWN]) 
                return true; 
        }
    } else {
        // right part of the board
        if (((position & 7) != 7) && isThereAWhitePieceAt(position - 7)) {
                mask = 1ULL << (position - 7); 
                if (mask & piece.bitboards[WHITE_PAWN]) 
                    return true; 
            }

        // left part of the board
        if (((position & 7) != 0) && isThereAWhitePieceAt(position - 9)) {
                mask = 1ULL << (position - 9); 
                if (mask & piece.bitboards[WHITE_PAWN]) 
                    return true; 
            }

    }

    // King 

    // Move up right (+9)
    if (((position & 7) != 7) && (position + 9) < 64 ) {
        mask = 1ULL << (position + 9); 
        if (mask & piece.bitboards[isWhite? BLACK_KING : WHITE_KING])
            return true; 
        }


    // Move up left (+7)
    if ((position & 7) != 0 && (position + 7) < 64)  {
        mask = 1ULL << (position + 7); 
        if (mask & piece.bitboards[isWhite? BLACK_KING : WHITE_KING])
            return true; 
        }

    // Move down right (-7)
    if ((position & 7) != 7 && (position - 7) >= 0)  {
        mask = 1ULL << (position - 7); 
        if (mask & piece.bitboards[isWhite? BLACK_KING : WHITE_KING])
            return true; 
        }

    // Move down left (-9)
    if ((position & 7) != 0 && (position - 9) >= 0)  {
        mask = 1ULL << (position - 9); 
        if (mask & piece.bitboards[isWhite? BLACK_KING : WHITE_KING])
            return true; 
        }

    // Move right (+1)
    if (((position & 7) != 7) && (position + 1) < 64)  {
        mask = 1ULL << (position + 1); 
        if (mask & piece.bitboards[isWhite? BLACK_KING : WHITE_KING])
            return true; 
        }
    
    // Move left (-1)
    if (((position & 7) != 0) && (position - 1) >= 0)  {
        mask = 1ULL << (position - 1); 
        if (mask & piece.bitboards[isWhite? BLACK_KING : WHITE_KING])
            return true; 
        }
    
    // Move up (+8)
    if ((position + 8) < 64)  {
        mask = 1ULL << (position + 8); 
        if (mask & piece.bitboards[isWhite? BLACK_KING : WHITE_KING])
            return true; 
        }
    
    // Move down (-8)
    if ((position - 8) >= 0)  {
        mask = 1ULL << (position - 8); 
        if (mask & piece.bitboards[isWhite? BLACK_KING : WHITE_KING])
            return true; 
        }


    return false;
}



bool ChessBoard::isAttacked(int position, bool isWhite) { 
    
    uint64_t mask = 1ULL << position; 
    
    // Tower 
    
    // Move right (+1) 
    for (int i = 1; ((position + i) & 7) != 0 && (position + i) < 64; i++) { 
        if (isThereAPieceAt(position + i)) { 
            mask = 1ULL << (position + i); 
            if ((mask & piece.bitboards[isWhite? BLACK_ROOK : WHITE_ROOK]) || (mask & piece.bitboards[isWhite? BLACK_QUEEN : WHITE_QUEEN]))
                return true; 
            break; 
        }
    }

    // Move left (-1)
    for (int i = -1; ((position + i) & 7) != 7 && (position + i) >= 0; i--) {
        if (isThereAPieceAt(position + i)) { 
            mask = 1ULL << (position + i); 
            if ((mask & piece.bitboards[isWhite? BLACK_ROOK : WHITE_ROOK]) || (mask & piece.bitboards[isWhite? BLACK_QUEEN : WHITE_QUEEN]))
                return true; 
            break; 
        }
    }

    // Move up (+8)
    for (int i = 8; (position + i) < 64; i += 8) {
        if (isThereAPieceAt(position + i)) { 
            mask = 1ULL << (position + i); 
            if ((mask & piece.bitboards[isWhite? BLACK_ROOK : WHITE_ROOK]) || (mask & piece.bitboards[isWhite? BLACK_QUEEN : WHITE_QUEEN]))
                return true; 
            break; 
        }
    }

    // Move down (-8)
    for (int i = -8; (position + i) >= 0; i -= 8) {
        if (isThereAPieceAt(position + i)) { 
            mask = 1ULL << (position + i); 
            if ((mask & piece.bitboards[isWhite? BLACK_ROOK : WHITE_ROOK]) || (mask & piece.bitboards[isWhite? BLACK_QUEEN : WHITE_QUEEN]))
                return true; 
            break; 
        }
    }


    // Bishop

    // Move up right (+9)
    for (int i = 9; ((position + i) & 7) != 0 && (position + i) < 64; i+= 9) {
        if (isThereAPieceAt(position + i)) { 
            mask = 1ULL << (position + i); 
            if ((mask & piece.bitboards[isWhite? BLACK_BISHOP : WHITE_BISHOP]) || (mask & piece.bitboards[isWhite? BLACK_QUEEN : WHITE_QUEEN]))
                return true; 
            break; 
        }
    }

    // Move up left (+7)
    for (int i = 7; (((position + i) & 7) != 7) && (position + i) < 64; i+= 7) {
       if (isThereAPieceAt(position + i)) { 
            mask = 1ULL << (position + i); 
            if ((mask & piece.bitboards[isWhite? BLACK_BISHOP : WHITE_BISHOP]) || (mask & piece.bitboards[isWhite? BLACK_QUEEN : WHITE_QUEEN]))
                return true; 
            break; 
        }
    }

    // Move down right (-7)
    for (int i = -7; (((position + i) & 7) != 0) && (position + i) >= 0; i-= 7) {
        if (isThereAPieceAt(position + i)) { 
            mask = 1ULL << (position + i); 
            if ((mask & piece.bitboards[isWhite? BLACK_BISHOP : WHITE_BISHOP]) || (mask & piece.bitboards[isWhite? BLACK_QUEEN : WHITE_QUEEN]))
                return true; 
            break; 
        }
    }

    // Move down left (-9)
    for (int i = -9; (((position + i) & 7) != 7) && (position + i) >= 0; i-= 9) {
        if (isThereAPieceAt(position + i)) { 
            mask = 1ULL << (position + i); 
            if ((mask & piece.bitboards[isWhite? BLACK_BISHOP : WHITE_BISHOP]) || (mask & piece.bitboards[isWhite? BLACK_QUEEN : WHITE_QUEEN]))
                return true; 
            break; 
        }
    }

    // Knight

    // 2 Up 1 Right (+17)
    if (((position + 17) < 64) && (position & 7) != 7) {
        if (isThereAPieceAt(position + 17)) { 
            mask = 1ULL << (position + 17); 
            if (mask & piece.bitboards[isWhite? BLACK_KNIGHT : WHITE_KNIGHT])
                return true; 
    
        }
    }

    // 2 Down 1 Right (-15)
    if (((position - 15) >= 0) && (position & 7) != 7) {
        if (isThereAPieceAt(position - 15)) { 
            mask = 1ULL << (position - 15); 
            if (mask & piece.bitboards[isWhite? BLACK_KNIGHT : WHITE_KNIGHT]) 
                return true; 
     
        }
    }

    // 2 Right 1 Up (+10)
    if (((position + 10) < 64) && (position & 7) < 6) {
        if (isThereAPieceAt(position + 10)) { 
            mask = 1ULL << (position + 10); 
            if (mask & piece.bitboards[isWhite? BLACK_KNIGHT : WHITE_KNIGHT])
                return true; 
       
        }
    }

    // 2 Right 1 Down (-6)
    if (((position - 6) >= 0) && (position & 7) < 6) {
        if (isThereAPieceAt(position - 6)) { 
            mask = 1ULL << (position - 6); 
            if (mask & piece.bitboards[isWhite? BLACK_KNIGHT : WHITE_KNIGHT])
                return true; 
    
        }
    }


    // 2 Left 1 Down (-10)
    if (((position - 10) >= 0) && (position & 7) > 1) {
        if (isThereAPieceAt(position - 10)) { 
            mask = 1ULL << (position - 10); 
            if (mask & piece.bitboards[isWhite? BLACK_KNIGHT : WHITE_KNIGHT])
                return true; 
       
        }
    }

    // 2 Left 1 Up (+6)
    if (((position + 6) < 64) && (position & 7) > 1) {
        if (isThereAPieceAt(position + 6)) { 
            mask = 1ULL << (position + 6); 
            if (mask & piece.bitboards[isWhite? BLACK_KNIGHT : WHITE_KNIGHT]) 
                return true; 
       
        }
    }


    // 2 Up 1 Left (+15)
    if (((position + 15) < 64) && (position & 7) != 0) {
        if (isThereAPieceAt(position + 15)) { 
            mask = 1ULL << (position + 15); 
            if (mask & piece.bitboards[isWhite? BLACK_KNIGHT : WHITE_KNIGHT])
                return true; 
        
        }
    }


    // 2 Down 1 Left (-17)
    if (((position - 17) >= 0) && (position & 7) != 0) {
        if (isThereAPieceAt(position - 17)) { 
            mask = 1ULL << (position - 17); 
            if (mask & piece.bitboards[isWhite? BLACK_KNIGHT : WHITE_KNIGHT])
                return true; 
            
        }
    }

    //  Pawn
    if (isWhite) {

        // left part of the board
        if (((position & 7) != 0) && isThereABlackPieceAt(position + 7)) {
            mask = 1ULL << (position + 7); 
            if (mask & piece.bitboards[BLACK_PAWN]) 
                return true; 
        }

        // right part of the board
        if (((position & 7) != 7) && isThereABlackPieceAt(position + 9)) {
            mask = 1ULL << (position + 9); 
            if (mask & piece.bitboards[BLACK_PAWN]) 
                return true; 
        }
    } else {
        // right part of the board
        if (((position & 7) != 7) && isThereAWhitePieceAt(position - 7)) {
                mask = 1ULL << (position - 7); 
                if (mask & piece.bitboards[WHITE_PAWN]) 
                    return true; 
            }

        // left part of the board
        if (((position & 7) != 0) && isThereAWhitePieceAt(position - 9)) {
                mask = 1ULL << (position - 9); 
                if (mask & piece.bitboards[WHITE_PAWN]) 
                    return true; 
            }

    }


    // King 

    // Move up right (+9)
    if (((position & 7) != 7) && (position + 9) < 64 ) {
        mask = 1ULL << (position + 9); 
        if (mask & piece.bitboards[isWhite? BLACK_KING : WHITE_KING])
            return true; 
        }


    // Move up left (+7)
    if ((position & 7) != 0 && (position + 7) < 64)  {
        mask = 1ULL << (position + 7); 
        if (mask & piece.bitboards[isWhite? BLACK_KING : WHITE_KING])
            return true; 
        }

    // Move down right (-7)
    if ((position & 7) != 7 && (position - 7) >= 0)  {
        mask = 1ULL << (position - 7); 
        if (mask & piece.bitboards[isWhite? BLACK_KING : WHITE_KING])
            return true; 
        }

    // Move down left (-9)
    if ((position & 7) != 0 && (position - 9) >= 0)  {
        mask = 1ULL << (position - 9); 
        if (mask & piece.bitboards[isWhite? BLACK_KING : WHITE_KING])
            return true; 
        }

    // Move right (+1)
    if (((position & 7) != 7) && (position + 1) < 64)  {
        mask = 1ULL << (position + 1); 
        if (mask & piece.bitboards[isWhite? BLACK_KING : WHITE_KING])
            return true; 
        }
    
    // Move left (-1)
    if (((position & 7) != 0) && (position - 1) >= 0)  {
        mask = 1ULL << (position - 1); 
        if (mask & piece.bitboards[isWhite? BLACK_KING : WHITE_KING])
            return true; 
        }
    
    // Move up (+8)
    if ((position + 8) < 64)  {
        mask = 1ULL << (position + 8); 
        if (mask & piece.bitboards[isWhite? BLACK_KING : WHITE_KING])
            return true; 
        }
    
    // Move down (-8)
    if ((position - 8) >= 0)  {
        mask = 1ULL << (position - 8); 
        if (mask & piece.bitboards[isWhite? BLACK_KING : WHITE_KING])
            return true; 
        }


    return false;
}



void ChessBoard::possibilityCastle(std::vector<Move>& movesList, bool isWhite) {
    if (isWhite) {
        if (whiteKingSideCastling) {
            if (!isThereAPieceAt(5) && !isThereAPieceAt(6)) {
                if (!isAttacked(5, true) && !isAttacked(6, true) && !isInCheck(true)) {
                    Move move;
                    move.piece = WHITE_KING;
                    move.moveType = CASTLING;
                    move.castlingType = KINGSIDE;
                    move.from = 4; // The king go from 4 to 6
                    move.to = 6;
                    move.capturedType = NONE;
                    movesList.push_back(move);
                }
            }
        }

        if (whiteQueenSideCastling) {
            if (!isThereAPieceAt(1) && !isThereAPieceAt(2) && !isThereAPieceAt(3)) {
                if (!isAttacked(1, true) && !isAttacked(2, true) && !isAttacked(3, true) && !isInCheck(true)) {
                    Move move;
                    move.piece = WHITE_KING;
                    move.moveType = CASTLING;
                    move.castlingType = QUEENSIDE;
                    move.from = 4; // The king go from 4 to 2
                    move.to = 2;
                    move.capturedType = NONE;
                    movesList.push_back(move);
                }
            }
        }


    } else {

        if (blackKingSideCastling) {
            if (!isThereAPieceAt(61) && !isThereAPieceAt(62)) {
                if (!isAttacked(61, false) && !isAttacked(62, false) && !isInCheck(false)) {
                    Move move;
                    move.piece = BLACK_KING;
                    move.moveType = CASTLING;
                    move.castlingType = KINGSIDE;
                    move.from = 60; // The king go from 60 to 62
                    move.to = 62;
                    move.capturedType = NONE;
                    movesList.push_back(move);
                }
            }
        }

        if (blackQueenSideCastling) {
            if (!isThereAPieceAt(57) && !isThereAPieceAt(58) && !isThereAPieceAt(59)) {
                if (!isAttacked(57, false) && !isAttacked(58, false) && !isAttacked(59, false) && !isInCheck(false)) {
                    Move move;
                    move.piece = BLACK_KING;
                    move.moveType = CASTLING;
                    move.castlingType = QUEENSIDE;
                    move.from = 60; // The king go from 60 to 58
                    move.to = 58;
                    move.capturedType = NONE;
                    movesList.push_back(move);
                }
            }
        }


    }

}

std::vector<Move> ChessBoard::allMovesForWhite() {
    std::vector<Move> movesList;

    // WHITE_KNIGHT
    std::vector<int> positions = getPositionsPiece(piece.bitboards[WHITE_KNIGHT]);

    for (int position : positions) {
        int moves[8];
        int counts = possibilityWhiteKnight(position, moves);

        for (int i = 0; i < counts; ++i) 
            movesList.push_back(getMoveForAPosition(position, moves[i], WHITE_KNIGHT, true));
    }

    // WHITE_BISHOP
    positions = getPositionsPiece(piece.bitboards[WHITE_BISHOP]);

    for (int position : positions) {
        int moves[14];
        int counts = possibilityWhiteBishop(position, moves);

        for (int i = 0; i < counts; ++i) 
            movesList.push_back(getMoveForAPosition(position, moves[i], WHITE_BISHOP, true));
    }

    // WHITE_ROOK
    positions = getPositionsPiece(piece.bitboards[WHITE_ROOK]);

    for (int position : positions) {
        int moves[16];
        int counts = possibilityWhiteTower(position, moves);

        for (int i = 0; i < counts; ++i) 
            movesList.push_back(getMoveForAPosition(position, moves[i], WHITE_ROOK, true));
    }

    // WHITE_QUEEN
    positions = getPositionsPiece(piece.bitboards[WHITE_QUEEN]);

    for (int position : positions) {
        int moves[27];
        int counts = possibilityWhiteQueen(position, moves);

        for (int i = 0; i < counts; ++i) 
            movesList.push_back(getMoveForAPosition(position, moves[i], WHITE_QUEEN, true));
    }

    // WHITE_KING
    positions = getPositionsPiece(piece.bitboards[WHITE_KING]);

    for (int position : positions) {
        int moves[27];
        int counts = possibilityWhiteKing(position, moves);

        for (int i = 0; i < counts; ++i) 
            movesList.push_back(getMoveForAPosition(position, moves[i], WHITE_KING, true));
    }

    // WHITE_PAWN
    positions = getPositionsPiece(piece.bitboards[WHITE_PAWN]);

    for (int position : positions) {
        int moves[4];
        int counts = possibilityWhitePawn(position, moves);

        for (int i = 0; i < counts; ++i) 
            movesList.push_back(getMoveForAPosition(position, moves[i], WHITE_PAWN, true));
    }

    possibilityCastle(movesList, true);

    return movesList;
 }



 std::vector<Move> ChessBoard::allMovesForBlack() {
    std::vector<Move> movesList;

    

    // BLACK_KNIGHT
    std::vector<int> positions = getPositionsPiece(piece.bitboards[BLACK_KNIGHT]);

    for (int position : positions) {
        int moves[8];
        int counts = possibilityBlackKnight(position, moves);

        for (int i = 0; i < counts; ++i) 
            movesList.push_back(getMoveForAPosition(position, moves[i], BLACK_KNIGHT, false));
    }

    // BLACK_BISHOP
    positions = getPositionsPiece(piece.bitboards[BLACK_BISHOP]);

    for (int position : positions) {
        int moves[14];
        int counts = possibilityBlackBishop(position, moves);

        for (int i = 0; i < counts; ++i) 
            movesList.push_back(getMoveForAPosition(position, moves[i], BLACK_BISHOP, false));
    }

    // BLACK_ROOK
    positions = getPositionsPiece(piece.bitboards[BLACK_ROOK]);

    for (int position : positions) {
        int moves[16];
        int counts = possibilityBlackTower(position, moves);

        for (int i = 0; i < counts; ++i) 
            movesList.push_back(getMoveForAPosition(position, moves[i], BLACK_ROOK, false));
    }

    // BLACK_QUEEN
    positions = getPositionsPiece(piece.bitboards[BLACK_QUEEN]);

    for (int position : positions) {
        int moves[27];
        int counts = possibilityBlackQueen(position, moves);

        for (int i = 0; i < counts; ++i) 
            movesList.push_back(getMoveForAPosition(position, moves[i], BLACK_QUEEN, false));
    }

    // BLACK_KING
    positions = getPositionsPiece(piece.bitboards[BLACK_KING]);

    for (int position : positions) {
        int moves[27];
        int counts = possibilityBlackKing(position, moves);

        for (int i = 0; i < counts; ++i) 
            movesList.push_back(getMoveForAPosition(position, moves[i], BLACK_KING, false));
    }

    // BLACK_PAWN
    positions = getPositionsPiece(piece.bitboards[BLACK_PAWN]);

    for (int position : positions) {
        int moves[4];
        int counts = possibilityBlackPawn(position, moves);

        for (int i = 0; i < counts; ++i) 
            movesList.push_back(getMoveForAPosition(position, moves[i], BLACK_PAWN, false));
    }

    possibilityCastle(movesList, false);

    return movesList;
 }
/*
void ChessBoard::moveOrdering(std::vector<Move>* moves) {

    int index = 0;
    for (int i = 1; i < moves->size(); ++i) {
        if ((*moves)[i].capturedType != NONE) {


            std::swap((*moves)[index], (*moves)[i]);
            ++index;
            }
    }
 }
*/

void ChessBoard::moveOrdering(std::vector<Move>* moves) {
    // Valeurs des pièces
    static const int pieceValues[] = {
        100, 100,   // PAWN
        500, 500,   // ROOK  
        300, 300,   // KNIGHT
        300, 300,   // BISHOP
        900, 900,   // QUEEN
        0, 0        // KING
    };

    std::sort(moves->begin(), moves->end(), [](const Move& a, const Move& b) {
        int scoreA = 0, scoreB = 0;
        
        if (a.capturedType != NONE)
            scoreA = pieceValues[a.capturedType] * 10 - pieceValues[a.piece];
        if (b.capturedType != NONE)
            scoreB = pieceValues[b.capturedType] * 10 - pieceValues[b.piece];
            
        return scoreA > scoreB;
    });
}


void ChessBoard::printMove(Move& move) {
    std::cout << "                                                 " << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Move.from " << move.from << std::endl;
    std::cout << "Move.to " << move.to << std::endl;
    std::cout << "Move.piece " << move.piece << std::endl;
    std::cout << "Move.capturedType " << move.capturedType << std::endl;
    std::cout << "Move.moveType " << move.moveType << std::endl;
    std::cout << "Move.castlingType " << move.castlingType << std::endl;
    std::cout << "Move.whiteKingSideCastlingBefore " << move.whiteKingSideCastlingBefore << std::endl;
    std::cout << "Move.whiteQueenSideCastlingBefore " << move.whiteQueenSideCastlingBefore << std::endl;
    std::cout << "Move.blackKingSideCastlingBefore " << move.blackKingSideCastlingBefore << std::endl;
    std::cout << "Move.blackQueenSideCastlingBefore " << move.blackQueenSideCastlingBefore << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "                                                 " << std::endl;

}

bool ChessBoard::makeMove(Move& move) {

    hashHistory.push_back(currentHash);

    // Save before change flags
    move.whiteKingSideCastlingBefore = whiteKingSideCastling;
    move.whiteQueenSideCastlingBefore = whiteQueenSideCastling;
    move.blackKingSideCastlingBefore = blackKingSideCastling;
    move.blackQueenSideCastlingBefore = blackQueenSideCastling;

    move.whiteKingSideCastlingAfter = whiteKingSideCastling;
    move.whiteQueenSideCastlingAfter = whiteQueenSideCastling;
    move.blackKingSideCastlingAfter = blackKingSideCastling;
    move.blackQueenSideCastlingAfter = blackQueenSideCastling;

    move.enPassantSquareBefore = enPassant;
    enPassant = -1;

    if (move.capturedType != NONE) {
        piece.bitboards[move.capturedType] &= ~(1ULL << move.to);

        if (move.capturedType == WHITE_ROOK && move.to == 7) {
            whiteKingSideCastling = false;
            move.whiteKingSideCastlingAfter = false;
            }
        if (move.capturedType == WHITE_ROOK && move.to == 0) {
            whiteQueenSideCastling = false;
            move.whiteQueenSideCastlingAfter = false;
            }
        if (move.capturedType == BLACK_ROOK && move.to == 56) {
            blackQueenSideCastling = false;
            move.blackKingSideCastlingAfter = false;
            }
        if (move.capturedType == BLACK_ROOK && move.to == 63) {
            blackKingSideCastling = false;
            move.blackQueenSideCastlingAfter = false;
            }
        }

    if (((move.piece == WHITE_PAWN) ^ (move.piece == BLACK_PAWN) ) && ((move.to >> 3) == 7) ^ ((move.to >> 3) == 0)) {
       piece.bitboards[move.piece] &= ~(1ULL << move.from); // Delete Pawn
       piece.bitboards[(move.piece == WHITE_PAWN ? WHITE_QUEEN : BLACK_QUEEN)] |= (1ULL << move.to);
       move.enPassantSquareAfter = enPassant; 
       currentHash = zobrist.updateHash(currentHash, move);
       return true; // Pawn Become Queen
    } 

    bool isWhite = move.piece < 6;

    if (move.piece == WHITE_KING) {
        whiteKingSideCastling = false;
        whiteQueenSideCastling = false;
        move.whiteKingSideCastlingAfter = false;
        move.whiteQueenSideCastlingAfter = false;
    }

    if (move.piece == BLACK_KING) {
        blackKingSideCastling = false;
        blackQueenSideCastling = false;
        move.blackKingSideCastlingAfter = false;
        move.blackQueenSideCastlingAfter = false;
    }

    if (move.piece == WHITE_ROOK) {
        if (move.from == 7) {
            whiteKingSideCastling = false;
            move.whiteKingSideCastlingAfter = false;
        }
        if (move.from == 0) {
            whiteQueenSideCastling = false;
            move.whiteQueenSideCastlingAfter = false;
        }
    }

    if (move.piece == BLACK_ROOK) {
        if (move.from == 63) {
            blackKingSideCastling = false;
            move.blackKingSideCastlingAfter = false;
        }
        if (move.from == 56) {
            blackQueenSideCastling = false;
            move.blackQueenSideCastlingAfter = false;
        }
    }


    if (move.moveType == CASTLING) {  

        // King Move :
        piece.bitboards[move.piece] &= ~(1ULL << move.from);
        piece.bitboards[move.piece] |= (1ULL << move.to);

        if (isWhite) {
            if (move.castlingType == KINGSIDE) {
                piece.bitboards[WHITE_ROOK] &= ~(1ULL << 7); // Remove Rook
                piece.bitboards[WHITE_ROOK] |= (1ULL << 5);
            } else {
                piece.bitboards[WHITE_ROOK] &= ~(1ULL << 0);
                piece.bitboards[WHITE_ROOK] |= (1ULL << 3);
            }
        } else {
            if (move.castlingType == KINGSIDE) {
                piece.bitboards[BLACK_ROOK] &= ~(1ULL << 63); // Remove Rook
                piece.bitboards[BLACK_ROOK] |= (1ULL << 61);
            } else {
                piece.bitboards[BLACK_ROOK] &= ~(1ULL << 56);
                piece.bitboards[BLACK_ROOK] |= (1ULL << 59);
            }

        }
        move.enPassantSquareAfter = enPassant; 
        currentHash = zobrist.updateHash(currentHash, move);
        return false;
        }

        if (move.moveType == EN_PASSANT) {
            piece.bitboards[move.piece] &= ~(1ULL << move.from);
            piece.bitboards[move.piece] |= (1ULL << move.to);

            if (isWhite) 
                piece.bitboards[BLACK_PAWN] &= ~(1ULL << (move.to - 8));
            else 
                piece.bitboards[WHITE_PAWN] &= ~(1ULL << (move.to + 8));

            move.enPassantSquareAfter = enPassant;
            currentHash = zobrist.updateHash(currentHash, move);
            return false;
        }


        if (move.piece == WHITE_PAWN && (move.to - move.from) == 16) 
            enPassant = move.from + 8;
        if (move.piece == BLACK_PAWN && (move.from - move.to) == 16) 
            enPassant = move.from - 8;

        move.enPassantSquareAfter = enPassant; 
        // Normal Move
        piece.bitboards[move.piece] &= ~(1ULL << move.from);
        piece.bitboards[move.piece] |= (1ULL << move.to);
        currentHash = zobrist.updateHash(currentHash, move);
        return false;
}

void ChessBoard::unMakeMove(bool pawnBecomeQueen, Move& move) {
    if (move.capturedType != NONE && move.moveType != EN_PASSANT)  
        piece.bitboards[move.capturedType] |= (1ULL << move.to);

    whiteKingSideCastling = move.whiteKingSideCastlingBefore;
    whiteQueenSideCastling = move.whiteQueenSideCastlingBefore;
    blackKingSideCastling = move.blackKingSideCastlingBefore;
    blackQueenSideCastling = move.blackQueenSideCastlingBefore;

    enPassant = move.enPassantSquareBefore;
    
    if (pawnBecomeQueen) {
        piece.bitboards[(move.piece == WHITE_PAWN ? WHITE_QUEEN : BLACK_QUEEN)] &= ~(1ULL << move.to); // Remove the Queen
        piece.bitboards[move.piece] |= (1ULL << move.from); // Add the Pawn
        return;
    } 
    
    bool isWhite = move.piece < 6;

    if (move.moveType == CASTLING) {
        // For the King
        piece.bitboards[move.piece] |= (1ULL << move.from);
        piece.bitboards[move.piece] &= ~(1ULL << move.to);

        if (isWhite) {
            if (move.castlingType == KINGSIDE) {
                piece.bitboards[WHITE_ROOK] |= (1ULL << 7);
                piece.bitboards[WHITE_ROOK] &= ~(1ULL << 5);
            } else {
                piece.bitboards[WHITE_ROOK] |= (1ULL << 0);
                piece.bitboards[WHITE_ROOK] &= ~(1ULL << 3);
            }
        } else {
            if (move.castlingType == KINGSIDE) {
                piece.bitboards[BLACK_ROOK] |= (1ULL << 63);
                piece.bitboards[BLACK_ROOK] &= ~(1ULL << 61);
            } else {
                piece.bitboards[BLACK_ROOK] |= (1ULL << 56);
                piece.bitboards[BLACK_ROOK] &= ~(1ULL << 59);
            }
        }
    } else if (move.moveType == EN_PASSANT) {
        piece.bitboards[move.piece] |= (1ULL << move.from);
        piece.bitboards[move.piece] &= ~(1ULL << move.to);

        if (isWhite) 
            piece.bitboards[BLACK_PAWN] |= (1ULL << (move.to - 8));
        else 
            piece.bitboards[WHITE_PAWN] |= (1ULL << (move.to + 8));

    }
    
    
    else {
        piece.bitboards[move.piece] |= (1ULL << move.from);
        piece.bitboards[move.piece] &= ~(1ULL << move.to);
    }

    currentHash = hashHistory.back();
    hashHistory.pop_back();
    
}


int ChessBoard::alphaBeta(int depth, bool isWhite, int alpha, int beta) {
    counter_alpha_beta++;
    bool cutoff = false;

    auto it = transpositionTable.find(currentHash);
    if (it != transpositionTable.end() && it->second.depth >= depth) {
        counter_same_hash++;
        return it->second.score;
    }

    if (depth == 0) {
        int score = evaluatePawnPower();
        TTEntry tt;
        tt.score = score;
        tt.depth = depth;
        transpositionTable[currentHash] = tt;  
        
        return score;
    }

    bool hasLegalMove = false;
    if (isWhite) {
        int max_ = -1000;

        std::vector<Move> moves = allMovesForWhite();
        moveOrdering(&moves);
        
        for (Move& move : moves) {

            bool pawnBecomeQueen = makeMove(move);

            if (!isInCheck(true)) {
                hasLegalMove = true;
                int eval = alphaBeta(depth -1, false, alpha, beta);
                alpha = std::max(alpha, eval);
                max_ = std::max(max_, eval);
            }
            
            // Undo
            unMakeMove(pawnBecomeQueen, move);

            if (beta <= alpha) {
                cutoff = true;
                break;
            }

        }

        if (!hasLegalMove) {
            if (isInCheck(true))
                return -10000 - depth; // Mat
            else
                return 0; // Pat

        }
        if (!cutoff) {
            TTEntry tt;
            tt.score = max_;
            tt.depth = depth;
            transpositionTable[currentHash] = tt;
        }
        return max_;
    } else {
        int min_ = 1000;

        std::vector<Move> moves = allMovesForBlack();
        moveOrdering(&moves);

        for (Move& move : moves) {
            bool pawnBecomeQueen = makeMove(move);

            if (!isInCheck(false)) {
                hasLegalMove = true;
                int eval = alphaBeta(depth -1, true, alpha, beta);
                min_ = std::min(min_, eval);
                beta = std::min(beta, eval);   
            }
            
            // Undo
            unMakeMove(pawnBecomeQueen, move);

            if (beta <= alpha) {
                cutoff = true;
                break;
            }
        }

        if (!hasLegalMove) {
            if (isInCheck(false))
                return 10000 + depth; // Mat
            else
                return 0; // Pat
        }
        if (!cutoff) {
            TTEntry tt;
            tt.score = min_;
            tt.depth = depth;
            transpositionTable[currentHash] = tt;
        }
        return min_;
    }
 }


void ChessBoard::AI_chess(bool AIplaysBlack) {
    int depth = 6;
    bool hasLegalMove = false;
    auto start = std::chrono::high_resolution_clock::now();
    
    
    std::vector<Move> moves;
    int max_;
    int min_;

    if (AIplaysBlack) {
        min_ = 1000;
        moves = allMovesForBlack();
    } else {
        max_ = -1000;
        moves = allMovesForWhite();
        }

    Move move_;
    for (Move& move : moves) {

        bool pawnBecomeQueen = makeMove(move);

        if (!isInCheck(!AIplaysBlack)) {

            if (!hasLegalMove) {
                move_ = move;
            }
            hasLegalMove = true;
            int eval;
            if (AIplaysBlack) {
                eval = alphaBeta(depth, true, -1000, 1000); 
            } else {
                eval = alphaBeta(depth, false, -1000, 1000); 
            }

            if (AIplaysBlack) {
                if (eval < min_) {
                    move_ = move;
                    min_ = eval;
                    }
            } else {
                if (eval > max_) {
                    move_ = move;
                    max_ = eval;
                    }
            }

        }

        // Undo
        unMakeMove(pawnBecomeQueen, move);
    }

    if (!hasLegalMove) {
        if (isInCheck(!AIplaysBlack)) {
            std::cout << "AI is in checkmate" << std::endl;
            window.setTitle("AI is in checkmate");
            }
        else {
            std::cout << "AI is in pat" << std::endl;
            window.setTitle("AI is in pat");
            }
        return;
        }

    // DO THE BEST MOVE
    if (hasLegalMove)
        makeMove(move_);

    std::vector<Move> myPossiblesMoves = AIplaysBlack ? allMovesForWhite() : allMovesForBlack();
    hasLegalMove = false;

    for (Move& myMove : myPossiblesMoves) {
        bool pawnBecomeQueen = makeMove(myMove);
        if (!isInCheck(AIplaysBlack)) {
           hasLegalMove = true;
            unMakeMove(pawnBecomeQueen, myMove);
            break;  
        }
        unMakeMove(pawnBecomeQueen, myMove);
    }

    if (!hasLegalMove) {
        if (isInCheck(AIplaysBlack)) {
            std::cout << "You are in checkmate" << std::endl;
            window.setTitle("You are in checkmate");
        }
        else {
            std::cout << "You are in pat" << std::endl;
            window.setTitle("You are in pat");
            }


    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    if (transpositionTable.size() > 100000) {
        transpositionTable.clear();
        hashHistory.clear();
    }

    std::cout << "size transpositionTable : " << transpositionTable.size() << std::endl;
    //transpositionTable.clear();
    std::cout << "size transpositionTable  : " << transpositionTable.size() << " counter_same_hash :"<< counter_same_hash << std::endl;
    std::cout << "counter_alpha_beta " << counter_alpha_beta << std::endl;
    std::cout << "time : " << duration.count() / 1000.0f << " s" << std::endl;
    std::cout << std::endl;
    counter_alpha_beta = 0;
    counter_same_hash = 0;

    std::cout << "=== TEST HASH ===" << std::endl;
    
    //uint64_t hashInitial = currentHash;
    //std::cout << "Hash initial: " << hashInitial << std::endl;
    
    // Génère tous les coups blancs

    /*
    moves = allMovesForWhite();
    
    if (moves.size() > 0) {
        Move& testMove = moves[0];
        
        // Fait le coup
        bool pawn = makeMove(testMove);
        uint64_t hashAfterMove = currentHash;
        std::cout << "Hash après coup: " << hashAfterMove << std::endl;
        
        // Annule le coup
        unMakeMove(pawn, testMove);
        uint64_t hashAfterUndo = currentHash;
        std::cout << "Hash après undo: " << hashAfterUndo << std::endl;
        
        // Vérifie
        if (hashInitial == hashAfterUndo) {
            std::cout << "✅ HASH CORRECT après undo" << std::endl;
        } else {
            std::cout << "❌ HASH FAUX après undo !" << std::endl;
            std::cout << "Différence: " << (hashInitial - hashAfterUndo) << std::endl;
        }
        
        // Test 2 : Refaire le même coup doit donner le même hash
        bool pawn2 = makeMove(testMove);
        uint64_t hashSecondMove = currentHash;
        
        if (hashAfterMove == hashSecondMove) {
            std::cout << "✅ HASH IDENTIQUE pour le même coup" << std::endl;
        } else {
            std::cout << "❌ HASH DIFFÉRENT pour le même coup !" << std::endl;
        }
        
        unMakeMove(pawn2, testMove);
    }
    
    std::cout << "=== FIN TEST ===" << std::endl;
    */
}





