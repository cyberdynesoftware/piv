#include "Image.h"
#include "Folder.h"
#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "usage: piv <path>" << std::endl;
        return EXIT_FAILURE;
    }

    Folder folder(argv[1]);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "piv", sf::Style::Fullscreen);
    window.draw(folder.getImage()->getSprite());
    window.display();

    sf::Cursor arrow;
    arrow.loadFromSystem(sf::Cursor::Arrow);
    sf::Cursor cross;
    cross.loadFromSystem(sf::Cursor::Cross);

    sf::Event event;
    sf::Vector2i previousMousePosition = sf::Mouse::getPosition();

    while (window.isOpen())
    {
        window.waitEvent(event);

        switch (event.type)
        {
            case sf::Event::KeyPressed:
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
                break;

            case sf::Event::MouseWheelScrolled:
                folder.getImage()->zoom(event.mouseWheelScroll.delta);
                break;

            case sf::Event::MouseButtonPressed:
                switch (event.mouseButton.button)
                {
                    case sf::Mouse::Button::Left:
                        window.setMouseCursor(cross);
                        break;
                    case sf::Mouse::Button::Right:
                        folder.getImage()->fitToScreen();
                        break;
                    default:
                        break;
                }
                break;

            case sf::Event::MouseButtonReleased:
                switch (event.mouseButton.button)
                {
                    case sf::Mouse::Button::Left:
                        window.setMouseCursor(arrow);
                        break;
                    default:
                        break;
                }
                break;

            case sf::Event::MouseMoved:
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                    folder.getImage()->move(sf::Mouse::getPosition() - previousMousePosition);

                previousMousePosition = sf::Mouse::getPosition();
                break;

            default:
                break;
        }

        window.clear();
        window.draw(folder.getImage()->getSprite());
        window.display();
    }

    return EXIT_SUCCESS;
}
