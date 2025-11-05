#include <SFML/Graphics.hpp>
#include "Headers/chessboard.h"
#include <bitset>
#include <iostream>



int main()
{
    const int windowSize = 1000;
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Chess AI");

    ChessBoard board(windowSize, windowSize);  // creation of the class to display the board
    bool rightClic = false;
    int positionRightClic;
    int positionRightClic2;

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

                    int position = board.mouseToPosition(x, y, size);
                    
                    std::cout << "Mouse clicked at: " << position << std::endl;

                    uint64_t piece = board.whitePieceSelected(position);
                    if (piece == board.piece.whitePawns) {
                        std::cout << "Pawn selected" << std::endl; 
                        int moves[4];
                        int nMoves = board.possibilityWhitePawn(position, moves);

                        for (int i = 0; i < nMoves; i++)
                            std::cout << "Pawn white : " << moves[i] << std::endl;


                    }
                        
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

                if (event.mouseButton.button == sf::Mouse::Right ) {
                    std::cout << "RIGHT click" << std::endl;
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;
                    sf::Vector2u size = window.getSize(); // size.x size.y

                    if (!rightClic)
                        positionRightClic = board.mouseToPosition(x, y, size);
                    else
                        positionRightClic2 = board.mouseToPosition(x, y, size);

                    rightClic = !rightClic;

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
