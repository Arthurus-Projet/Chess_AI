#include <SFML/Graphics.hpp>
#include "Headers/chessboard.h"
#include <bitset>
#include <iostream>
#include <optional>
#include <functional>


int main()
{
    const int windowSize = 1000;
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Chess AI");

    ChessBoard board(windowSize, windowSize);  // creation of the class to display the board

    // Left Click :
    bool leftClick = false;
    int position;
    int position2;
    uint64_t* pieceLeftClick;
    uint64_t* pieceLeftClick2;
    int moves[4];
    int nMoves;
    uint64_t possibilityMove_ = 0x0ULL;

    // Right Click :
    bool rightClick = false;
    int positionrightClick;
    int positionrightClick2;
    uint64_t* piece = nullptr;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {

                // Left Click :
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;
                    sf::Vector2u size = window.getSize(); // size.x size.y

                    leftClick = !leftClick;

                    

                    if (leftClick) {

                        position = board.mouseToPosition(x, y, size);
                    
                        std::cout << "Mouse clicked at: " << position << std::endl;

                        pieceLeftClick = &board.whitePieceSelected(position);
                        std::cout << "piece memory adress : " << piece << std::endl;

                        if (*pieceLeftClick == board.piece.whitePawns) {
                            std::cout << "Pawn selected" << std::endl; 
                            nMoves = board.possibilityWhitePawn(position, moves);

                            
                            possibilityMove_ = 0x0ULL;
                            for (int i = 0; i < nMoves; i++) {
                                std::cout << "Pawn white : " << moves[i] << std::endl;
                                possibilityMove_ |= (1ULL << moves[i]);
                            }
                            

                            
                        }
                    } else {
                        possibilityMove_ = 0x0ULL;
                        position2 = board.mouseToPosition(x, y, size);
                        pieceLeftClick2 = &board.PieceSelected(position2);
                        bool valideMove = false;

                        for (int i = 0; i < nMoves; i++) {
                            if (position2 == moves[i]) {
                                board.movePiece(pieceLeftClick, pieceLeftClick2, position, position2);
                             
                                break;
                            }

                        }




                    }
                        
                    /*else if (piece == board.piece.whiteRooks)
                        std::cout << "Rook selected" << std::endl; 
                    else if (piece == board.piece.whiteBishops)
                        std::cout << "Bishop selected" << std::endl; 
                    else if (piece == board.piece.whiteKing)
                        std::cout << "King selected" << std::endl; 
                    else if (piece == board.piece.whiteQueen)
                        std::cout << "Queen selected" << std::endl; 
                    else if (piece == board.piece.whiteKnights)
                        std::cout << "Knight selected" << std::endl; 
                        */

                }

                // Right Click :
                if (event.mouseButton.button == sf::Mouse::Right ) {
                    std::cout << "RIGHT click" << std::endl;
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;
                    sf::Vector2u size = window.getSize(); // size.x size.y

                    rightClick = !rightClick;

                    if (rightClick) {
                        positionrightClick = board.mouseToPosition(x, y, size);
                        piece = &board.PieceSelected(positionrightClick);
                        *piece &= ~(1ULL << positionrightClick);
                        std::cout << "Right click delete" << std::endl;
                    } else {
                        positionrightClick2 = board.mouseToPosition(x, y, size);
                        std::cout << "Put piece here : " << positionrightClick2 << std::endl;
                        *piece |= (1ULL << positionrightClick2);
                    }
                     

                }
            }
        }

        window.clear();

        board.draw(window); // draw the chessboard

        board.drawChessPieces(window, possibilityMove_, board.possibilityMove);

        
        board.drawAllPieces(window);
        //std::cout << "White pawns:\n" << std::bitset<64>(whitePawns) << "\n\n";

    
        


        window.display();
    }

    return 0;
}
