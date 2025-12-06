#include <SFML/Graphics.hpp>
#include "Headers/chessboard.h"
#include <bitset>
#include <iostream>
#include <optional>
#include <functional>


int main()
{
    std::string name_window = "chess AI";
    int number = 10;
    int windowSize = 1000;
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), name_window);

    ChessBoard board(windowSize, windowSize, 8, window);  // creation of the class to display the board

    // AI
    bool AIisBlack = true;

    // Left Click :
    bool leftClick = false;
    int position;
    int position2;
    uint64_t* pieceLeftClick;
    uint64_t* pieceLeftClick2;
    int moves[27];
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


            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P) {
                    windowSize += number;
                    window.setSize(sf::Vector2u(windowSize, windowSize));
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::O) {
                    windowSize -= number;
                    window.setSize(sf::Vector2u(windowSize, windowSize));
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {

                // Left Click :
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;
                    sf::Vector2u size = window.getSize(); // size.x size.y

                    leftClick = !leftClick;

                    if (leftClick) {

                        position = board.mouseToPosition(x, y, size);
                    
                        //std::cout << "Mouse clicked at: " << position << std::endl;

                        pieceLeftClick = &board.PieceSelected(position);
                        //std::cout << "piece memory adress : " << piece << std::endl;

                        // To be sure we select a White Piece :
                        if (pieceLeftClick != 0x0ULL) { 
                            if (*pieceLeftClick == board.piece.bitboards[WHITE_PAWN]) {
                                nMoves = board.possibilityWhitePawn(position, moves);
                            } else if (*pieceLeftClick == board.piece.bitboards[BLACK_PAWN]) {
                                nMoves = board.possibilityBlackPawn(position, moves);
                            } else if (*pieceLeftClick == board.piece.bitboards[WHITE_ROOK]) {
                                std::cout << "Tower selected" << std::endl; 
                                nMoves = board.possibilityWhiteTower(position, moves);
                            } else if (*pieceLeftClick == board.piece.bitboards[WHITE_BISHOP]) {
                                nMoves = board.possibilityWhiteBishop(position, moves);
                            } else if (*pieceLeftClick == board.piece.bitboards[WHITE_KNIGHT]) {
                                nMoves = board.possibilityWhiteKnight(position, moves);
                            } else if (*pieceLeftClick == board.piece.bitboards[WHITE_QUEEN]) {
                                nMoves = board.possibilityWhiteQueen(position, moves);
                            } else if (*pieceLeftClick == board.piece.bitboards[BLACK_QUEEN]) {
                                nMoves = board.possibilityBlackQueen(position, moves);
                            } else if (*pieceLeftClick == board.piece.bitboards[WHITE_KING]) {
                                nMoves = board.possibilityWhiteKing(position, moves);
                            } else if (*pieceLeftClick == board.piece.bitboards[BLACK_KING]) {
                                nMoves = board.possibilityBlackKing(position, moves);
                            } else if (*pieceLeftClick == board.piece.bitboards[BLACK_ROOK]) {
                                nMoves = board.possibilityBlackTower(position, moves);
                            } else if (*pieceLeftClick == board.piece.bitboards[BLACK_BISHOP]) {
                                nMoves = board.possibilityBlackBishop(position, moves);
                            } else if (*pieceLeftClick == board.piece.bitboards[BLACK_KNIGHT]) {
                                nMoves = board.possibilityBlackKnight(position, moves);
                            }

                                
                            possibilityMove_ = 0x0ULL;
                            
                            
                            // Add only authorized movements
                            std::vector<Move> moves = board.allMovesForWhite();

                            for (Move& move : moves) {
                                bool pawn = board.makeMove(move);
                                if (board.piece.bitboards[move.piece] == *pieceLeftClick && move.from == position && !board.isInCheck(true))
                                    possibilityMove_ |= (1ULL << move.to);
                                board.unMakeMove(pawn, move);
                            }
                              
                        }                            
                    } else {
                        possibilityMove_ = 0x0ULL;
                        position2 = board.mouseToPosition(x, y, size);
                        pieceLeftClick2 = &board.PieceSelected(position2);
                        bool valideMove = false;

                        std::vector<Move> movesList = board.allMovesForWhite();


                        for (Move& move : movesList) {
                            //std::cout << "[DEBUG] " << move.moveType << " " << move.to << " " << position2 << " " << board.piece.bitboards[move.piece] << " " <<*pieceLeftClick  <<  std::endl;
                            //std::cout << "[DEBUG] Move.from :"  << move.from << " move.to " << move.to << " mode type " << move.moveType << " castlingType " << move.castlingType << " move piece "<< move.piece <<std::endl;
                            if ((position2 == move.to) && (position == move.from) ) {
                                board.makeMove(move);
                                board.AI_chess(AIisBlack);
                                break;
                            }

                        }


                    }
                }

                // Right Click :
                if (event.mouseButton.button == sf::Mouse::Right ) {
                    //std::cout << "RIGHT click" << std::endl;
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;
                    sf::Vector2u size = window.getSize(); // size.x size.y

                    rightClick = !rightClick;

                    if (rightClick) {
                        positionrightClick = board.mouseToPosition(x, y, size);
                        piece = &board.PieceSelected(positionrightClick);
                        *piece &= ~(1ULL << positionrightClick);
                        std::cout << "Right click delete" << std::endl;
                        window.setTitle("Right click delete");
                    } else {
                        positionrightClick2 = board.mouseToPosition(x, y, size);
                        std::cout << "Put piece here : " << positionrightClick2 << std::endl;
                        *piece |= (1ULL << positionrightClick2);
                        window.setTitle("Put piece here");
                    }
                     

                }
            }
        }

     
    


        window.clear();

        board.draw(); // draw the chessboard

        board.drawChessPieces(possibilityMove_, board.possibilityMove);
        
        board.drawAllPieces();

        window.display();
    }
    
    return 0;
}
