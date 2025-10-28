#include <SFML/Graphics.hpp>
#include "Headers/chessboard.h"
#include <bitset>
#include <iostream>



int main()
{
    const int windowSize = 1000;
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Chess AI");

    ChessBoard board(windowSize, windowSize);  // creation of the class to display the board

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        board.draw(window); // draw the chessboard

       
        sf::Sprite whitePawnSprite;
        whitePawnSprite.setTexture(board.textures["pion_b"]);

        sf::Sprite blackPawnSprite;
        blackPawnSprite.setTexture(board.textures["pion"]);
                
        sf::Sprite whiteTowerSprite;
        whiteTowerSprite.setTexture(board.textures["tour_b"]);

        sf::Sprite blackTowerSprite;
        blackTowerSprite.setTexture(board.textures["tour"]);

        sf::Sprite whiteBishopSprite;
        whiteBishopSprite.setTexture(board.textures["fou_b"]);

        sf::Sprite blackBishopSprite;
        blackBishopSprite.setTexture(board.textures["fou"]);

        sf::Sprite whiteKnightSprite;
        whiteKnightSprite.setTexture(board.textures["cavalier_b"]);

        sf::Sprite blackKnightSprite;
        blackKnightSprite.setTexture(board.textures["cavalier"]);

        sf::Sprite whiteKingSprite;
        whiteKingSprite.setTexture(board.textures["roi_b"]);

        sf::Sprite blackKingSprite;
        blackKingSprite.setTexture(board.textures["roi"]);

        sf::Sprite whiteQueenSprite;
        whiteQueenSprite.setTexture(board.textures["reine_b"]);

        sf::Sprite blackQueenSprite;
        blackQueenSprite.setTexture(board.textures["reine"]);

        
        

        uint64_t whitePawns = 0xFF00ULL; 
        uint64_t blackPawns = 0x00FF000000000000ULL;

        uint64_t whiteRooks = 0x0000000000000081ULL;
        uint64_t blackRooks = 0x8100000000000000ULL;

        uint64_t whiteBishops = 0x0000000000000024ULL;
        uint64_t blackBishops = 0x2400000000000000ULL;

        uint64_t whiteKnights = 0x0000000000000042ULL;
        uint64_t blackKnights = 0x4200000000000000ULL;

        uint64_t whiteKing = 0x0000000000000010ULL;
        uint64_t blackKing = 0x1000000000000000ULL;

        uint64_t whiteQueen = 0x0000000000000008ULL;
        uint64_t blackQueen = 0x0800000000000000ULL;




        std::cout << "White pawns:\n" << std::bitset<64>(whitePawns) << "\n\n";

       

        // Pawns :
        board.drawChessPieces(window, whitePawns, whitePawnSprite);
        board.drawChessPieces(window, blackPawns, blackPawnSprite);

        // Rooks:
        board.drawChessPieces(window, whiteRooks, whiteTowerSprite);
        board.drawChessPieces(window, blackRooks, blackTowerSprite);

        // Bishops:
        board.drawChessPieces(window, whiteBishops, whiteBishopSprite);
        board.drawChessPieces(window, blackBishops, blackBishopSprite);

        // Knights:
        board.drawChessPieces(window, whiteKnights, whiteKnightSprite);
        board.drawChessPieces(window, blackKnights, blackKnightSprite);

        //Kings:
        board.drawChessPieces(window, whiteKing, whiteKingSprite);
        board.drawChessPieces(window, blackKing, blackKingSprite);

        //Queens:
        board.drawChessPieces(window, whiteQueen, whiteQueenSprite);
        board.drawChessPieces(window, blackQueen, blackQueenSprite);



        




        
      


        window.display();
    }

    return 0;
}
