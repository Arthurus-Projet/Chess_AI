#include <SFML/Graphics.hpp>
#include "Headers/chessboard.h"
#include <bitset>
#include <iostream>



int main()
{
    const int windowSize = 1000;
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Chess AI");

    ChessBoard board(windowSize, windowSize);  // creation of the class to display the board

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

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;
                    sf::Vector2u size = window.getSize(); // size.x size.y
                    float square_x = static_cast<float>(size.x) / 8.f;
                    float square_y = static_cast<float>(size.y) / 8.f;

                    int y_ = 7 - (static_cast<int>(y / square_y) );

                    
                    int position =  y_ * 8 +  static_cast<int>(x / square_x);
                    std::cout << static_cast<int>(x / square_x)  << " " << static_cast<int>(y / square_y) << std::endl;
                    std::cout << y_ << " " << position << std::endl;
                    //std::cout << "Mouse clicked at: (" << ", " << ")\n";
                }
            }
        }

        window.clear();

        board.draw(window); // draw the chessboard

        

        //std::cout << "White pawns:\n" << std::bitset<64>(whitePawns) << "\n\n";

    
        // Pawns :
        board.drawChessPieces(window, whitePawns, board.whitePawnSprite);
        board.drawChessPieces(window, blackPawns, board.blackPawnSprite);

        // Rooks:
        board.drawChessPieces(window, whiteRooks, board.whiteTowerSprite);
        board.drawChessPieces(window, blackRooks, board.blackTowerSprite);

        // Bishops:
        board.drawChessPieces(window, whiteBishops, board.whiteBishopSprite);
        board.drawChessPieces(window, blackBishops, board.blackBishopSprite);

        // Knights:
        board.drawChessPieces(window, whiteKnights, board.whiteKnightSprite);
        board.drawChessPieces(window, blackKnights, board.blackKnightSprite);

        //Kings:
        board.drawChessPieces(window, whiteKing, board.whiteKingSprite);
        board.drawChessPieces(window, blackKing, board.blackKingSprite);

        //Queens:
        board.drawChessPieces(window, whiteQueen, board.whiteQueenSprite);
        board.drawChessPieces(window, blackQueen, board.blackQueenSprite);



        window.display();
    }

    return 0;
}
