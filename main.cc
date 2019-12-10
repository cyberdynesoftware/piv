#include "Image.h"
#include "Folder.h"
#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "usage: piv <path>" << std::endl;
        return 1;
    }

    Folder folder(argv[1]);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "piv", sf::Style::Fullscreen);
    window.draw(folder.getImage()->getSprite());
    window.display();

    sf::Event event;
    int mouseX = sf::Mouse::getPosition().x;
    int mouseY = sf::Mouse::getPosition().y;

    while (window.isOpen())
    {
        window.waitEvent(event);

        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::Q:
                    window.close();
                    break;
                case sf::Keyboard::Space:
                    folder.next();
                    break;
                case sf::Keyboard::Backspace:
                    folder.previous();
                    break;
                case sf::Keyboard::R:
                    folder.random();
                    break;
                case sf::Keyboard::A:
                    folder.select();
                    folder.next();
                    break;
                case sf::Keyboard::D:
                    folder.trash();
                    break;
                default:
                    break;
            }
        }
        if (event.type == sf::Event::MouseWheelScrolled)
        {
            folder.getImage()->zoom(event.mouseWheelScroll);
        }
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Button::Right)
                folder.getImage()->fitToScreen();
        }
        if (event.type == sf::Event::MouseMoved)
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                folder.getImage()->move(event.mouseMove.x - mouseX, event.mouseMove.y - mouseY);

            mouseX = event.mouseMove.x;
            mouseY = event.mouseMove.y;
        }

        window.clear();
        window.draw(folder.getImage()->getSprite());
        window.display();
    }

    return EXIT_SUCCESS;
}
