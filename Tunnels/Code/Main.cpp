#include "UtilityTime.h"
#include "GameObject.h"
#include <iostream>

int main()
{
    // Create window and set its parameters
    sf::RenderWindow window;
    window.create(sf::VideoMode(800, 600), "Tunnels");
    window.setFramerateLimit(60);
    // Create game object
    sf::Texture test;
    test.loadFromFile("test.png");
    GameObject obj;
    obj.setPosition(300, 300);
    obj.setScale(sf::Vector2f(200.f, 200.f));
    obj.SetTexture(test);
    int x = obj.getPosition().x;
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
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Right)
                {
                    x = 100 * UtilityTime::GetDeltaTime();
                    obj.move(sf::Vector2f(x, 0));
                }
            }
        }
        window.clear();
        window.draw(obj);
        window.display();
    }
    return 0;
}