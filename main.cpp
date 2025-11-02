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

                    uint64_t piece = board.whitePieceSelected(position);
                    if (piece == board.piece.whitePawns)
                        std::cout << "Pawn selected" << std::endl; 
                    else if (piece == board.piece.whiteRooks)
                        std::cout << "Rook selected" << std::endl; 
                    else if (piece == board.piece.whiteBishops)
                        std::cout << "Bishop selected" << std::endl; 
                    else if (piece == board.piece.whiteKing)
                        std::cout << "King selected" << std::endl; 
                    else if (piece == board.piece.whiteQueen)
                        std::cout << "Queen selected" << std::endl; 
                    else if (piece == board.piece.whiteKnights)
                        std::cout << "Knight selected" << std::endl; 

                }
            }
        }

        window.clear();

        board.draw(window); // draw the chessboard

        
        board.drawAllPieces(window);
        //std::cout << "White pawns:\n" << std::bitset<64>(whitePawns) << "\n\n";

    
        


        window.display();
    }

    return 0;
}
