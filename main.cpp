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

       
        sf::Sprite knightWhite;
        knightWhite.setTexture(board.textures["pion_b"]);
                
        

        uint64_t whitePawns = 0xFF00ULL; 
        std::cout << "White pawns:\n" << std::bitset<64>(whitePawns) << "\n\n";


        std::vector<int> positionsWhitePawns; 
        uint64_t pawns = whitePawns;
        while (pawns) {
            int square = __builtin_ctzll(pawns); 
            std::cout << "Pion blanc sur la case " << square << std::endl;
            positionsWhitePawns.push_back(square);
            pawns &= pawns - 1; 
        }
        
        for (int n : positionsWhitePawns) {
            std::cout << n << std::endl;
            knightWhite.setPosition( (n % 8) * board.squareSize  , (board.squareSize * 7) - (n / 8) * board.squareSize);
            window.draw(knightWhite); 
        }
        




        
      


        window.display();
    }

    return 0;
}
