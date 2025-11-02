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



 uint64_t ChessBoard::whitePieceSelected(int& position) {
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
    return 0x0ULL; 
 }


 bool ChessBoard::isThereAWhitePieceAt(int& position) {
    if ((piece.whitePawns >> position) & 1ULL)
        return true;
    if ((piece.whiteRooks >> position) & 1ULL)
        return true;
    if ((piece.whiteBishops >> position) & 1ULL)
        return true;
    if ((piece.whiteKing >> position) & 1ULL)
        return true;
    if ((piece.whiteQueen >> position) & 1ULL)
        return true;
    if ((piece.whiteKnights >> position) & 1ULL)
        return true;
    return false; 
 }

 bool ChessBoard::isThereABlackPieceAt(int position) {
    if ((piece.blackPawns >> position) & 1ULL)
        return true;
    if ((piece.blackRooks >> position) & 1ULL)
        return true;
    if ((piece.blackBishops >> position) & 1ULL)
        return true;
    if ((piece.blackKing >> position) & 1ULL)
        return true;
    if ((piece.blackQueen >> position) & 1ULL)
        return true;
    if ((piece.blackKnights >> position) & 1ULL)
        return true;
    return false; 
 }
 

bool ChessBoard::isThereAPieceAt(int position) {
    return isThereAWhitePieceAt(position) || isThereABlackPieceAt(position);
}


int ChessBoard::possibilityWhitePawn(int& position, int* moves) {
   
    int count = 0;
    if (!isThereAPieceAt(position + 8))
        moves[count++] = position + 8;

    if (position < 16 && position > 7) 
        if (!isThereAPieceAt(position + 16))
            moves[count++] = position + 16;

    if (position % 8 != 0) // left part of the board
        if (isThereABlackPieceAt(position + 7))
            moves[count++] = position + 7;

    if (position % 8 != 7) // right part of the board
        if (isThereABlackPieceAt(position + 9))
            moves[count++] = position + 9;

    return count;
 }
 




