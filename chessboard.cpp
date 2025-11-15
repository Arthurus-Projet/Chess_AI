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


void ChessBoard::drawChessPieces(sf::RenderWindow& window, uint64_t piece, sf::Sprite & sprite) {

    std::vector<int> positions;
    uint64_t copyPawns = piece;
    while (copyPawns) {
        int square = __builtin_ctzll(copyPawns); 
        positions.push_back(square);
        copyPawns &= copyPawns - 1; 
    }

    for (int n : positions) {
        sprite.setPosition( (n % 8) * squareSize  , (squareSize * 7) - (n / 8) * squareSize);
        window.draw(sprite); 
        }
}


void ChessBoard::drawAllPieces(sf::RenderWindow& window) {

    // Pawns :
    drawChessPieces(window, piece.whitePawns, whitePawnSprite);
    drawChessPieces(window, piece.blackPawns, blackPawnSprite);

    // Rooks:
    drawChessPieces(window, piece.whiteRooks, whiteTowerSprite);
    drawChessPieces(window, piece.blackRooks, blackTowerSprite);

    // Bishops:
    drawChessPieces(window, piece.whiteBishops, whiteBishopSprite);
    drawChessPieces(window, piece.blackBishops, blackBishopSprite);

    // Knights:
    drawChessPieces(window, piece.whiteKnights, whiteKnightSprite);
    drawChessPieces(window, piece.blackKnights, blackKnightSprite);

    //Kings:
    drawChessPieces(window, piece.whiteKing, whiteKingSprite);
    drawChessPieces(window, piece.blackKing, blackKingSprite);

    //Queens:
    drawChessPieces(window, piece.whiteQueen, whiteQueenSprite);
    drawChessPieces(window, piece.blackQueen, blackQueenSprite);

}



 uint64_t& ChessBoard::whitePieceSelected(int& position) {
    if ((piece.whitePawns >> position) & 1ULL)
        return piece.whitePawns;
    if ((piece.whiteRooks >> position) & 1ULL)
        return piece.whiteRooks;
    if ((piece.whiteBishops >> position) & 1ULL)
        return piece.whiteBishops;
    if ((piece.whiteKing >> position) & 1ULL)
        return piece.whiteKing;
    if ((piece.whiteQueen >> position) & 1ULL)
        return piece.whiteQueen;
    if ((piece.whiteKnights >> position) & 1ULL)
        return piece.whiteKnights;
    static uint64_t nullPiece = 0x0ULL;
    return nullPiece; 
 }


  uint64_t& ChessBoard::PieceSelected(int& position) {
    if ((piece.whitePawns >> position) & 1ULL)
        return piece.whitePawns;
    if ((piece.whiteRooks >> position) & 1ULL)
        return piece.whiteRooks;
    if ((piece.whiteBishops >> position) & 1ULL)
        return piece.whiteBishops;
    if ((piece.whiteKing >> position) & 1ULL)
        return piece.whiteKing;
    if ((piece.whiteQueen >> position) & 1ULL)
        return piece.whiteQueen;
    if ((piece.whiteKnights >> position) & 1ULL)
        return piece.whiteKnights;
    if ((piece.blackPawns >> position) & 1ULL)
        return piece.blackPawns;
    if ((piece.blackRooks >> position) & 1ULL)
        return piece.blackRooks;
    if ((piece.blackBishops >> position) & 1ULL)
        return piece.blackBishops;
    if ((piece.blackKing >> position) & 1ULL)
        return piece.blackKing;
    if ((piece.blackQueen >> position) & 1ULL)
        return piece.blackQueen;
    if ((piece.blackKnights >> position) & 1ULL)
        return piece.blackKnights;
    
    static uint64_t nullPiece = 0x0ULL;
    return nullPiece;
 }



 inline bool ChessBoard::isThereAWhitePieceAt(int position) {
    uint64_t mask = 1ULL << position;
    return mask & (piece.whiteBishops | piece.whiteKing |
                   piece.whiteKnights | piece.whitePawns |
                   piece.whiteQueen | piece.whiteRooks);
 }

 inline bool ChessBoard::isThereABlackPieceAt(int position) {
    uint64_t mask = 1ULL << position;
    return mask & (piece.blackBishops | piece.blackKing |
                   piece.blackKnights | piece.blackPawns |
                   piece.blackQueen | piece.blackRooks);
 }
 

inline bool ChessBoard::isThereAPieceAt(int position) {
    uint64_t mask = 1ULL << position;
    return mask & (piece.whitePawns | piece.whiteRooks | piece.whiteBishops |
                   piece.whiteKnights | piece.whiteQueen | piece.whiteKing |
                   piece.blackPawns | piece.blackRooks | piece.blackBishops |
                   piece.blackKnights | piece.blackQueen | piece.blackKing);
}


int ChessBoard::possibilityWhiteTower(int position, int* moves) {

    int count = 0;

    // Move right (+1)
    for (int i = 1; ((position + i) & 7) != 0; i++) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    // Move left (-1)
    for (int i = -1; ((position + i) & 7) != 7; i--) {
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
    for (int i = 1; ((position + i) & 7) != 0; i++) {
        if (isThereABlackPieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereAWhitePieceAt(position + i)) { 
            break;
        }
    }

    // Move left (-1)
    for (int i = -1; ((position + i) & 7) != 7; i--) {
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
    for (int i = 9; ((position + i) & 7) != 0; i+= 9) {
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

    std::cout << "Are you in the left part of the board : " << ((position & 7) != 0) << std::endl;
    std::cout << "Are you in the right part of the board : " << ((position & 7) != 7) << std::endl;
    // left part of the board
    if (((position & 7) != 0) && isThereABlackPieceAt(position + 7)) // 7 = 111 [position & 7 != 0] = [position % 8 != 0] 
        moves[count++] = position + 7;

    // right part of the board
    if (((position & 7) != 7) && isThereABlackPieceAt(position + 9))
        moves[count++] = position + 9;

    return count;
 }



 
int ChessBoard::possibilityWhiteQueen(int position, int* moves) {

    int count = 0;

    // Like Bishop :

    // Move up right (+9)
    for (int i = 9; ((position + i) & 7) != 0; i+= 9) {
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
    for (int i = 1; ((position + i) & 7) != 0; i++) {
        if (isThereAWhitePieceAt(position + i))
            break;

        moves[count++] = position + i;
        if (isThereABlackPieceAt(position + i)) { 
            break;
        }
    }

    // Move left (-1)
    for (int i = -1; ((position + i) & 7) != 7; i--) {
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
 




