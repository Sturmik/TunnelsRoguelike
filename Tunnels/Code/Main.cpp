#include "UtilityTime.h"
#include <iostream>

int main()
{
    // Create window and set its parameters
    sf::RenderWindow window;
    window.create(sf::VideoMode(800, 600), "Tunnels");
    window.setFramerateLimit(60);
    // Start loop
    while (window.isOpen())
    {
        UtilityTime::FrameStartTime();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                std::cout << "Window is closed";
                window.close();
            }
        }
        window.clear();

        window.display();
    }
    return 0;
}