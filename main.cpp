#include <SFML/Graphics.hpp>
#include "Headers/chessboard.h"



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



        window.display();
    }

    return 0;
}
