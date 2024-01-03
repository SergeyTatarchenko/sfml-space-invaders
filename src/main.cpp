#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
    sf::CircleShape shape1(10.f);
    sf::CircleShape shape2(20.f);


    shape1.setFillColor(sf::Color::Green);
    shape1.setPosition(5.f,5.f);

    shape2.setFillColor(sf::Color::Red);
    shape2.setPosition(30.f,30.f);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape1);
        window.draw(shape2);
        window.display();
    }

    return 0;
}